#pragma once

#include "CUIHUD.h"

class CTexture;
class CUIText;

class CUIEXPBar : public CUIHUD
{
public:
	CUIEXPBar(SVector2D tPos, float tWidth, float tHeight) : CUIHUD(tPos, tWidth, tHeight) {}
	virtual ~CUIEXPBar() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	void SetPlayer(GameObject* tPlayer) {
		this->mPlayer = tPlayer;
	}

private:
	GameObject* mPlayer;
	CTexture* mExpFill;
	CUIText* mLevelInfo;
};