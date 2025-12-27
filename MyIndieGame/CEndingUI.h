#pragma once

#include "CUIBase.h"

class CWeapon;
class CUIButton;
class CUIPanel;

class CEndingUI : public CUIBase
{
public:
	CEndingUI(SVector2D tPos = SVector2D(), float tWidth = 0.0f, float tHeight = 0.0f) : CUIBase(tPos, tWidth, tHeight, eUIType::EndingSceneUI) {}
	virtual ~CEndingUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

private:
	CUIButton* MakeWeaponButton(std::vector<CWeapon*>* tWeapons, CWeapon* tWeapon, float tX, float tY, bool tRight);
	CUIPanel* MakeItemPanel(std::wstring tItemID, float tX, float tY, bool tRight, bool tUp);
};

