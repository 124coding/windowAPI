#include "CShopScene.h"

#include "Object.h"

#include "CUIMgr.h"

void CShopScene::OnCreate()
{
	GameObject* bg = Instantiate<GameObject>(eLayerType::BackGround);
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(CResourceMgr::Find<CTexture>(L"SettingMap"));

	CScene::OnCreate();
}

void CShopScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CShopScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CShopScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CShopScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);
}

void CShopScene::OnEnter()
{
	CScene::OnEnter();
	CUIMgr::Push(eUIType::ShopUI);
}

void CShopScene::OnExit()
{
	CScene::OnExit();
	CUIMgr::ClearUI(eUIType::ShopUI);
}
