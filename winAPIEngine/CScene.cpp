#include "CScene.h"

#include "CCollisionMgr.h"

void CScene::OnCreate()
{
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnCreate();
	}
}

void CScene::OnDestroy()
{
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnDestroy();		
		SAFE_DELETE(*it);
	}
}

void CScene::OnUpdate(float tDeltaTime)
{
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnUpdate(tDeltaTime);
	}
}

void CScene::OnLateUpdate(float tDeltaTime)
{
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnLateUpdate(tDeltaTime);
	}
}

void CScene::Render(HDC tHDC)
{
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}		(*it)->Render(tHDC);
	}
}

void CScene::OnEnter()
{
}

void CScene::OnExit()
{
	CCollisionMgr::CollisionClear();
}

void CScene::RemoveDeadObjects() {
	for (auto& layer : mLayers) {
		layer->RemoveDeadObjects();
	}
}

void CScene::AddGameObject(GameObject* tGameObj, eLayerType tType)
{
	mLayers[(UINT)tType]->AddGameObject(tGameObj);
}

void CScene::EraseGameObject(GameObject* tGameObj)
{
	eLayerType layerType = tGameObj->GetLayerType();
	mLayers[(UINT)layerType]->EraseGameObject(tGameObj);
}
