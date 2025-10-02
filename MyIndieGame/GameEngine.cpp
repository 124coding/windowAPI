#pragma once

#include "GameEngine.h"
#include "GameObject.h"
#include "CInputMgr.h"
#include "SVector2D.h"
#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"
#include "CSceneMgr.h"

void GameEngine::OnCreate() {
	LoadScenes();
	CInputMgr::GetInst()->AddKeyInfo("ChangeScene", 'N');

	CSceneMgr::OnCreate();
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
	CSceneMgr::CreateScene<CTitleScene>(L"TitleScene");
	CSceneMgr::CreateScene<CPlayScene>(L"PlayScene");
	CSceneMgr::CreateScene<CEndingScene>(L"EndingScene");

	CSceneMgr::LoadScene(L"PlayScene");
}
