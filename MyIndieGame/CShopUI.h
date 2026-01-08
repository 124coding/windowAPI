#pragma once

#include "CUIBase.h"

class CPlayScene;
class CPlayerScript;

class CPlayer;

class CItemMgr;
class CWeaponMgr;

class CUIPanel;
class CUIText;
class CUIButton;
class CUIImg;

class CWeapon;
class CWeaponScript;

class CShopUI : public CUIBase
{
public:
	CShopUI(SVector2D tPos = SVector2D(), float tWidth = 0.0f, float tHeight = 0.0f) : CUIBase(tPos, tWidth, tHeight, eUIType::ShopUI) {}
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
	// ==========================================
	// Dynamic UI Generators (동적 UI 생성)
	// ==========================================

	// 상점 슬롯에 진열될 상품(아이템/무기) UI 생성
	std::pair<CUIPanel*, bool> MakeGoods(int tIdx, float tWidth, float tHeight, int tStageNum);

	// 획득한 아이템 목록 표시용 패널 생성
	CUIPanel* MakeItemPanel(std::wstring tItemID, float tX, float tY);

	// 인벤토리 무기 버튼 생성 (합성/판매 기능 포함)
	void ReSettingWeaponButton(CWeaponScript* tWpScript, CWeapon* tCurWp, CUIPanel* tParPanel, CUIButton* tWpButton, CUIText* tDescTex, CUIPanel* tWpImgPanel, CUIButton* tRecycleButton, CUIText* tRecycleTex);
	
	// ==========================================
	// UI Layout & Update (레이아웃 갱신)
	// ==========================================

	// 무기 구매/판매/합성 시 UI 버튼들의 정보를 갱신 (등급 색상, 데미지 표기 등)
	void WeaponButtonsReSetting(float tX, float tY, float tOffset, float tMax);
	
	// 무기 목록 변경 시 위치 재정렬 (Grid Layout Refresh)
	// 인벤토리에 무기가 추가되거나 삭제될 때 구멍 난 곳을 메꾸고 정렬함
	CUIButton* MakeWeaponButton(std::vector<CWeapon*>* tWeapons, CWeapon* tWeapon, float tX, float tY);

	// 무기 판매(Recycle) 또는 합성 시 해당 UI 요소 제거 및 메모리 해제
	void WeaponButtonRemove(int tIndex);

private:
	// ==========================================
	// Data References (데이터 참조)
	// ==========================================
	// UI는 데이터를 소유하지 않고, 플레이어의 매니저들을 참조하여 정보를 표시함
	CPlayer* mPl = nullptr;
	CPlayerScript* mPlSc = nullptr;
	CItemMgr* mPlItemMgr = nullptr;
	CWeaponMgr* mPlWeaponMgr = nullptr;
	std::vector<std::pair<std::wstring, int>>* mPlItems = nullptr;
	std::vector<CWeapon*>* mPlWeapons = nullptr;

	// ==========================================
	// UI Elements
	// ==========================================
	CUIPanel* mShopMainPanel = nullptr;
	CUIPanel* mHaveItemPanel = nullptr;
	CUIText* mHaveItemText = nullptr;

	CUIText* mMoneyTex = nullptr;

	// [Reroll System] 상점 새로고침 기능
	CUIButton* mResetButton = nullptr;
	CUIText* mResetTex = nullptr;
	CUIImg* mResetImg = nullptr;
	int mResetCost = 1;		// 리롤 횟수에 따라 증가하는 비용 관리
	int mResetCount = 0;

	// [Shop Slots] 상점 상품 목록 (4칸)
	// Pair: UI 패널 포인터, Lock 여부 (Lock된 상품은 리롤 시 유지됨)
	std::vector<std::pair<CUIPanel*, bool>> mGoods = std::vector<std::pair<CUIPanel*, bool>>(4);

	// 보유 아이템 UI 리스트
	std::vector<std::pair<CUIPanel*, CUIText*>> mItems;
	
	// [Inventory UI Management]
	// 무기 상태 변화(합성, 판매)에 따라 실시간으로 제어해야 할 UI 요소들을 벡터로 관리
	std::vector<CUIButton*> mWeapons;				// 무기 슬롯 버튼
	std::vector<CUIPanel*> mWeaponsDescPanel;		// 무기 정보 툴팁
	std::vector<CUIText*> mWeaponsDesc;
	std::vector<CUIPanel*> mWeaponsImgPanel;
	std::vector<CUIButton*> mRecycleButtons;		// 판매 버튼
	std::vector<CUIText*> mRecycleTexts;
	std::vector<CUIButton*> mCombinationButtons;	// 합성 버튼 (조건 충족 시 활성화)

	// 웨폰 버튼 리세팅 시 필요한 값을 가진 변수들
	CUIPanel* mHaveWeaponPanel = nullptr;
	CUIText* mHaveWeaponText = nullptr;

	// [Stats Dashboard] 플레이어 능력치 표기용 텍스트
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