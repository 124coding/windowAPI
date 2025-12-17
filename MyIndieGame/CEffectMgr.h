#pragma once

#include "Object.h"

#include "CEffect.h"

#include <unordered_map>

class CEffectText;

class CEffectMgr
{
public:
	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void PlayEffect(std::wstring tParticleName, SVector2D tPos, GameObject* tObj = nullptr);
	static void ShowEffectText(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor);

private:
	static std::unordered_map<std::wstring, std::vector<CEffect*>> mEffects;
	static std::vector<CEffectText*> mEffectTexts;
};

