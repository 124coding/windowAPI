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
	// 레벨업 시 획득한 업그레이드 상황을 보여주는 패널 생성
	CUIPanel* MakeUpgradeCheckPanel();

private:
	CUIText* mMoneyTex = nullptr;		// 보유 재화 표시
	CUIText* mStageNumTex = nullptr;	// 현재 웨이브(스테이지) 번호
	CUIText* mTimeTex = nullptr;		// 남은 시간 타이머

	// ==========================================
	// Performance Monitoring (FPS Counter)
	// ==========================================
	CUIText* mFpsText = nullptr;
	float mLastTime = 0.0f;
	float mCurrentTime = 0.0f;
	int mFrameCount = 0;				// 1초 동안 그려진 프레임 수 누적
	float mTimeElapsed = 0.0f;			// 경과 시간 누적

	// ==========================================
	// Upgrade Notification UI
	// ==========================================
	int mCurCheckLevel = 0;				// 현재 UI에 표시된 레벨 상태

	std::vector<CUIPanel*> mUpgradeCheckPanels;
};