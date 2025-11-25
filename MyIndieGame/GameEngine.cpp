#pragma once

#include "GameEngine.h"
#include "GameObject.h"

#include "CFMOD.h"

#include "CRenderer.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CMonsterSpawnMgr.h"
#include "CDataMgr.h"

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

	mainEngine = this;

	CDataMgr::OnCreate();
	LoadResources();
	CMonsterSpawnMgr::OnCreate();
	// CFMOD::OnCreate();
	CCollisionMgr::OnCreate();
	CUIMgr::OnCreate();
	CSceneMgr::OnCreate();
	LoadScenes();

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
	CMonsterSpawnMgr::OnDestroy();
	CDataMgr::OnDestroy();
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
	this->Clear(0.1f, 0.0f, 0.1f);
	CSceneMgr::Render(mBackBuffer->GetDCMem());
	CCollisionMgr::Render(mBackBuffer->GetDCMem());
	CUIMgr::Render(mBackBuffer->GetDCMem());

	this->Present();
}

void GameEngine::LoadScenes()
{
	// CSceneMgr::CreateScene<CTitleScene>(L"TitleScene");
	// CSceneMgr::CreateScene<CSettingScene>(L"SettingScene");
	CSceneMgr::CreateScene<CPlayScene>(L"PlayScene");
	// CSceneMgr::CreateScene<CEndingScene>(L"EndingScene");
	CSceneMgr::CreateScene <CToolScene>(L"ToolScene");
	

	CSceneMgr::LoadScene(L"PlayScene");
}

void GameEngine::LoadResources()
{
	// TitleScene
	CResourceMgr::Load<CTexture>(L"TitleMap", L"../resources/Maps/Title/Brotato_Title_Screen_Map.png");
	CResourceMgr::Load<CTexture>(L"TitleMob1", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_1.png");
	CResourceMgr::Load<CTexture>(L"TitleMob2", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_2.png");
	CResourceMgr::Load<CTexture>(L"TitleMob3", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_3.png");
	CResourceMgr::Load<CTexture>(L"TitleLight", L"../resources/Maps/Title/Brotato_Title_Screen_Light.png");
	CResourceMgr::Load<CTexture>(L"TitleLogo", L"../resources/Maps/Title/Logo.png");
	CResourceMgr::Load<CTexture>(L"TitleBrotato", L"../resources/Maps/Title/splash_art_brotato.png");



	// Enemies
	LoadMonster(L"BabyAlien", L"../resources/Enemy/BabyAlien/baby_alien.png");

	CResourceMgr::Load<CTexture>(L"EnemyBullet", L"../resources/Enemy/EnemyBullet/enemy_bullet.png");
	CTexture* enemyBirth = CResourceMgr::Load<CTexture>(L"EnemyBirth", L"../resources/Enemy/EnemyBirth/entity_birth.png");
	CTexture::ApplyOtherColorToWantedAreas(30, 255, 255.0f, 0.0f, 0.0f, enemyBirth->GetImage());

	// Player
	CResourceMgr::Load<CTexture>(L"PlayerBase", L"../resources/Player/base.png");

	//Weapon
	CResourceMgr::Load<CTexture>(L"Dagger", L"../resources/Weapons/Dagger/dagger.png");

	CResourceMgr::Load<CTexture>(L"Pistol", L"../resources/Weapons/Pistol/pistol.png");
	CResourceMgr::Load<CTexture>(L"PlayerBullet", L"../resources/Weapons/bullet_0001.png");

	// UI
	CResourceMgr::Load<CTexture>(L"StartOutMouse", L"../resources/ButtonImg/StartButtonOutMouse.png"); 
	CResourceMgr::Load<CTexture>(L"StartInMouse", L"../resources/ButtonImg/StartButtonInMouse.png"); 

	// TileMap
	CResourceMgr::Load<CTexture>(L"Tile1", L"../resources/Maps/Tiles/tiles_1.png");
	CResourceMgr::Load<CTexture>(L"Tile2", L"../resources/Maps/Tiles/tiles_2.png");
	CResourceMgr::Load<CTexture>(L"Tile3", L"../resources/Maps/Tiles/tiles_3.png");
	CResourceMgr::Load<CTexture>(L"Tile4", L"../resources/Maps/Tiles/tiles_4.png");
	CResourceMgr::Load<CTexture>(L"Tile5", L"../resources/Maps/Tiles/tiles_5.png");
	CResourceMgr::Load<CTexture>(L"Tile6", L"../resources/Maps/Tiles/tiles_6.png");
	CResourceMgr::Load<CTexture>(L"TileOutLine", L"../resources/Maps/Tiles/tiles_outline_modify.png");


}

void GameEngine::LoadMonster(std::wstring tName, std::wstring tPath)
{
	std::wstring collisionMonster = tName + L"Collision";

	CTexture* monsterImg = CResourceMgr::Load<CTexture>(tName, tPath);
	CTexture* monsterCollisionImg = CTexture::Create(collisionMonster, monsterImg->GetWidth(), monsterImg->GetHeight());

	CTexture::ApplySolidColor((BYTE)255, (BYTE)255, (BYTE)255, monsterCollisionImg->GetImage(), monsterImg->GetImage());

	monsterImg->CreateHBitmapFromGdiPlus(false);
	monsterCollisionImg->CreateHBitmapFromGdiPlus(false);
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
