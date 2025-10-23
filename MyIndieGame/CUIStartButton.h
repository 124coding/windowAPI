#pragma once

#include "CUIButton.h"

class CUIStartButton : public CUIButton
{
public:
	CUIStartButton() {}
	virtual ~CUIStartButton() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	virtual void ButtonClick() override;
};