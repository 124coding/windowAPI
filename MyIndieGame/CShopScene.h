#pragma once

#include "CScene.h"

class CShopScene : public CScene
{
public:
	friend class CCharacterSelectUI;

	CShopScene() {}
	virtual ~CShopScene() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

private:
};

