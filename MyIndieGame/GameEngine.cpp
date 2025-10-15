#pragma once

#include "GameEngine.h"
#include "GameObject.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"

#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"

#include "SVector2D.h"
#include "time.h"

void GameEngine::OnCreate() {
	int a = 0;
	srand((unsigned int)(&a));

	CCollisionMgr::OnCreate(this);
	CSceneMgr::OnCreate(this);
	LoadResources();
	LoadScenes();
}

void GameEngine::OnDestroy() {
	CCollisionMgr::OnDestroy();
	DestroyScenes();
	DestroyResources();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void GameEngine::OnUpdate(float tDeltaTime) {
	Render();

	CCollisionMgr::OnUpdate(tDeltaTime);
	CSceneMgr::OnUpdate(tDeltaTime);

	RemoveDeadObjects();
}

void GameEngine::OnLateUpdate(float tDeltaTime) {
	CCollisionMgr::OnLateUpdate(tDeltaTime);
	CSceneMgr::OnLateUpdate(tDeltaTime);
}

void GameEngine::Render() {
	this->Clear(0.5f, 0.5f, 0.5f);
	CCollisionMgr::Render(mBackBuffer->GetDCMem());
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
	// Maps
	CResourceMgr::Load<CTexture>(this, L"TitleMap", L"../resources/Maps/Brotato_Title_Screen_Map.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob1", L"../resources/Maps/Brotato_Title_Screen_Mobs_1.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob2", L"../resources/Maps/Brotato_Title_Screen_Mobs_2.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob3", L"../resources/Maps/Brotato_Title_Screen_Mobs_3.png");
	CResourceMgr::Load<CTexture>(this, L"TitleLight", L"../resources/Maps/Brotato_Title_Screen_Light.png");
	CResourceMgr::Load<CTexture>(this, L"TitleLogo", L"../resources/Maps/Logo.png");
	CResourceMgr::Load<CTexture>(this, L"TitleBrotato", L"../resources/Maps/splash_art_brotato.png");



	// Sprites
	CResourceMgr::Load<CTexture>(this, L"Cat", L"../resources/Sprites/ChickenAlpha.bmp");
	CResourceMgr::Load<CTexture>(this, L"Player", L"../resources/Sprites/Player.bmp");
}

void GameEngine::DestroyScenes()
{
	CSceneMgr::OnDestroy();
}

void GameEngine::DestroyResources()
{
	CResourceMgr::OnDestroy();
}

void GameEngine::RemoveDeadObjects()
{
	CSceneMgr::RemoveDeadObjects();
}
