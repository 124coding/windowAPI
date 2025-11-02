#define NOMINMAX
#include "CCollisionMgr.h"

#include "GameObject.h"

#include "CSceneMgr.h"

#include "Object.h"

std::bitset <(UINT)eLayerType::MAX> CCollisionMgr::mCollisionLayerMtrix[(UINT)eLayerType::MAX] = {};
std::unordered_map<UINT64, bool> CCollisionMgr::mCollisionMap = {};

void CCollisionMgr::OnCreate(CAPIEngine* tEngine){

}

void CCollisionMgr::OnDestroy() {

}

void CCollisionMgr::OnUpdate(float tDeltaTime) {
	CScene* scene = CSceneMgr::GetActiveScene();

	for (UINT row = 0; row < (UINT)eLayerType::MAX; row++) {
		for (UINT col = 0; col < (UINT)eLayerType::MAX; col++) {
			if (mCollisionLayerMtrix[row][col] == true) {
				LayerCollision(tDeltaTime, scene, (eLayerType)row, (eLayerType)col);
			}
		}
	}
}

void CCollisionMgr::OnLateUpdate(float tDeltaTime) {

}

void CCollisionMgr::Render(HDC tHDC) {

}

void CCollisionMgr::CollisionClear()
{
	mCollisionMap.clear();
	mCollisionLayerMtrix->reset();
}

void CCollisionMgr::CollisionLayerCheck(eLayerType tLeft, eLayerType tRight, bool tEnable) {
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

void CCollisionMgr::LayerCollision(float tDeltaTime, CScene* tScene, eLayerType tLeft, eLayerType tRight) {
	const std::vector<GameObject*>& lefts = CSceneMgr::GetGameObjects(tLeft);
	const std::vector<GameObject*>& rights = CSceneMgr::GetGameObjects(tRight);

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

			ColliderCollision(tDeltaTime, leftCollider, rightCollider);
		}
	}
}

void CCollisionMgr::ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight) {
	
	// 두 충돌체 번호를 가져온 ID를 확인해서 CollisionID값 세팅
	CollisionID id = {};
	id.left = tLeft->GetID();
	id.right = tRight->GetID();

	// 해당 ID로 정보 검색 없으면 생성
	auto iter = mCollisionMap.find(id.cId);

	if (iter == mCollisionMap.end()) {
		mCollisionMap.insert(std::make_pair(id.cId, false));
		iter = mCollisionMap.find(id.cId);
	}

	if (Intersect(tLeft, tRight)) {
		if (iter->second == false) { // 최초 충돌
			tLeft->OnCollisionEnter(tDeltaTime, tRight);
			tRight->OnCollisionEnter(tDeltaTime, tLeft);
			iter->second = true;
		}
		else { // 충돌중
			tLeft->OnCollisionStay(tDeltaTime, tRight);
			tRight->OnCollisionStay(tDeltaTime, tLeft);
		}
	}
	else {
		if (iter->second == true) { // 충돌 빠져나감
			tLeft->OnCollisionExit(tDeltaTime, tRight);
			tRight->OnCollisionExit(tDeltaTime, tLeft);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::Intersect(CCollider* tLeft, CCollider* tRight)
{
	CTransform* leftTr = tLeft->GetOwner()->GetComponent<CTransform>();
	CTransform* rightTr = tRight->GetOwner()->GetComponent<CTransform>();

	CSpriteRenderer* leftSr = tLeft->GetOwner()->GetComponent<CSpriteRenderer>();
	CSpriteRenderer* rightSr = tRight->GetOwner()->GetComponent<CSpriteRenderer>();
	
	SVector2D leftPos = leftTr->GetPos() + tLeft->GetOffset();
	SVector2D rightPos = rightTr->GetPos() + tRight->GetOffset();

	SVector2D leftSize = ObjectSize(tLeft->GetOwner());
	leftSize.mX = leftSize.mX * tLeft->GetSize().mX;
	leftSize.mY = leftSize.mY * tLeft->GetSize().mY;

	SVector2D rightSize = ObjectSize(tRight->GetOwner());
	rightSize.mX = rightSize.mX * tRight->GetSize().mX;
	rightSize.mY = rightSize.mY * tRight->GetSize().mY;

	SVector2D leftCenterPos = ObjectCenterPos(tLeft->GetOwner());
	SVector2D rightCenterPos = ObjectCenterPos(tRight->GetOwner()); // SVector2D(rightPos.mX - tRight->GetOwner()->GetAnchorPoint().mX * tRight->GetSize().mX * tRight->GetOwner()->GetSize().mX * rightTr->GetScale().mX + rightSize.mX, rightPos.mY - tRight->GetOwner()->GetAnchorPoint().mY * tRight->GetSize().mY * tRight->GetOwner()->GetSize().mY * rightTr->GetScale().mY + rightSize.mY);

	float distance = (leftCenterPos - rightCenterPos).LengthSq();

	// AABB 충돌

	eColliderType leftType = tLeft->GetColliderType();
	eColliderType rightType = tRight->GetColliderType();

	// 각 오브젝트의 중심을 기준으로
	// 수정 필요
	if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
		// rect - rect
		if (fabs(leftCenterPos.mX - rightCenterPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
			fabs((leftCenterPos.mY - rightCenterPos.mY) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f))) {

			return true;
		}
	}
	else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {// circle - circle

		if (distance <= (leftSize.mX / 2.0f + rightSize.mX / 2.0f) * (leftSize.mX / 2.0f + rightSize.mX / 2.0f)) {
			return true;
		}
	}
	else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Circle2D ||
			leftType == eColliderType::Circle2D && rightType == eColliderType::Rect2D) { // circle - rect

		SVector2D circlePos, rectPos, rectHalfSize;
		float circleRadius;

		if (leftType == eColliderType::Circle2D) {
			circlePos = SVector2D(leftCenterPos.mX, leftCenterPos.mY);
			circleRadius = leftSize.mX / 2.0f;
			rectPos = SVector2D(rightCenterPos.mX, rightCenterPos.mY);
			rectHalfSize = SVector2D(rightSize.mX / 2.0f, rightSize.mY / 2.0f);
		}
		else {
			circlePos = SVector2D(rightCenterPos.mX, rightCenterPos.mY);
			circleRadius = rightSize.mX / 2.0f;
			rectPos = SVector2D(leftCenterPos.mX, leftCenterPos.mY);
			rectHalfSize = SVector2D(leftSize.mX / 2.0f, leftSize.mY / 2.0f);
		}

		// 클램핑(Clamping) 기법 이용 충돌 코드
		float rectMinX = rectPos.mX - rectHalfSize.mX;
		float rectMaxX = rectPos.mX + rectHalfSize.mX;
		float rectMinY = rectPos.mY - 2 * rectHalfSize.mY;
		float rectMaxY = rectPos.mY;

		float closestX = std::max(rectMinX, std::min(circlePos.mX, rectMaxX));
		float closestY = std::max(rectMinY, std::min(circlePos.mY, rectMaxY));

		SVector2D closestPoint(closestX, closestY);

		float distanceSquared = (circlePos - closestPoint).LengthSq();
		float radiusSquared = circleRadius * circleRadius;

		if (distanceSquared < radiusSquared) {
			return true;
		}
	}
	return false;
}