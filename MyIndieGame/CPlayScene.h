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

	CPlayer* GetPlayer() {
		return this->mPlayer;
	}

public:
	void LoadBakedMap(const wchar_t* tPath);
	void RandomBakedMap();
	void OutLineFill(Gdiplus::Graphics* tGraphics, int tTileW, int tTileH);

private:
	CPlayer* mPlayer;
	CWeaponMgr* mPlayerWeapons;

	GameObject* mBakedMap;

	int mStageNum = 0;
};