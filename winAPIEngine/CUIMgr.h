#pragma once

#include "CUIBase.h"
#include "CAPIEngine.h"

#include "Enums.h"

#include <stack>
#include <queue>
#include <unordered_map>

class CUIMgr
{
public:
	static void OnCreate();
	static void OnLoad(eUIType tType);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);
	static void OnComplete(CUIBase* tAddUI);

	static CUIBase* GetUI(eUIType tType);
	static CUIBase* CreateUI(eUIType tType);

	static void ClearUI(eUIType tType);
	static void ProcessPop();
	static void ProcessDelete();

	static void Push(eUIType tType);
	static void Pop(eUIType tType);

	static void CheckMouseHover();

	static CUIBase* FindUI(eUIType tType) {
		return mUIs.find(tType)->second;
	}

private:
	static std::unordered_map<eUIType, CUIBase*> mUIs;
	static std::vector<CUIBase*> mActiveUIs;
	static std::queue<eUIType> mRequestUIQueue;
	static std::queue<eUIType> mPopQueue;
	static std::queue<eUIType> mDeleteQueue;

	static CUIBase* mPrevHoverUI;
};

