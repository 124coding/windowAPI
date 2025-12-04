#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CInputMgr.h"

#include "CCharacterSelectUI.h"
#include "CWeaponSelectUI.h"
#include "CTitleSceneUI.h"

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
	ProcessPop();
	ProcessDelete();

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

CUIBase* CUIMgr::GetUI(eUIType tType)
{
	auto it = mUIs.find(tType);
	if (it != mUIs.end()) {
		return it->second;
	}

	CUIBase* newUI = CreateUI(tType);

	if (newUI != nullptr) {
		newUI->OnCreate();
		mUIs.insert({ tType, newUI });
	}

	return newUI;
}

CUIBase* CUIMgr::CreateUI(eUIType tType)
{
	switch (tType) {
	case eUIType::CharacterSelectUI:
		return new CCharacterSelectUI;
	case eUIType::WeaponSelectUI:
		return new CWeaponSelectUI;
	case eUIType::TitleSceneUI:
		return new CTitleSceneUI;
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

		auto it = std::find_if(mActiveUIs.begin(), mActiveUIs.end(),
			[type](CUIBase* ui) { return ui->GetType() == type; });

		if (it != mActiveUIs.end()) {
			CUIBase* targetUI = *it;

			targetUI->InActive();

			if (targetUI->IsFullScreen()) {
				for (CUIBase* ui : mActiveUIs) {
					if (ui != targetUI) ui->Active();
				}
			}

			if (mPrevHoverUI != nullptr) {
				mPrevHoverUI = nullptr;
			}

			mActiveUIs.erase(it);
		}
	}
}

void CUIMgr::ProcessDelete()
{
	while (!mDeleteQueue.empty()) {
		eUIType type = mDeleteQueue.front();
		mDeleteQueue.pop();

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

		auto it = mUIs.find(type);
		if (it != mUIs.end()) {
			if (mPrevHoverUI != nullptr)
			{
				mPrevHoverUI = nullptr;
			}
			it->second->UIClear();
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
