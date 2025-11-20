#pragma once

#include "GameEngine.h"
#include "GameObject.h"

#include "CFMOD.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CMonsterSpawnMgr.h"

#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"
#include "CToolScene.h"
#include "CSettingScene.h"

#include "SVector2D.h"
#include "time.h"

class CBabyAlien;

void GameEngine::OnCreate() {
	int a = 0;
	srand((unsigned int)(&a));

	LoadResources();
	LoadDatas();
	CMonsterSpawnMgr::LoadMonsterBasicStats();

	// CFMOD::OnCreate();
	CCollisionMgr::OnCreate(this);
	CUIMgr::OnCreate(this);
	LoadScenes();
	CSceneMgr::OnCreate(this);

	CInputMgr::GetInst()->AddKeyInfo("DoMoveLt", 'A');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveRt", 'D');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveFt", 'W');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveBt", 'S');
	CInputMgr::GetInst()->AddKeyInfo("DoSave", 'K');
	CInputMgr::GetInst()->AddKeyInfo("DoLoad", 'L');
	CInputMgr::GetInst()->AddKeyInfo("MouseLeftClick", VK_LBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseRightClick", VK_RBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseWheelClick", VK_MBUTTON);
}

void GameEngine::OnDestroy() {
	DestroyResources();
	DestroyScenes();
	CUIMgr::OnDestroy();
	CCollisionMgr::OnDestroy();
}

void GameEngine::OnUpdate(float tDeltaTime) {
	Render();

	CUIMgr::OnUpdate(tDeltaTime);
	CCollisionMgr::OnUpdate(tDeltaTime);
	CSceneMgr::OnUpdate(tDeltaTime);
	RemoveDeadObjects();
}

void GameEngine::OnLateUpdate(float tDeltaTime) {
	CUIMgr::OnLateUpdate(tDeltaTime);
	CCollisionMgr::OnLateUpdate(tDeltaTime);
	CSceneMgr::OnLateUpdate(tDeltaTime);
}

void GameEngine::Render() {
	this->Clear(0.5f, 0.5f, 0.5f);
	CSceneMgr::Render(mBackBuffer->GetDCMem());
	CCollisionMgr::Render(mBackBuffer->GetDCMem());
	CUIMgr::Render(mBackBuffer->GetDCMem());

	this->Present();
}

void GameEngine::LoadScenes()
{
	CSceneMgr::CreateScene<CTitleScene>(this, L"TitleScene");
	// CSceneMgr::CreateScene<CSettingScene>(this, L"SettingScene");
	CSceneMgr::CreateScene<CPlayScene>(this, L"PlayScene");
	// CSceneMgr::CreateScene<CEndingScene>(this, L"EndingScene");
	// CSceneMgr::CreateScene<CToolScene>(this, L"ToolScene");
	

	CSceneMgr::LoadScene(L"PlayScene");
}

void GameEngine::LoadResources()
{
	// TitleScene
	CResourceMgr::Load<CTexture>(this, L"TitleMap", L"../resources/Maps/Brotato_Title_Screen_Map.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob1", L"../resources/Maps/Brotato_Title_Screen_Mobs_1.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob2", L"../resources/Maps/Brotato_Title_Screen_Mobs_2.png");
	CResourceMgr::Load<CTexture>(this, L"TitleMob3", L"../resources/Maps/Brotato_Title_Screen_Mobs_3.png");
	CResourceMgr::Load<CTexture>(this, L"TitleLight", L"../resources/Maps/Brotato_Title_Screen_Light.png");
	CResourceMgr::Load<CTexture>(this, L"TitleLogo", L"../resources/Maps/Logo.png");
	CResourceMgr::Load<CTexture>(this, L"TitleBrotato", L"../resources/Maps/splash_art_brotato.png");



	// Sprites
	CResourceMgr::Load<CTexture>(this, L"BabyAlien", L"../resources/Sprites/Enemy/baby_alien.png");
	CResourceMgr::Load<CTexture>(this, L"BabyAlienCollsion", L"../resources/Sprites/Enemy/baby_alien_white.png");

	CResourceMgr::Load<CTexture>(this, L"EnemyBullet", L"../resources/Sprites/Enemy/enemy_bullet.png");

	// Player
	CResourceMgr::Load<CTexture>(this, L"PlayerBase", L"../resources/Sprites/Player/base.png");

	//Weapon
	CResourceMgr::Load<CTexture>(this, L"Dagger", L"../resources/Sprites/Weapon/dagger.png");

	CResourceMgr::Load<CTexture>(this, L"Pistol", L"../resources/Sprites/Weapon/pistol.png");
	CResourceMgr::Load<CTexture>(this, L"PlayerBullet", L"../resources/Sprites/Weapon/player_bullet.png");

	// UI
	CResourceMgr::Load<CTexture>(this, L"StartOutMouse", L"../resources/ButtonImg/StartButtonOutMouse.png"); 
	CResourceMgr::Load<CTexture>(this, L"StartInMouse", L"../resources/ButtonImg/StartButtonInMouse.png"); 

	// Tool
	CResourceMgr::Load<CTexture>(this, L"SpringFloor", L"../resources/SpringFloor.bmp");


}

void GameEngine::LoadDatas() {
	CMonsterSpawnMgr::Register<CBabyAlien>("CBabyAlien");
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
