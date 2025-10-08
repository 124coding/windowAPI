#include "CInputMgr.h"
#include "winMacro.h"

CInputMgr* CInputMgr::mInst = nullptr;

CInputMgr::~CInputMgr()
{
	for (auto it = mMapKeyInfos.begin(); it != mMapKeyInfos.end(); it++) {
		SAFE_DELETE(it->second);
	}

	mMapKeyInfos.clear();
}

void CInputMgr::Update()
{
	for (auto it = mMapKeyInfos.begin(); it != mMapKeyInfos.end(); it++) {
		int tPushCount = 0;
		if (GetAsyncKeyState(it->second->mKeyInput) & 0x8000)
		{
			tPushCount++;
		}

		if (tPushCount == 1) {
			if (it->second->mState == eKeyState::None) {
				it->second->mState = eKeyState::Down;
			}
			else if (it->second->mState == eKeyState::Down && !it->second->bPressed) {
				it->second->mState = eKeyState::Pressed;
				it->second->bPressed = true;
			}
		}
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

bool CInputMgr::GetKeyDown(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (tInfo->mState == eKeyState::Down) return true;
	return false;
}

bool CInputMgr::GetKeyPressed(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (tInfo->mState == eKeyState::Pressed) return true;
	return false;
}

bool CInputMgr::GetKeyUp(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (tInfo->mState == eKeyState::Up) return true;
	return false;
}

bool CInputMgr::GetKeyNone(const std::string& tStrKey)
{
	SKeyInfo* tInfo = FindKeyInfo(tStrKey);

	if (tInfo == nullptr) {
		return false;
	}

	if (tInfo->mState == eKeyState::None) return true;
	return false;
}