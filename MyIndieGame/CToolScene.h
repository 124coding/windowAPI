#pragma once

#include "CScene.h"

class CToolScene : public CScene
{
public:
	CToolScene() {}
	virtual ~CToolScene() {}

	void OnCreate(CAPIEngine* tEngine) override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

private:
};

