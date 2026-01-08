#pragma once
#include "CEntity.h"
#include "GameObject.h"
#include "CLayer.h"

#include <vector>


class CScene : public CEntity
{
public:
	CScene() : mLayers() { 
		// 레이어 타입의 개수만큼 벡터 크기 조정 및 메모리 할당
		mLayers.resize((UINT)eLayerType::MAX);
		
		for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
			*it = new CLayer();
		}
	}
	virtual ~CScene() {};

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);

	// ==========================================
	// Scene Transitions (씬 전환)
	// ==========================================
	virtual void OnEnter();
	virtual void OnExit();

	// 특정 레이어에 게임 오브젝트를 추가
	void AddGameObject(GameObject* tGameObj, eLayerType tType);

	// 씬(레이어)에서 오브젝트 연결 해제 (메모리 해제는 아님)
	void EraseGameObject(GameObject* tGameObj);

	// 상태가 'Dead'인 오브젝트들을 일괄적으로 메모리 해제 (Garbage Collection)
	void RemoveDeadObjects();

	CLayer* GetLayer(eLayerType tType) { return mLayers[(UINT)tType]; }

private:
	// 씬을 구성하는 레이어 목록 (예: 배경, 플레이어, UI 등)
	std::vector<CLayer*> mLayers;
};