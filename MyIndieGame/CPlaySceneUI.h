#pragma once
#pragma once

#include "CUIBase.h"

class CUIText;

class CPlaySceneUI : public CUIBase
{
public:
	CPlaySceneUI() : CUIBase(eUIType::PlaySceneUI) {}
	~CPlaySceneUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

private:
	CUIText* mMoneyTex = nullptr;
	CUIText* mStageNumTex = nullptr;
	CUIText* mTimeTex = nullptr;
};