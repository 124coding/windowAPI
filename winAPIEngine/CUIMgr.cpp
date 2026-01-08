#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CInputMgr.h"

#include "CCharacterSelectUI.h"
#include "CWeaponSelectUI.h"
#include "CTitleSceneUI.h"
#include "CPlaySceneUI.h"
#include "CShopUI.h"
#include "CEndingUI.h"
#include "CLevelUpUI.h"

#include "winMacro.h"

std::unordered_map<eUIType, CUIBase*> CUIMgr::mUIs = {};
std::vector<CUIBase*> CUIMgr::mActiveUIs = {};
std::queue<eUIType> CUIMgr::mRequestUIQueue = {};
std::queue<eUIType> CUIMgr::mPopQueue = {};
std::queue<eUIType> CUIMgr::mDeleteQueue = {};
CUIBase* CUIMgr::mPrevHoverUI = nullptr;

void CUIMgr::OnCreate() {
}

void CUIMgr::OnLoad(eUIType tType) {
	CUIBase* pUI = GetUI(tType);

	if (pUI != nullptr) {
		OnComplete(pUI);
	}
}

void CUIMgr::OnDestroy() {
	// 모든 캐싱된 UI 메모리 해제
	for (auto& it : mUIs) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}

	mUIs.clear();
}

void CUIMgr::OnUpdate(float tDeltaTime) {
	// 활성화된 UI들만 업데이트 수행
	for (CUIBase* ui : mActiveUIs) {
		if (ui) ui->OnUpdate(tDeltaTime);
	}

	// 마우스 상호작용 및 UI 추가/제거 요청 처리
	CheckMouseHover();
	ProcessPop();
	ProcessDelete();

	// UI 생성 요청 처리 (Pop/Delete 이후에 수행하여 안전성 확보)
	if (!mRequestUIQueue.empty()) {
		eUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI);
	}
}

void CUIMgr::OnLateUpdate(float tDeltaTime) {
	for (CUIBase* ui : mActiveUIs) {
		if (ui) ui->OnLateUpdate(tDeltaTime);
	}
}

void CUIMgr::Render(HDC tHDC) {
	for (CUIBase* ui : mActiveUIs) {
		if (ui) ui->Render(tHDC);
	}
}

void CUIMgr::OnComplete(CUIBase* tAddUI) {
	if (tAddUI == nullptr) {
		return;
	}

	tAddUI->Active();

	// Full Screen UI 로직 (예: 인벤토리, 일시정지 메뉴 등)
	// 화면 전체를 덮는 UI가 열리면, 뒤에 있는 UI들은 비활성화하여 입력을 막음
	if (tAddUI->IsFullScreen()) {
		for (CUIBase* ui : mActiveUIs) {
			ui->InActive();
		}
	}

	 // 화면의 가장 앞쪽(벡터의 맨 뒤)에 추가
	mActiveUIs.push_back(tAddUI);
}

CUIBase* CUIMgr::GetUI(eUIType tType)
{
	// 1. 이미 생성된 UI인지 확인 (캐싱)
	auto it = mUIs.find(tType);
	if (it != mUIs.end()) {
		return it->second;
	}

	// 2. 없다면 새로 생성 (Lazy Loading)
	CUIBase* newUI = CreateUI(tType);

	if (newUI != nullptr) {
		newUI->OnCreate();
		mUIs.insert({ tType, newUI });
	}

	return newUI;
}

// 팩토리 패턴: Enum 타입에 맞춰 알맞은 클래스 인스턴스 생성
CUIBase* CUIMgr::CreateUI(eUIType tType)
{
	switch (tType) {
	case eUIType::CharacterSelectUI:
		return new CCharacterSelectUI();
	case eUIType::WeaponSelectUI:
		return new CWeaponSelectUI();
	case eUIType::TitleSceneUI:
		return new CTitleSceneUI();
	case eUIType::PlaySceneUI:
		return new CPlaySceneUI();
	case eUIType::ShopUI:
		return new CShopUI();
	case eUIType::EndingSceneUI:
		return new CEndingUI();
	case eUIType::LevelUpUI:
		return new CLevelUpUI();
	}
	return nullptr;
}

