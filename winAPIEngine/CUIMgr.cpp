#include "CUIMgr.h"

#include "CSceneMgr.h"

#include "CUIButton.h"
#include "CUIStartButton.h"
#include "CUIHUD.h"
#include "CUIHPBar.h"
#include "CUIEXPBar.h"

#include "winMacro.h"

std::unordered_map<eUIType, CUIBase*> CUIMgr::mUIs = {};
std::stack<CUIBase*> CUIMgr::mUIBases = {};
std::queue<eUIType> CUIMgr::mRequestUIQueue = {};
CUIBase* CUIMgr::mActiveUI = nullptr;

void CUIMgr::OnCreate(CAPIEngine* tEngine) {
	CUIStartButton* button = new CUIStartButton();
	mUIs.insert(std::make_pair(eUIType::StartButton, button));

	CUIHPBar* HPBar = new CUIHPBar();
	mUIs.insert(std::make_pair(eUIType::HPBar, HPBar));

	CUIEXPBar* EXPBar = new CUIEXPBar();
	mUIs.insert(std::make_pair(eUIType::EXPBar, EXPBar));
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
	for (auto& it : mUIs) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}

	mUIs.clear();
}

void CUIMgr::OnUpdate(float tDeltaTime) {
	std::stack<CUIBase*> uiBases = mUIBases;

	while (!uiBases.empty()) {
		CUIBase* uiBase = uiBases.top();

		if (uiBase) {
			uiBase->OnUpdate(tDeltaTime);
			uiBases.pop();
		}
	}

	if (mRequestUIQueue.size() > 0) {
		eUIType requestUI = mRequestUIQueue.front();
		mRequestUIQueue.pop();
		OnLoad(requestUI, tDeltaTime);
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

void CUIMgr::Pop(eUIType tType) {
	if (mUIBases.size() <= 0)
		return;

	std::stack<CUIBase*> tempStack;

	CUIBase* uiBase = nullptr;
	
	while (mUIBases.size() > 0) {
		uiBase = mUIBases.top();
		mUIBases.pop();

		if (uiBase->GetType() != tType) {
			tempStack.push(uiBase);
			continue;
		}

		if (uiBase->IsFullScreen()) {
			std::stack<CUIBase*> uiBases = mUIBases;

			while (!uiBases.empty()) {
				CUIBase* ui = uiBases.top();
				uiBases.pop();

				if (ui) {
					ui->Active();
					break;
				}
			}
		}

		uiBase->UIClear();
	}

	while (tempStack.size() > 0) {
		uiBase = tempStack.top();
		tempStack.pop();
		mUIBases.push(uiBase);
	}
}
