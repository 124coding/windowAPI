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
#include "time.h"

void GameEngine::OnCreate() {
	int a = 0;
	srand((unsigned int)(&a));
	LoadResources();
	LoadScenes();

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
	CResourceMgr::Load<CTexture>(this, L"Cat", L"../resources/Sprites/ChickenAlpha.bmp");
	CResourceMgr::Load<CTexture>(this, L"Player", L"../resources/Sprites/Player.bmp");
}
