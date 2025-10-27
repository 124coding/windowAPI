#pragma once

#include "CAPIEngine.h"

#include "CCollider.h"

#include <unordered_map>
#include <bitset>

class CScene;

class CCollisionMgr
{
	union CollisionID {
		struct {
			UINT32 left;
			UINT32 right;
		};
		UINT64 cId;
	};

public:
	static void OnCreate(CAPIEngine* tEngine);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void CollisionClear();

	static void CollisionLayerCheck(eLayerType tLeft, eLayerType tRight, bool tEnable);
	static void LayerCollision(float tDeltaTime, CScene* tScene, eLayerType tLeft, eLayerType tRight);
	static void ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight);
	static bool Intersect(CCollider* tLeft, CCollider* tRight);

private:
	static std::bitset <(UINT)eLayerType::MAX> mCollisionLayerMtrix[(UINT)eLayerType::MAX];
	static std::unordered_map<UINT64, bool> mCollisionMap;
};

