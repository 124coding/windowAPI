#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CInputMgr.h"

#include "CCharacterSelectUI.h"

#include "winMacro.h"

std::unordered_map<eUIType, CUIBase*> CUIMgr::mUIs = {};
std::vector<CUIBase*> CUIMgr::mActiveUIs = {};
std::queue<eUIType> CUIMgr::mRequestUIQueue = {};
CUIBase* CUIMgr::mPrevHoverUI = nullptr;

void CUIMgr::OnCreate() {
	CCharacterSelectUI* chSelectUI = new CCharacterSelectUI();
	mUIs.insert(std::make_pair(eUIType::CharacterSelectUI, chSelectUI));
	chSelectUI->SetWidth(windowWidth);
	chSelectUI->SetHeight(windowHeight);

	chSelectUI->OnCreate();
}

void CUIMgr::OnLoad(eUIType tType) {
	auto it = mUIs.find(tType);

	if (it == mUIs.end()) {
		return;
	}

	OnComplete(it->second);
}

void CUIMgr::OnDestroy() {
	for (auto& it : mUIs) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}

	mUIs.clear();
}

void CUIMgr::OnUpdate(float tDeltaTime) {
	for (CUIBase* ui : mActiveUIs) {
		if (ui) ui->OnUpdate(tDeltaTime);
	}

	CheckMouseHover();

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

	// ui가 전체 화면이라면 해당 ui말고는 전부 비활성화
	 if (tAddUI->IsFullScreen()) {
		 for (CUIBase* ui : mActiveUIs) {
			 ui->InActive();
		 }
	 }

	mActiveUIs.push_back(tAddUI);
}

void CUIMgr::Push(eUIType tType) {
	mRequestUIQueue.push(tType);
}

void CUIMgr::Pop(eUIType tType) {
	auto it = std::find_if(mActiveUIs.begin(), mActiveUIs.end(),
		[tType](CUIBase* ui) { return ui->GetType() == tType; });

	if (it != mActiveUIs.end()) {
		CUIBase* targetUI = *it;

		targetUI->UIClear();
		targetUI->InActive();

		if (targetUI->IsFullScreen()) {
			for (CUIBase* ui : mActiveUIs) {
				if (ui != targetUI) ui->Active();
			}
		}

		if (mPrevHoverUI == targetUI) {
			mPrevHoverUI = nullptr;
		}

		mActiveUIs.erase(it);
	}
}

void CUIMgr::CheckMouseHover()
{
	SVector2D mousePos = CInputMgr::GetMousePosition();

	CUIBase* pTargetUI = nullptr;

	// 가장 늦게 들어온 UI부터 (역순)
	for (auto iter = mActiveUIs.rbegin(); iter != mActiveUIs.rend(); ++iter) {
		pTargetUI = (*iter)->FindTargetUI(mousePos);

		if (pTargetUI != nullptr) break;
	}

	if (mPrevHoverUI != pTargetUI) {
		if (mPrevHoverUI != nullptr) {
			mPrevHoverUI->SetHover(false);
			mPrevHoverUI->GetEventOutHover();
		}

		if (pTargetUI != nullptr) {
			pTargetUI->SetHover(true);
			pTargetUI->GetEventHover();
		}

		mPrevHoverUI = pTargetUI;
	}
}
