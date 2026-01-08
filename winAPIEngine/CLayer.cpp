#include "CLayer.h"

void CLayer::OnCreate() {
	// 레이어에 소속된 모든 오브젝트 초기화
	for (auto it = mGameObjects.begin(); it != mGameObjects.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnCreate();
	}
}

void CLayer::OnDestroy() {
	// 레이어 삭제 시 소속된 모든 오브젝트 메모리 해제
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
		
		// 일시정지(Paused)거나 죽은(Dead) 상태면 업데이트 스킵
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

		// 일시정지(Paused)거나 죽은(Dead) 상태면 업데이트 스킵
		if (DontSeeObjects(*it)) {
			continue;
		}
		else {
			(*it)->Render(tHDC);
		}
	}
}

// 오브젝트의 상태를 확인하여 로직 처리를 할지 말지 결정 (최적화)
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

// 리스트에서 오브젝트를 제거하되, 메모리는 해제하지 않음
// (주로 씬 이동이나 레이어 변경(DontDestroyOnLoad 등) 시 사용)
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

// 'Dead' 상태인 오브젝트를 찾아 실제 메모리 해제 (Garbage Collection)
// 프레임의 마지막(Scene::OnExit 등)에 호출되어야 안전함
void CLayer::RemoveDeadObjects()
{
	for (auto it = mGameObjects.begin(); it != mGameObjects.end();) {
		if (*it == nullptr) {
			continue;
		}

		GameObject::eState active = (*it)->GetState();

		if (active == GameObject::eState::Dead) {
			
			GameObject* deadObj = (*it);

			// 벡터에서 제거 (erase는 다음 유효한 이터레이터를 반환하므로 it 갱신)
			it = mGameObjects.erase(it);

			// 메모리 해제
			deadObj->OnDestroy();
			SAFE_DELETE(deadObj);

			continue;
		}

		it++;
	}
}