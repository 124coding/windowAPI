#pragma once

#include "CUIBase.h"

class CUIPanel;

class CWeaponSelectUI : public CUIBase
{
public:
	CWeaponSelectUI() : CUIBase(eUIType::WeaponSelectUI) {}
	~CWeaponSelectUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	CUIPanel* GetCharDescPanel() {
		return this->mCharDescriptionPanel;
	}

private:
	CUIPanel* mCharDescriptionPanel = nullptr;
};

