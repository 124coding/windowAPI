#include "CScene.h"

#include "CCollisionMgr.h"

void CScene::OnCreate()
{
	// 모든 레이어를 순회하며 초기화(OnCreate) 호출
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnCreate();
	}
}

void CScene::OnDestroy()
{
	// 씬 소멸 시, 보유한 모든 레이어의 메모리를 해제
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
	// 각 레이어에 업데이트 요청 (배경 -> 게임오브젝트 -> UI 순서 등 레이어 순서대로 진행)
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnUpdate(tDeltaTime);
	}
}

void CScene::OnLateUpdate(float tDeltaTime)
{
	// 모든 오브젝트의 로직이 끝난 후, 후처리 작업 수행
	for (auto it = mLayers.begin(); it != mLayers.end(); it++) {
		if (*it == nullptr) {
			continue;
		}
		(*it)->OnLateUpdate(tDeltaTime);
	}
}

void CScene::Render(HDC tHDC)
{
	// 렌더링 요청 (앞쪽 레이어가 나중에 그려져 화면 위에 덮어씌워짐)
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
	// 씬을 나갈 때, 이전 씬의 충돌 체크 등록 정보를 모두 초기화해야 함
	CCollisionMgr::CollisionClear();
}

void CScene::RemoveDeadObjects() {
	// 각 레이어에게 'Dead' 상태인 오브젝트를 삭제하라고 명령 (Garbage Collection)
	for (auto& layer : mLayers) {
		layer->RemoveDeadObjects();
	}
}

void CScene::AddGameObject(GameObject* tGameObj, eLayerType tType)
{
	// 해당 레이어를 찾아 오브젝트 추가 위임
	mLayers[(UINT)tType]->AddGameObject(tGameObj);
}

void CScene::EraseGameObject(GameObject* tGameObj)
{
	// 해당 오브젝트가 소속된 레이어를 찾아 목록에서 제거 (메모리 해제 아님)
	eLayerType layerType = tGameObj->GetLayerType();
	mLayers[(UINT)layerType]->EraseGameObject(tGameObj);
}
