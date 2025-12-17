#pragma once

#include "CComponent.h"
#include "GameObject.h"
#include "CLayer.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "CCollider.h"

#include <Windows.h>

template<typename T>
static T* Instantiate(eLayerType tType, CScene* targetScene = nullptr) {
	T* gameObject = new T();
	gameObject->SetLayerType(tType);

	CScene* scene = nullptr;

	if (targetScene != nullptr) {
		scene = targetScene;
	}
	else {
		scene = CSceneMgr::GetActiveScene();
	}

	CLayer* layer = scene->GetLayer(tType);

	layer->AddGameObject(gameObject);

	return gameObject;
}

template<typename T>
static T* Instantiate(eLayerType tType, SVector2D tPosition, CScene* targetScene = nullptr) {
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

template<typename T>
static void AddObjectToScene(T* tObj) {
	CScene* activeScene = CSceneMgr::GetActiveScene();

	CLayer* layer = activeScene->GetLayer(tObj->GetLayerType());
	layer->AddGameObject(tObj);
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

	return SVector2D(tObj->GetSize().mX * sr->GetTexture()->GetWidth(), tObj->GetSize().mY * sr->GetTexture()->GetHeight());
}

static SVector2D ObjectCenterPos(GameObject* tObj) {
	CTransform* tr = tObj->GetComponent<CTransform>();
	CCollider* cl = tObj->GetComponent<CCollider>();

	float fCos = cos(DegToRad(tr->GetRot()));
	float fSin = sin(DegToRad(tr->GetRot()));

	SVector2D vLocalLC, vRotC, centerPos;

	if (cl == nullptr) {
		vLocalLC.mX = -tObj->GetAnchorPoint().mX * tObj->GetSize().mX;
		vLocalLC.mY = -tObj->GetAnchorPoint().mY * tObj->GetSize().mY;

		vLocalLC = vLocalLC + (ObjectSize(tObj) / 2);

		vRotC = SVector2D(vLocalLC.mX * fCos - vLocalLC.mY * fSin, vLocalLC.mX * fSin + vLocalLC.mY * fCos);
	}
	else {
		vLocalLC.mX = -tObj->GetAnchorPoint().mX * tObj->GetSize().mX * cl->GetSize().mX + cl->GetOffset().mX;
		vLocalLC.mY = -tObj->GetAnchorPoint().mY * tObj->GetSize().mY * cl->GetSize().mY + cl->GetOffset().mY;

		vLocalLC = vLocalLC + (ObjectSize(tObj) * cl->GetSize() / 2);

		vRotC = SVector2D(vLocalLC.mX * fCos - vLocalLC.mY * fSin, vLocalLC.mX * fSin + vLocalLC.mY * fCos);
	}

	centerPos = tr->GetPos() + vRotC;

	return centerPos; // SVector2D(tr->GetPos().mX - tObj->GetAnchorPoint().mX * tObj->GetSize().mX + ObjectSize(tObj).mX / 2, tr->GetPos().mY - tObj->GetAnchorPoint().mY * tObj->GetSize().mY + ObjectSize(tObj).mY / 2);
}