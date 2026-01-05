#pragma once

#include "CUIBase.h"

class CUIText;
class CUIPanel;

class CPlaySceneUI : public CUIBase
{
public:
	CPlaySceneUI(SVector2D tPos = SVector2D(), float tWidth = 0.0f, float tHeight = 0.0f) : CUIBase(tPos, tWidth, tHeight, eUIType::PlaySceneUI) {}
	~CPlaySceneUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

public:
	CUIPanel* MakeUpgradeCheckPanel();

private:
	CUIText* mMoneyTex = nullptr;
	CUIText* mStageNumTex = nullptr;
	CUIText* mTimeTex = nullptr;
	CUIText* mFpsText = nullptr;
	float mLastTime = 0.0f;
	float mCurrentTime = 0.0f;
	int mFrameCount = 0;
	float mTimeElapsed = 0.0f;

	int mCurCheckLevel = 0;

	std::vector<CUIPanel*> mUpgradeCheckPanels;
};