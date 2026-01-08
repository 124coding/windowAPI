#pragma once

#include "CEntity.h"
#include "Enums.h"
#include "GameObject.h"

#include <vector>

class CLayer : public CEntity
{
public:
	CLayer() : mGameObjects() {}
	~CLayer() {}
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);

	bool DontSeeObjects(GameObject* tObj);

	void AddGameObject(GameObject* tGameObject);
	void EraseGameObject(GameObject* tGameObject);
	void RemoveDeadObjects();

	const std::vector<GameObject*> GetGameObjects() {
		return this->mGameObjects;
	}

private:
	// 레이어가 가지는 오브젝트들의 집합
	std::vector<GameObject*> mGameObjects;
};

