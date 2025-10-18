#include "CUIMgr.h"

std::unordered_map<eUIType, CUIBase*> CUIMgr::mUIs = {};
std::stack<CUIBase*> CUIMgr::mUIBases = {};
std::queue<eUIType> CUIMgr::mRequestUIQueue = {};
CUIBase* CUIMgr::mActiveUI = nullptr;

void CUIMgr::OnCreate(CAPIEngine* tEngine) {

}

void CUIMgr::OnLoad(eUIType tType, float tDeltaTime) {
	auto it = mUIs.find(tType);

	if (it == mUIs.end()) {
		OnFail();
		return;
	}

	OnComplete(it->second, tDeltaTime);
}

void CUIMgr::OnDestroy() {

}

void CUIMgr::OnUpdate(float tDeltaTime) {
	std::stack<CUIBase*> uiBases = mUIBases;

	while (!uiBases.empty()) {
		CUIBase* uiBase = uiBases.top();

		if (uiBase) {
			uiBase->OnUpdate(tDeltaTime);
			uiBases.pop();
		}

		if (mRequestUIQueue.size() > 0) {
			eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();
			OnLoad(requestUI, tDeltaTime);
		}
	}
}

void CUIMgr::OnLateUpdate(float tDeltaTime) {
	std::stack<CUIBase*> uiBases = mUIBases;

	while (!uiBases.empty()) {
		CUIBase* uiBase = uiBases.top();

		if (uiBase) {
			uiBase->OnLateUpdate(tDeltaTime);
			uiBases.pop();
		}

		if (mRequestUIQueue.size() > 0) {
			eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();
			OnLoad(requestUI);
		}
	}
}

void CUIMgr::Render(HDC tHDC) {
	std::stack<CUIBase*> uiBases = mUIBases;

	while (!uiBases.empty()) {
		CUIBase* uiBase = uiBases.top();

		if (uiBase) {
			uiBase->Render(tHDC);
			uiBases.pop();
		}

		if (mRequestUIQueue.size() > 0) {
			eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();
			OnLoad(requestUI);
		}
	}
}

void CUIMgr::OnComplete(CUIBase* tAddUI, float tDeltaTime) {
	if (tAddUI == nullptr) {
		return;
	}

	 tAddUI->OnCreate();
	 tAddUI->Active();
	 tAddUI->OnUpdate(tDeltaTime);

	// ui가 전체 화면이라면 해당 ui말고는 전부 비활성화
	if (tAddUI->IsFullScreen()) {
		std::stack<CUIBase*> uiBases = mUIBases;

		while (!uiBases.empty()) {
			CUIBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase) {
				uiBase->InActive();
			}
		}
	}

	mUIBases.push(tAddUI);
	mActiveUI = nullptr;
}

void CUIMgr::OnFail() {
	mActiveUI = nullptr;
}

void CUIMgr::Push(eUIType tType) {
	mRequestUIQueue.push(tType);
}

void CUIMgr::Pop() {
	if (mUIBases.size() <= 0)
		return;

	CUIBase* uiBase = nullptr;
	
	while (mUIBases.size() > 0) {
		uiBase = mUIBases.top();
		mUIBases.pop();
	}
}