void CUIMgr::ClearUI(eUIType tType)
{
	mDeleteQueue.push(tType);
}

void CUIMgr::ProcessPop()
{
	while (!mPopQueue.empty()) {
		eUIType type = mPopQueue.front();
		mPopQueue.pop();

		// 활성 리스트에서 해당 UI 찾기
		auto it = std::find_if(mActiveUIs.begin(), mActiveUIs.end(),
			[type](CUIBase* ui) { return ui->GetType() == type; });

		if (it != mActiveUIs.end()) {
			CUIBase* targetUI = *it;

			targetUI->InActive();

			// 만약 닫히는 UI가 전체화면(FullScreen)이었다면,
			// 숨겨져 있던 뒤쪽 UI들을 다시 활성화해줌
			if (targetUI->IsFullScreen()) {
				for (CUIBase* ui : mActiveUIs) {
					if (ui != targetUI) ui->Active();
				}
			}

			// 호버 중이던 UI가 사라지면 포인터 초기화 (Dangling Pointer 방지)
			if (mPrevHoverUI != nullptr) {
				mPrevHoverUI = nullptr;
			}

			mActiveUIs.erase(it);
		}
	}
}

// UI 완전 삭제 처리 (메모리 해제 포함)
void CUIMgr::ProcessDelete()
{
	while (!mDeleteQueue.empty()) {
		eUIType type = mDeleteQueue.front();
		mDeleteQueue.pop();

		// 1. 활성 리스트에 있다면 제거 (Pop 로직과 유사)
		auto itActive = std::find_if(mActiveUIs.begin(), mActiveUIs.end(),
			[type](CUIBase* ui) { return ui->GetType() == type; });

		if (itActive != mActiveUIs.end()) {
			Pop(type);
			CUIBase* targetUI = *itActive;

			targetUI->InActive();

			if (targetUI->IsFullScreen()) {
				for (CUIBase* ui : mActiveUIs) {
					if (ui != targetUI) ui->Active();
				}
			}

			mActiveUIs.erase(itActive);
		}

		// 2. 전체 관리 맵(mUIs)에서 제거 및 메모리 해제
		auto it = mUIs.find(type);
		if (it != mUIs.end()) {
			if (mPrevHoverUI != nullptr)
			{
				mPrevHoverUI = nullptr;
			}
			it->second->UIClear();	// UI 내부 리소스 정리
			SAFE_DELETE(it->second);
			mUIs.erase(it);
		}
	}
}

void CUIMgr::Push(eUIType tType) {
	mRequestUIQueue.push(tType);
}

void CUIMgr::Pop(eUIType tType) {
	mPopQueue.push(tType);
}

void CUIMgr::CheckMouseHover()
{
	SVector2D mousePos = CInputMgr::GetMousePosition();

	CUIBase* pTargetUI = nullptr;

	// 역순 순회 (Reverse Iterator)
	// 화면에 가장 늦게 그려진(가장 위에 있는) UI부터 검사해야 클릭/호버가 올바르게 작동함
	for (auto iter = mActiveUIs.rbegin(); iter != mActiveUIs.rend(); ++iter) {
		pTargetUI = (*iter)->FindTargetUI(mousePos);

		if (pTargetUI != nullptr) break;	// 상위 UI를 찾았으면 더 뒤쪽(아래쪽)은 검사 안 함
	}

	// 호버 대상이 변경되었을 때 이벤트 처리
	if (mPrevHoverUI != pTargetUI) {

		// 이전 UI: 마우스 나감(OutHover)
		if (mPrevHoverUI != nullptr) {
			mPrevHoverUI->SetHover(false);
			mPrevHoverUI->GetEventOutHover();
		}

		// 새 UI: 마우스 들어옴(Hover)
		if (pTargetUI != nullptr) {
			pTargetUI->SetHover(true);
			pTargetUI->GetEventHover();
		}

		mPrevHoverUI = pTargetUI;
	}
}

void CUIMgr::CurHoverNull()
{
	mPrevHoverUI = nullptr;
}
