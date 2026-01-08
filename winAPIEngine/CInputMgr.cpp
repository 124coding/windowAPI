#include "CInputMgr.h"
#include "winMacro.h"

#include "CRenderer.h"

CInputMgr* CInputMgr::mInst = nullptr;
SVector2D CInputMgr::mMousePosition = SVector2D(0.0f, 0.0f);

CInputMgr::~CInputMgr()
{
	// 맵에 동적 할당된 SKeyInfo 객체들을 모두 해제
	for (auto it = mMapKeyInfos.begin(); it != mMapKeyInfos.end(); it++) {
		SAFE_DELETE(it->second);
	}

	mMapKeyInfos.clear();
}

void CInputMgr::KeyClear()
{
	// 윈도우가 포커스를 잃었을 때 호출됨.
	// 키를 누른 채로 창을 내리거나(Alt-Tab) 했을 때, 게임 내에서 계속 눌린 것으로 인식되는 '키 박힘' 버그 방지
	for (auto& it : mMapKeyInfos) {
		if (it.second->mState == eKeyState::Down || it.second->mState == eKeyState::Pressed) {
			it.second->mState = eKeyState::Up;	// 강제로 뗀 상태로 전환
		}
		else if (it.second->mState == eKeyState::Up) {
			it.second->mState = eKeyState::None;
		}

		it.second->bPressed = false;
	}
}

void CInputMgr::GetMousePositionByWindow(HWND tHWND)
{
	POINT mousePos = {};
	GetCursorPos(&mousePos);	// 현재 모니터 전체 기준의 마우스 좌표 가져오기
	ScreenToClient(tHWND, &mousePos);	// 모니터 좌표 -> 해당 윈도우 클라이언트 영역 기준 좌표로 변환

	mMousePosition.mX = -1.0f;
	mMousePosition.mY = -1.0f;
	
	// 마우스가 윈도우 영역 안에 있을 때만 좌표 갱신
	if (mousePos.x > 0 && mousePos.x < windowWidth) {
		mMousePosition.mX = (float)mousePos.x;
	}

	if (mousePos.y > 0 && mousePos.y < windowHeight) {
		mMousePosition.mY = (float)mousePos.y;
	}
}

void CInputMgr::Update(HWND tHWND)
{
	// 윈도우가 현재 활성화(Focus) 되어있을 때만 입력을 처리
	if (GetFocus()) {
		for (auto it = mMapKeyInfos.begin(); it != mMapKeyInfos.end(); it++) {
			int tPushCount = 0;

			// 0x8000: 현재 키가 눌려있는 상태인지 확인 (비트 연산)
			if (GetAsyncKeyState(it->second->mKeyInput) & 0x8000)
			{
				tPushCount++;
			}

			// 키가 눌려있다면 (물리적 입력 O)
			if (tPushCount == 1) {
				// 이전 상태에 따른 상태 전이 (State Machine)
				if (it->second->mState == eKeyState::None) {
					it->second->mState = eKeyState::Down;
				}
				else if (it->second->mState == eKeyState::Down && !it->second->bPressed) {
					it->second->mState = eKeyState::Pressed;
					it->second->bPressed = true;
				}
			}
			// 키가 눌려있지 않다면 (물리적 입력 X)
			else {
				if (it->second->mState == eKeyState::Down || it->second->mState == eKeyState::Pressed) {
					it->second->mState = eKeyState::Up;
					it->second->bPressed = false;
				}
				else if (it->second->mState == eKeyState::Up) {
					it->second->mState = eKeyState::None;
				}
			}
		}

		// 마우스 위치 갱신
		GetMousePositionByWindow(tHWND);
	}
	else {
		// 포커스가 없으면 입력 상태 초기화
		KeyClear();
	}
}

CInputMgr* CInputMgr::GetInst()
{
	if (mInst == nullptr) {
		mInst = new CInputMgr();
	}

	return mInst;
}

void CInputMgr::ReleaseInst()
{
	SAFE_DELETE(mInst);
}

// 아래는 각 상태별 키 입력 확인 함수들
// 키 정보 존재 여부 + 윈도우 포커스 여부 + 해당 상태 여부 체크

bool CInputMgr::GetKeyDown(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (!GetFocus()) return false;
	if (tInfo->mState == eKeyState::Down) return true;
	return false;
}

bool CInputMgr::GetKeyPressed(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (!GetFocus()) return false;
	if (tInfo->mState == eKeyState::Pressed) return true;
	return false;
}

bool CInputMgr::GetKeyUp(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (!GetFocus()) return false;
	if (tInfo->mState == eKeyState::Up) return true;
	return false;
}

bool CInputMgr::GetKeyNone(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (!GetFocus()) return false;
	if (tInfo->mState == eKeyState::None) return true;
	return false;
}