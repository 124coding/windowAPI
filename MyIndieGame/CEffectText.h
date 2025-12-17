#pragma once

#include "CEffect.h"

class CEffectText : public CEffect
{
public:
	CEffectText() {}
	virtual ~CEffectText() {}

	CEffectText(const CEffectText& tEffectText);
	CEffectText& operator=(const CEffectText& tEffectText);

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	virtual void Reset(SVector2D tPos) override;
	void Reset(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor);

private:
	std::wstring mText = L"";

	Gdiplus::Color mColor;
	float mGravity = 800.0f;
	float mWaitTime = 1.0f;
};

