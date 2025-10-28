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
	static void OnCreate(CAPIEngine* tEngine);
	static void OnLoad(eUIType tType, float tDeltaTime);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);
	static void OnComplete(CUIBase* tAddUI, float tDeltaTime);
	static void OnFail();

	static void Push(eUIType tType);
	static void Pop(eUIType tType);

	static CUIBase* FindUI(eUIType tType) {
		return mUIs.find(tType)->second;
	}

private:
	static std::unordered_map<eUIType, CUIBase*> mUIs;
	static std::stack<CUIBase*> mUIBases;
	static std::queue<eUIType> mRequestUIQueue;
	static CUIBase* mActiveUI;
};

