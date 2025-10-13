#pragma once

#include "CEntity.h"
#include "Enums.h"
#include "GameObject.h"

#include <vector>

class CLayer : public CEntity
{
public:
	CLayer() : mType(), mGameObjects() {}
	~CLayer() {}
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);

	bool DontSeeObjects(GameObject* tObj);

	void AddGameObject(GameObject* tGameObject);
	void RemoveDeadObjects();

private:
	eLayerType mType;
	std::vector<GameObject*> mGameObjects;
};

