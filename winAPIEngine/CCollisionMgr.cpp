#define NOMINMAX
#include "CCollisionMgr.h"

#include "GameObject.h"

#include "CSceneMgr.h"

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
	
	SVector2D leftPos = leftTr->GetPos() + tLeft->GetOffset();
	SVector2D rightPos = rightTr->GetPos() + tRight->GetOffset();

	SVector2D leftSize = SVector2D(tLeft->GetSize().mX * tLeft->GetOwner()->GetAnchorPoint().mX * 2, tLeft->GetSize().mX * tLeft->GetOwner()->GetAnchorPoint().mY);
	SVector2D rightSize = SVector2D(tRight->GetSize().mX * tRight->GetOwner()->GetAnchorPoint().mX * 2, tRight->GetSize().mX * tRight->GetOwner()->GetAnchorPoint().mY);

	// AABB 충돌

	eColliderType leftType = tLeft->GetColliderType();
	eColliderType rightType = tRight->GetColliderType();

	// 각 오브젝트의 중심을 기준으로 (현재 AnchorPoint는 중간 바닥)
	if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
		// rect - rect
		if (fabs(leftPos.mX - rightPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
			fabs((leftPos.mY - leftSize.mY / 2.0f) - (rightPos.mY - rightSize.mY / 2.0f)) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f)) {

			return true;
		}
	}
	else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {// circle - circle

		SVector2D leftCirclePos = SVector2D(leftPos.mX, leftPos.mY - leftSize.mY / 2.0f);
		SVector2D rightCirclePos = SVector2D(rightPos.mX, rightPos.mY - rightSize.mY / 2.0f);

		float distance = (leftCirclePos - rightCirclePos).Length();

		if (distance <= (leftSize.mX / 2.0f + rightSize.mX / 2.0f)) {
			return true;
		}
	}
	else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Circle2D ||
			leftType == eColliderType::Circle2D && rightType == eColliderType::Rect2D) { // circle - rect

		SVector2D circlePos, rectPos, rectHalfSize;
		float circleRadius;

		if (leftType == eColliderType::Circle2D) {
			circlePos = SVector2D(leftPos.mX, leftPos.mY - leftSize.mY / 2.0f);
			circleRadius = leftSize.mX / 2.0f;
			rectPos = SVector2D(rightPos.mX, rightPos.mY - rightSize.mY / 2.0f);
			rectHalfSize = SVector2D(rightSize.mX / 2.0f, rightSize.mY / 2.0f);
		}
		else {
			circlePos = SVector2D(rightPos.mX, rightPos.mY - rightSize.mY / 2.0f);
			circleRadius = rightSize.mX / 2.0f;
			rectPos = SVector2D(leftPos.mX, leftPos.mY - leftSize.mY / 2.0f);
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

		/*if (leftType == eColliderType::Circle2D) {
			SVector2D leftCirclePos = leftPos;

			if (fabs(leftCirclePos.mX - rightPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
				fabs(leftCirclePos.mY - rightPos.mY) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f)) {

				SVector2D leftTopVertex = SVector2D(rightPos.mX - rightSize.mX / 2.0f, rightPos.mY - rightSize.mY / 2.0f);
				SVector2D leftBottomVertex = SVector2D(rightPos.mX - rightSize.mX / 2.0f, rightPos.mY + rightSize.mY / 2.0f);
				SVector2D rightTopVertex = SVector2D(rightPos.mX + rightSize.mX / 2.0f, rightPos.mY - rightSize.mY / 2.0f);
				SVector2D rightBottomVertex = SVector2D(rightPos.mX + rightSize.mX / 2.0f, rightPos.mY + rightSize.mY / 2.0f);

				if (leftCirclePos.mX < leftTopVertex.mX && leftCirclePos.mY < leftTopVertex.mY) {
					if ((leftCirclePos - leftTopVertex).Length() < leftSize.mX / 2.0f) {
						return true;
					}
				}
				else if (leftCirclePos.mX < leftBottomVertex.mX && leftCirclePos.mY > leftBottomVertex.mY) {
					if ((leftCirclePos - leftBottomVertex).Length() < leftSize.mX / 2.0f) {
						return true;
					}
				}
				else if (leftCirclePos.mX > rightTopVertex.mX && leftCirclePos.mY < rightTopVertex.mY) {
					if ((leftCirclePos - rightTopVertex).Length() < leftSize.mX / 2.0f) {
						return true;
					}
				}
				else if (leftCirclePos.mX > rightBottomVertex.mX && leftCirclePos.mY > rightBottomVertex.mY) {
					if ((leftCirclePos - rightBottomVertex).Length() < leftSize.mX / 2.0f) {
						return true;
					}
				}
				else {
					return true;
				}
			}

		}
		else {
			SVector2D rightCirclePos = rightPos;

			if (fabs(rightCirclePos.mX - leftPos.mX) < fabs(rightSize.mX / 2.0f + leftSize.mX / 2.0f) &&
				fabs(rightCirclePos.mY - leftPos.mY) < fabs(rightSize.mY / 2.0f + leftSize.mY / 2.0f)) {

				SVector2D leftTopVertex = SVector2D(leftPos.mX - leftSize.mX / 2.0f, leftPos.mY - leftSize.mY / 2.0f);
				SVector2D leftBottomVertex = SVector2D(leftPos.mX - leftSize.mX / 2.0f, leftPos.mY + leftSize.mY / 2.0f);
				SVector2D rightTopVertex = SVector2D(leftPos.mX + leftSize.mX / 2.0f, leftPos.mY - leftSize.mY / 2.0f);
				SVector2D rightBottomVertex = SVector2D(leftPos.mX + leftSize.mX / 2.0f, leftPos.mY + leftSize.mY / 2.0f);

				if (rightCirclePos.mX < leftTopVertex.mX && rightCirclePos.mY < leftTopVertex.mY) {
					if ((rightCirclePos - leftTopVertex).Length() < rightSize.mX / 2.0f) {
						return true;
					}
				}
				else if (rightCirclePos.mX < leftBottomVertex.mX && rightCirclePos.mY > leftBottomVertex.mY) {
					if ((rightCirclePos - leftBottomVertex).Length() < rightSize.mX / 2.0f) {
						return true;
					}
				}
				else if (rightCirclePos.mX > rightTopVertex.mX && rightCirclePos.mY < rightTopVertex.mY) {
					if ((rightCirclePos - rightTopVertex).Length() < rightSize.mX / 2.0f) {
						return true;
					}
				}
				else if (rightCirclePos.mX > rightBottomVertex.mX && rightCirclePos.mY > rightBottomVertex.mY) {
					if ((rightCirclePos - rightBottomVertex).Length() < rightSize.mX / 2.0f) {
						return true;
					}
				}
				else {
					return true;
				}
			}
		}*/
	}
	return false;
}