#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"

class CPlayScene : public CScene
{
public:
	CPlayScene() {}
	virtual ~CPlayScene() {}

	void OnCreate(CAPIEngine* tEngine) override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;

	void OnEnter() override;
	void OnExit() override;
};

