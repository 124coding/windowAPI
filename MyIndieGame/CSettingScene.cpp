#include "CSettingScene.h"

#include "Object.h"

#include "CUIMgr.h"

std::vector<CSettingScene::SEffect> CSettingScene::mApplicableEffects;

void CSettingScene::OnCreate()
{
	GameObject* bg = Instantiate<GameObject>(eLayerType::BackGround);
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(CResourceMgr::Find<CTexture>(L"SettingMap"));

	CScene::OnCreate();
}

void CSettingScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CSettingScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CSettingScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CSettingScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);
}

void CSettingScene::OnEnter()
{
	CScene::OnEnter();

	CUIMgr::Push(eUIType::CharacterSelectUI);
	mCurSelect = eSelect::Character;
}

void CSettingScene::OnExit()
{
	CScene::OnExit();

	CUIMgr::Pop(eUIType::CharacterSelectUI);
}
