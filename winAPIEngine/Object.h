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

	CScene* dontDestroyOnLoad = CSceneMgr::GetDontDestroyOnLoadScene();
	dontDestroyOnLoad->AddGameObject(tObj, tObj->GetLayerType());

}

static SVector2D ObjectSize(GameObject* tObj) {
	CTransform* tr = tObj->GetComponent<CTransform>();
	CSpriteRenderer* sr = tObj->GetComponent<CSpriteRenderer>();

	return SVector2D(tObj->GetSize().mX * tr->GetScale().mX * sr->GetTexture()->GetWidth(), tObj->GetSize().mY * tr->GetScale().mY * sr->GetTexture()->GetHeight());
}

static SVector2D ObjectCenterPos(GameObject* tObj) {
	CTransform* tr = tObj->GetComponent<CTransform>();

	return SVector2D(tr->GetPos().mX - tObj->GetAnchorPoint().mX * tObj->GetSize().mX * tr->GetScale().mX + ObjectSize(tObj).mX / 2, tr->GetPos().mY - tObj->GetAnchorPoint().mY * tObj->GetSize().mY * tr->GetScale().mY + ObjectSize(tObj).mY / 2);
}