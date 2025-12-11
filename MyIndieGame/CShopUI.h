#pragma once

#include "CUIBase.h"

class CUIPanel;
class CUIText;
class CUIButton;
class CUIImg;

class CShopUI : public CUIBase
{
public:
	CShopUI() : CUIBase(eUIType::TitleSceneUI) {}
	virtual ~CShopUI() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

private:
	CUIPanel* MakeGoods(float tWidth, float tHeight);

private:
	CUIPanel* mShopMainPanel = nullptr;

	CUIText* mMoneyTex = nullptr;

	// 초기화 버튼
	CUIButton* mResetButton = nullptr;
	CUIText* mResetTex = nullptr;
	CUIImg* mResetImg = nullptr;
	int mResetCost = 1;

	// 상품 목록
	std::vector<CUIPanel*> mGoods = std::vector<CUIPanel*>(4, nullptr);

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