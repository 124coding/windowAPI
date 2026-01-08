#pragma once

#include "CAPIEngine.h"

#include "CCollider.h"

#include <functional>
#include <unordered_map>
#include <bitset>

class CScene;

class CCollisionMgr
{

	// 충돌체 쌍(Pair)을 고유한 ID로 만들기 위한 공용체(Union)
	// 두 개의 32비트 ID(Left, Right)를 합쳐서 하나의 64비트 정수(cId)로 변환
	// 용도: std::unordered_map의 Key로 사용하여 두 객체가 충돌 중인지(Enter/Stay/Exit) 상태 관리
	union CollisionID {
		struct {
			UINT32 left;
			UINT32 right;
		};
		UINT64 cId;
	};

public:
	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	// 씬 전환 시 이전 충돌 기록 초기화 (필수: 안 하면 삭제된 객체 참조 오류 발생)
	static void CollisionClear();

	// 충돌 행렬(Matrix) 설정: "Layer A와 Layer B는 충돌 검사를 한다/안한다" 설정
	static void CollisionLayerCheck(eLayerType tLeft, eLayerType tRight, bool tEnable);

	// 실제 두 레이어 간의 모든 오브젝트를 순회하며 충돌 검사 수행
	static void LayerCollision(float tDeltaTime, CScene* tScene, eLayerType tLeft, eLayerType tRight);

	// 두 충돌체(Left, Right) 간의 상태(Enter, Stay, Exit)를 판단하고 이벤트 호출
	static void ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight);

	// 순수 기하학적 교차 검사
	static bool Intersect(CCollider* tLeft, CCollider* tRight);

private:
	// 충돌 매트릭스: 비트셋을 사용하여 각 레이어별 충돌 여부를 O(1)로 확인
	static std::bitset <(UINT)eLayerType::MAX> mCollisionLayerMtrix[(UINT)eLayerType::MAX];

	// 충돌 상태 기록 저장소
	static std::unordered_map<UINT64, bool> mCollisionMap;
};

