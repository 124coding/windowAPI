#pragma once

#include "CUIBase.h"

class CUIButton : public CUIBase
{
public:
	CUIButton() : CUIBase(eUIType::Button) {}
	virtual ~CUIButton() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	virtual void ButtonClick();

private:
	CTexture* mTexture;
	SEvent mOnClick;
};