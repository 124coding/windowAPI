#pragma once

#include "CUIBase.h"

class CUIPanel;
class CUIText;
class CUIButton;
class CUIImg;

class CPlayer;

class CWeapon;
class CWeaponScript;

class CShopUI : public CUIBase
{
public:
	CShopUI(SVector2D tPos = SVector2D(), float tWidth = 0.0f, float tHeight = 0.0f) : CUIBase(tPos, tWidth, tHeight, eUIType::TitleSceneUI) {}
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
	std::pair<CUIPanel*, bool> MakeGoods(int tIdx, float tWidth, float tHeight);
	CUIPanel* MakeItemPanel(std::wstring tItemID, float tX, float tY);
	void ReSettingWeaponButton(CWeaponScript* tWpScript, CWeapon* tCurWp, CUIPanel* tParPanel, CUIButton* tWpButton, CUIText* tDescTex, CUIPanel* tWpImgPanel, CUIButton* tRecycleButton, CUIText* tRecycleTex);
	void WeaponButtonsReSetting(float tX, float tY, float tOffset, float tMax);


private:
	CUIPanel* mShopMainPanel = nullptr;

	CUIText* mMoneyTex = nullptr;

	// 초기화 버튼
	CUIButton* mResetButton = nullptr;
	CUIText* mResetTex = nullptr;
	CUIImg* mResetImg = nullptr;
	int mResetCost = 1;
	int mResetCount = 0;

	// 상품 목록
	std::vector<std::pair<CUIPanel*, bool>> mGoods = std::vector<std::pair<CUIPanel*, bool>>(4);

	std::vector<CUIPanel*> mItems;
	
	// 무기 버튼 모음(수정이 필요한 부분들만 가져오는 것)
	std::vector<CUIButton*> mWeapons;
	std::vector<CUIPanel*> mWeaponsDescPanel;
	std::vector<CUIButton*> mCombinationButtons;

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