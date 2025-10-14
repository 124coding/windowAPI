#include "CCollisionMgr.h"

#include "GameObject.h"

#include "CSceneMgr.h"

std::bitset <(UINT)eLayerType::MAX> CCollisionMgr::mCollisionLayerMtrix[(UINT)eLayerType::MAX] = {};

void CCollisionMgr::OnCreate(CAPIEngine* tEngine){

}

void CCollisionMgr::OnDestroy() {

}

void CCollisionMgr::OnUpdate(float tDeltaTime) {
	CScene* scene = CSceneMgr::GetActiveScene();

	for (UINT row = 0; row < (UINT)eLayerType::MAX; row++) {
		for (UINT col = 0; col < (UINT)eLayerType::MAX; col++) {
			if (mCollisionLayerMtrix[row][col] == true) {
				LayerCollision(scene, (eLayerType)row, (eLayerType)col);
			}
		}
	}
}

void CCollisionMgr::OnLateUpdate(float tDeltaTime) {

}

void CCollisionMgr::Render(HDC tHDC) {

}

void CCollisionMgr::LayerCollision(CScene* tScene, eLayerType tLeft, eLayerType tRight) {
	const std::vector<GameObject*>& lefts = tScene->GetLayer(tLeft)->GetGameObjects();
	const std::vector<GameObject*>& rights = tScene->GetLayer(tRight)->GetGameObjects();

	for (GameObject* left : lefts) {
		if (left->IsActive() == false) continue;

		CCollider* leftCollider = left->GetComponent<CCollider>();
		if (leftCollider == nullptr) continue;
		
		for (GameObject* right : rights) {
			if (right->IsActive() == false) continue;

			CCollider* rightCollider = right->GetComponent<CCollider>();
			if (rightCollider == nullptr) continue;

			if (left == right) {
				continue;
			}

			ColliderCollision(leftCollider, rightCollider);
		}
	}
}

void CCollisionMgr::ColliderCollision(CCollider* tLeft, CCollider* tRight) {

}
