#pragma once

#include "Object.h"

#include "CEffect.h"

#include <unordered_map>

class CDamageText;

class CEffectMgr
{
public:
	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void PlayEffect(std::wstring tParticleName, SVector2D tPos);
	static void ShowDamageText(SVector2D tPos, int tDamage, Gdiplus::Color tColor);

private:
	static std::unordered_map<std::wstring, std::vector<CEffect*>> mEffects;
	static std::vector<CDamageText*> mDamageTexts;
};

