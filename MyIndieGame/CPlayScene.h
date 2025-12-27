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

	void StagePass();
	void GameEnd();

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
	void LoadBakedMap(const wchar_t* tPath);
	void RandomBakedMap();
	void OutLineFill(Gdiplus::Graphics* tGraphics, int tTileW, int tTileH);

private:
	static CPlayer* mPlayer;
	static int mStageNum;

	GameObject* mBakedMap;

	float mWaitTime = 3.0f;
};