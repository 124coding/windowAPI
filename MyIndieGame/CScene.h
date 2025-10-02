#pragma once
#include "CEntity.h"
#include "GameObject.h"
#include "CLayer.h"

#include <vector>


class CScene : public CEntity
{
public:
	CScene() : mLayers() { 
		mLayers.resize((UINT)eLayerType::MAX);
		
		for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
			*it = new CLayer();
		}
	}
	~CScene() {};

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC hDC);

	virtual void OnEnter();
	virtual void OnExit();

	void AddGameObject(GameObject* tGameObj, eLayerType tType);

private:
	std::vector<CLayer*> mLayers;
};