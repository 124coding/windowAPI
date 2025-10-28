#pragma once

#include "GameEngine.h"
#include "GameObject.h"

#include "CFMOD.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"

#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"
#include "CToolScene.h"
#include "CSettingScene.h"

#include "SVector2D.h"
#include "time.h"

void GameEngine::OnCreate() {
	int a = 0;
	srand((unsigned int)(&a));

	LoadResources();

	CFMOD::OnCreate();
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void GameEngine::OnUpdate(float tDeltaTime) {
	Render();

	CCollisionMgr::OnUpdate(tDeltaTime);
	CUIMgr::OnUpdate(tDeltaTime);
	CSceneMgr::OnUpdate(tDeltaTime);

	RemoveDeadObjects();
}

void GameEngine::OnLateUpdate(float tDeltaTime) {
	CCollisionMgr::OnLateUpdate(tDeltaTime);
	CUIMgr::OnLateUpdate(tDeltaTime);
	CSceneMgr::OnLateUpdate(tDeltaTime);
}

void GameEngine::Render() {
	this->Clear(0.5f, 0.5f, 0.5f);
	CCollisionMgr::Render(mBackBuffer->GetDCMem());
	CSceneMgr::Render(mBackBuffer->GetDCMem());
	CUIMgr::Render(mBackBuffer->GetDCMem());

	this->Present();
}

void GameEngine::LoadScenes()
{
	// CSceneMgr::CreateScene<CTitleScene>(this, L"TitleScene");
	// CSceneMgr::CreateScene<CSettingScene>(this, L"SettingScene");
	CSceneMgr::CreateScene<CPlayScene>(this, L"PlayScene");
	// CSceneMgr::CreateScene<CEndingScene>(this, L"EndingScene");
	// CSceneMgr::CreateScene<CToolScene>(this, L"ToolScene");
	

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
	CResourceMgr::Load<CTexture>(this, L"BabyAlien", L"../resources/Sprites/Enemy/baby_alien.png");
	CResourceMgr::Load<CTexture>(this, L"PlayerBase", L"../resources/Sprites/Player/base.png");

	CResourceMgr::Load<CTexture>(this, L"StartOutMouse", L"../resources/ButtonImg/StartButtonOutMouse.png"); 
	CResourceMgr::Load<CTexture>(this, L"StartInMouse", L"../resources/ButtonImg/StartButtonInMouse.png"); 

	CResourceMgr::Load<CTexture>(this, L"SpringFloor", L"../resources/SpringFloor.bmp");

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
