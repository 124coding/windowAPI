#include "CEndingScene.h"

#include "Object.h"

#include "CUIMgr.h"

void CEndingScene::OnCreate()
{
	GameObject* bg = Instantiate<GameObject>(eLayerType::BackGround);
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(CResourceMgr::Find<CTexture>(L"SettingMap"));

	CScene::OnCreate();
}

void CEndingScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CEndingScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CEndingScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);
}

void CEndingScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);
}

void CEndingScene::OnEnter()
{
	CScene::OnEnter();
	CUIMgr::Push(eUIType::EndingSceneUI);
	CUIMgr::ClearUI(eUIType::PlaySceneUI);
}

void CEndingScene::OnExit()
{
	CUIMgr::ClearUI(eUIType::EndingSceneUI);
	CScene::OnExit();
}
