#pragma once

#include "CEffect.h"

class CDamageText : public CEffect
{
public:
	CDamageText() {}
	virtual ~CDamageText() {}

	CDamageText(const CDamageText& tDamageText);
	CDamageText& operator=(const CDamageText& tDamagetTExt);

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	void Reset(SVector2D tPos, int tDamage, Gdiplus::Color tColor);

private:
	std::wstring mText = L"";
	SVector2D mStartPos;

	Gdiplus::Color mColor;
	float mGravity = 800.0f;
	float mWaitTime = 1.0f;
};

