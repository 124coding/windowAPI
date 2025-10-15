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
	virtual ~CScene() {};

	virtual void OnCreate(CAPIEngine* tEngine);
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);

	virtual void OnEnter();
	virtual void OnExit();

	void AddGameObject(GameObject* tGameObj, eLayerType tType);
	void EraseGameObject(GameObject* tGameObj);
	void RemoveDeadObjects();

	CLayer* GetLayer(eLayerType tType) { return mLayers[(UINT)tType]; }

private:
	std::vector<CLayer*> mLayers;
};