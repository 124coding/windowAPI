#pragma once

#include "CComponent.h"
#include "GameObject.h"
#include "CLayer.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CTexture.h"
#include "CResourceMgr.h"

#include <Windows.h>

template<typename T>
static T* Instantiate(eLayerType tType) {
	T* gameObject = new T();
	gameObject->SetLayerType(tType);

	CScene* activeScene = CSceneMgr::GetActiveScene();
	CLayer* layer = activeScene->GetLayer(tType);

	layer->AddGameObject(gameObject);

	return gameObject;
}

template<typename T>
static T* Instantiate(CAPIEngine* tEngine, eLayerType tType, SVector2D tPosition = SVector2D()) {
	T* gameObject = new T();
	gameObject->SetLayerType(tType);

	CScene* activeScene = CSceneMgr::GetActiveScene();

	CLayer* layer = activeScene->GetLayer(tType);
	layer->AddGameObject(gameObject);

	CTransform* tr = gameObject->GetComponent<CTransform>();
	tr->SetPos(tPosition);
	tr->SetName(L"TR");

	return gameObject;
}

static void ObjDestroy(GameObject* tObj) {
	tObj->Death();
}

static void DontDestroyOnLoad(GameObject* tObj) {
	CScene* activeScene = CSceneMgr::GetActiveScene();
	activeScene->EraseGameObject(tObj);

	CScene* dontDestroyOnLoad = CSceneMgr::GetDontDestroyOnLoad();
	dontDestroyOnLoad->AddGameObject(tObj, tObj->GetLayerType());

}