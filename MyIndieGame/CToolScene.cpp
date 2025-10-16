#include "CToolScene.h"
#include "CTile.h"
#include "CResourceMgr.h"
#include "CTexture.h"

#include "CCamera.h"
#include "CTilemapRenderer.h"

#include "CRenderer.h"
#include "Object.h"

void CToolScene::OnCreate(CAPIEngine* tEngine)
{
	GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(800, 400));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;

	CTile* tile = Instantiate<CTile>(eLayerType::Tile);
	CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
	tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));

	CScene::OnCreate(tEngine);
}

void CToolScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CToolScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CToolScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);
}

void CToolScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

	for (size_t i = 0; i < 200; i++) {
		MoveToEx(tHDC, 16 * i, 0, NULL);
		LineTo(tHDC, 16 * i, 1000);
	}

	for (size_t i = 0; i < 200; i++) {
		MoveToEx(tHDC, 0, 16 * i, NULL);
		LineTo(tHDC, 20006, 16 * i);
	}
}

void CToolScene::OnEnter()
{
	CScene::OnEnter();
}

void CToolScene::OnExit()
{
	CScene::OnExit();
}
