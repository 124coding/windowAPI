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
	CScene* activeScene = CSceneMgr::GetActiveScene();

	CLayer* layer = activeScene->GetLayer(tType);
	layer->AddGameObject(gameObject);

	return gameObject;
}

template<typename T>
static T* Instantiate(CAPIEngine* tEngine, eLayerType tType, SVector2D tPosition = SVector2D(0.0f, 0.0f), std::wstring tKey = L"") {
	T* gameObject = new T();
	CScene* activeScene = CSceneMgr::GetActiveScene();

	CLayer* layer = activeScene->GetLayer(tType);
	layer->AddGameObject(gameObject);

	CTransform* tr = gameObject->GetComponent<CTransform>();
	tr->SetPos(tPosition);
	tr->SetName(L"TR");

	CSpriteRenderer* sr = gameObject->GetComponent<CSpriteRenderer>();

	CTexture* img = CResourceMgr::Find<CTexture>(tKey);
	sr->SetTexture(img);
	sr->SetName(L"SR");
	sr->SetSize(SVector2D(3.0f, 3.0f));

	return gameObject;
}