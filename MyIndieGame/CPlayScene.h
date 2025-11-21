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

	void OnCreate(CAPIEngine* tEngine) override;
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
	void LoadMap(CAPIEngine* tEngine, const wchar_t* tPath);

private:
	CPlayer* mPlayer;
	CWeaponMgr* mPlayerWeapons;
	int mStageNum = 0;
};