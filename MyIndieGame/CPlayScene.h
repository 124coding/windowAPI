#pragma once
#include "CScene.h"

class CPlayer;

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

private:
	CPlayer* mPlayer;
};

