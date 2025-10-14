#pragma once

#include "CAPIEngine.h"

#include "CCollider.h"

#include <bitset>

class CScene;

class CCollisionMgr
{
public:
	static void OnCreate(CAPIEngine* tEngine);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void CollisionLayerCheck(eLayerType tLeft, eLayerType tRight, bool tEnable) {
		int row = 0;
		int col = 0;

		if (tLeft <= tRight) {
			row = (UINT)tLeft;
			col = (UINT)tRight;
		}
		else {
			row = (UINT)tRight;
			col = (UINT)tLeft;
		}

		mCollisionLayerMtrix[row][col] = tEnable;
	}

	static void LayerCollision(CScene* tScene, eLayerType tLeft, eLayerType tRight);
	static void ColliderCollision(CCollider* tLeft, CCollider* tRight);

private:
	static std::bitset <(UINT)eLayerType::MAX> mCollisionLayerMtrix[(UINT)eLayerType::MAX];
};

