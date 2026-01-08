#pragma once
#include "CScene.h"

#include <vector>

class CPlayer;
class CWeaponMgr;

class CPlayScene : public CScene
{
public:
	CPlayScene() {}
	virtual ~CPlayScene() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

	// 웨이브/스테이지 클리어 시 호출 (상점 이동 등의 로직 수행)
	void StagePass();

	// 플레이어 사망 시 호출 (결과 화면 이동 로직 수행)
	void GameEnd();


	// ==========================================
	// Global Access Point (Service Locator)
	// ==========================================
	// 몬스터 AI, UI, 투사체 등이 플레이어 정보를 쉽게 얻을 수 있도록 Static 접근자 제공
	// 복잡한 포인터 전달 과정을 줄여주는 편의성 위주의 설계
	static CPlayer* GetPlayer() {
		return mPlayer;
	}

	static int GetStageNum() {
		return mStageNum;
	}

	static void ResetStageNum() {
		mStageNum = 0;
	}

public:
	// 툴(ToolScene)에서 제작하여 저장된 타일맵 파일을 로드
	void LoadBakedMap(const wchar_t* tPath);

	// 로그라이크 특성: 준비된 여러 맵 중 하나를 랜덤하게 선택하여 로드
	void RandomBakedMap();

	// 타일맵의 외곽선이나 배경을 채우는 그래픽 처리 함수
	void OutLineFill(Gdiplus::Graphics* tGraphics, int tTileW, int tTileH);

private:
	// 전역 접근을 위해 static으로 선언된 주요 게임 객체들
	static CPlayer* mPlayer;
	static int mStageNum;

	// 레벨업 UI가 활성화되었을 때 게임 로직(Update)을 멈추기 위한 플래그
	bool mStatUpgrading = false;

	GameObject* mBakedMap;		// 현재 로드된 맵 객체

	float mWaitTime = 2.0f;		// 게임 종료/클리어 후 씬 전환 전 대기 시간 (연출용)
};