#pragma once

#include "CScene.h"
#include "CPlayer.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"

class CEndingScene : public CScene
{
public:
	CEndingScene() {}
	virtual ~CEndingScene() {}

	void OnCreate(CAPIEngine* tEngine) override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;
};

