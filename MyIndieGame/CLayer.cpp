#include "CLayer.h"

void CLayer::OnCreate() {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnCreate();
	}
}

void CLayer::OnDestroy() {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnDestroy();
		SAFE_DELETE(*it);
	}
}

void CLayer::OnUpdate(float tDeltaTime) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnUpdate(tDeltaTime);
	}
}

void CLayer::OnLateUpdate(float tDeltaTime) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnLateUpdate(tDeltaTime);
	}
}

void CLayer::Render(HDC tHDC) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->Render(tHDC);
	}
}

void CLayer::AddGameObject(GameObject* tGameObject)
{
	if (tGameObject == nullptr) {
		return;
	}
	mGameObjects.push_back(tGameObject);
}