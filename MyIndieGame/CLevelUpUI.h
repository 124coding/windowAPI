#pragma once

#include "CUIBase.h"

#include "CDataMgr.h"

class CUIPanel;
class CUIText;
class CUIButton;
class CUIImg;

class CLevelUpUI : public CUIBase
{
public:
	CLevelUpUI(SVector2D tPos = SVector2D(), float tWidth = 0.0f, float tHeight = 0.0f) : CUIBase(tPos, tWidth, tHeight, eUIType::LevelUpUI) {}
	~CLevelUpUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

public:
	std::pair<std::wstring, CUIPanel*> MakeStatUpPanel(float tX, float tY, int tStageNum);
	std::pair<int, CDataMgr::SUpgrades> ChooseRandomUpgradeByStageNum(int tStageNum);
	void ResetLevelUpPanels(int tStageNum);

private:
	CUIPanel* mLevelUpMainPanel = nullptr;
	std::vector<std::pair<std::wstring, CUIPanel*>> mStatUpPanels;

	CUIPanel* mStatPanel = nullptr;

	// 초기화 버튼
	CUIButton* mResetButton = nullptr;
	CUIText* mResetTex = nullptr;
	CUIImg* mResetImg = nullptr;
	int mResetCost = 1;
	int mResetCount = 0;

	// 능력치
	CUIText* mLevel = nullptr;
	CUIText* mMaxHP = nullptr;
	CUIText* mHPRegen = nullptr;
	CUIText* mHPSteal = nullptr;
	CUIText* mDamage = nullptr;
	CUIText* mMeleeDamage = nullptr;
	CUIText* mRangedDamage = nullptr;
	CUIText* mAttackSpeed = nullptr;
	CUIText* mCritChance = nullptr;
	CUIText* mRange = nullptr;
	CUIText* mArmor = nullptr;
	CUIText* mDodge = nullptr;
	CUIText* mSpeed = nullptr;
};