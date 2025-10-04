#pragma once

#include "GameEngine.h"
#include "GameObject.h"
#include "CInputMgr.h"
#include "SVector2D.h"
#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"

void GameEngine::OnCreate() {
	LoadResources();
	LoadScenes();
	CInputMgr::GetInst()->AddKeyInfo("ChangeScene", 'N');

	CSceneMgr::OnCreate(this);
}

void GameEngine::OnDestroy() {

	CSceneMgr::OnDestroy();
}

void GameEngine::OnUpdate(float tDeltaTime) {
	Render();

	CSceneMgr::OnUpdate(tDeltaTime);
}

void GameEngine::OnLateUpdate(float tDeltaTime) {
	CSceneMgr::OnLateUpdate(tDeltaTime);
}

void GameEngine::Render() {
	this->Clear(0.5f, 0.5f, 0.5f);
	CSceneMgr::Render(mBackBuffer->GetDCMem());
	this->Present();
}

void GameEngine::LoadScenes()
{
	CSceneMgr::CreateScene<CTitleScene>(this, L"TitleScene");
	CSceneMgr::CreateScene<CPlayScene>(this, L"PlayScene");
	CSceneMgr::CreateScene<CEndingScene>(this, L"EndingScene");

	CSceneMgr::LoadScene(L"PlayScene");
}

void GameEngine::LoadResources()
{
	CResourceMgr::Load<CTexture>(this, L"MAP", L"../resources/blue_sky.png");
	CResourceMgr::Load<CTexture>(this, L"Chicken", L"../resources/Sprites/ChickenAlpha.bmp");
}
