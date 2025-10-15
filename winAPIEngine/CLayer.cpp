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
		if (*it == nullptr){
			continue;
		}
		
		if (DontSeeObjects(*it)) {
			continue;
		}
		else {
			(*it)->OnUpdate(tDeltaTime);
		}
	}
}

void CLayer::OnLateUpdate(float tDeltaTime) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}

		if (DontSeeObjects(*it)) {
			continue;
		}
		else {
			(*it)->OnLateUpdate(tDeltaTime);
		}
	}
}

void CLayer::Render(HDC tHDC) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}

		if (DontSeeObjects(*it)) {
			continue;
		}
		else {
			(*it)->Render(tHDC);
		}
	}
}

bool CLayer::DontSeeObjects(GameObject* tObj)
{
	if (tObj->GetState() == GameObject::eState::Paused || tObj->GetState() == GameObject::eState::Dead) {
		return true;
	}
	return false;
}

void CLayer::AddGameObject(GameObject* tGameObject)
{
	if (tGameObject == nullptr) {
		return;
	}
	mGameObjects.push_back(tGameObject);
}

void CLayer::EraseGameObject(GameObject* tGameObject) {
	for (auto it = mGameObjects.begin(); it != mGameObjects.end();) {
		if (*it == nullptr) {
			continue;
		}

		if (*it == tGameObject) {
			it = mGameObjects.erase(it);

			continue;
		}

		it++;
	}
}

void CLayer::RemoveDeadObjects()
{
	for (auto it = mGameObjects.begin(); it != mGameObjects.end();) {
		if (*it == nullptr) {
			continue;
		}

		GameObject::eState active = (*it)->GetState();

		if (active == GameObject::eState::Dead) {
			
			GameObject* deadObj = (*it);

			it = mGameObjects.erase(it);

			SAFE_DELETE(deadObj);

			continue;
		}

		it++;
	}
}