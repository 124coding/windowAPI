#include "CScene.h"

void CScene::OnCreate(CAPIEngine* tEngine)
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
		}
		(*it)->Render(tHDC);
	}
}

void CScene::OnEnter()
{
}

void CScene::OnExit()
{
}

void CScene::AddGameObject(GameObject* tGameObj, eLayerType tType)
{
	mLayers[(UINT)tType]->AddGameObject(tGameObj);
}
