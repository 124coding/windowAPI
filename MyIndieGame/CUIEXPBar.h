#pragma once

#include "CUIHUD.h"

class CPlayer;

class CUIEXPBar : public CUIHUD
{
public:
	CUIEXPBar() {}
	virtual ~CUIEXPBar() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	void SetPlayer(CPlayer* tPlayer) {
		this->mPlayer = tPlayer;
	}

private:
	CPlayer* mPlayer;
};