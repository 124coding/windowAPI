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
	// ==========================================
	// Dynamic Reward Generation (동적 보상 생성)
	// ==========================================

	// 선택된 업그레이드 정보를 바탕으로 UI 패널(버튼, 설명, 아이콘)을 생성
	// Factory Pattern의 변형으로, 데이터(ID)를 주면 UI 객체를 반환함
	std::pair<std::wstring, CUIPanel*> MakeStatUpPanel(float tX, float tY, int tStageNum);

	// [Core Logic] 현재 스테이지에 맞춰 가중치 확률(Weighted Random)로 업그레이드 등급과 종류를 결정
	// 반환값: <티어(등급), 업그레이드 데이터 구조체>
	std::pair<int, CDataMgr::SUpgrades> ChooseRandomUpgradeByStageNum(int tStageNum);

	// [Reroll Logic] 현재 표시된 선택지를 모두 삭제하고 새로 생성
	// 리롤 버튼 클릭 시 호출되며, 비용 지불 후 확률 로직을 다시 돌림
	void ResetLevelUpPanels(int tStageNum);

private:
	CUIPanel* mLevelUpMainPanel = nullptr;

	// 현재 화면에 표시된 랜덤 선택지(보상 패널) 목록 관리
	// 선택 후 비활성화하거나 리롤 시 메모리 해제를 위해 벡터로 저장
	std::vector<std::pair<std::wstring, CUIPanel*>> mStatUpPanels;

	CUIPanel* mStatPanel = nullptr;		// 현재 플레이어 스탯 요약창

	// 초기화 버튼
	CUIButton* mResetButton = nullptr;
	CUIText* mResetTex = nullptr;
	CUIImg* mResetImg = nullptr;
	int mResetCost = 1;
	int mResetCount = 0;

	// ==========================================
	// Stats Dashboard (Current Player Status)
	// ==========================================
	// 레벨업 선택 시 참고할 수 있도록 현재 플레이어의 스탯을 실시간 표시
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