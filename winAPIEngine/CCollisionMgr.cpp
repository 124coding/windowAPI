#define NOMINMAX
#include "CCollisionMgr.h"

#include "GameObject.h"

#include "CSceneMgr.h"

#include "Object.h"

std::bitset <(UINT)eLayerType::MAX> CCollisionMgr::mCollisionLayerMtrix[(UINT)eLayerType::MAX] = {};
std::unordered_map<UINT64, bool> CCollisionMgr::mCollisionMap = {};

void CCollisionMgr::OnCreate(){

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

		CCollider* leftCollider = left->GetComponent<CCollider>(eComponentType::Collider);
		if (leftCollider == nullptr) continue;
		
		for (GameObject* right : rights) {
			if (right->IsActive() == false) continue;

			CCollider* rightCollider = right->GetComponent<CCollider>(eComponentType::Collider);
			if (rightCollider == nullptr) continue;

			if (left == right) {
				continue;
			}

			ColliderCollision(tDeltaTime, leftCollider, rightCollider);
		}
	}
}

void CCollisionMgr::ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight) {
	
	if (!tLeft->GetActivate() || !tRight->GetActivate()) {
		return;
	}

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
	CTransform* leftTr = tLeft->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CTransform* rightTr = tRight->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	SVector2D leftSize = ObjectSize(tLeft->GetOwner()) * tLeft->GetSize();
	SVector2D rightSize = ObjectSize(tRight->GetOwner()) * tRight->GetSize();

	SVector2D leftCenterPos = ObjectCenterPos(tLeft->GetOwner());
	SVector2D rightCenterPos = ObjectCenterPos(tRight->GetOwner()); // SVector2D(rightPos.mX - tRight->GetOwner()->GetAnchorPoint().mX * tRight->GetSize().mX * tRight->GetOwner()->GetSize().mX * rightTr->GetScale().mX + rightSize.mX, rightPos.mY - tRight->GetOwner()->GetAnchorPoint().mY * tRight->GetSize().mY * tRight->GetOwner()->GetSize().mY * rightTr->GetScale().mY + rightSize.mY);

	float distanceSq = (leftCenterPos - rightCenterPos).LengthSq();

	eColliderType leftType = tLeft->GetColliderType();
	eColliderType rightType = tRight->GetColliderType();

	// 각 오브젝트의 중심을 기준으로
	if (leftTr->GetRot() == 0.0f && rightTr->GetRot() == 0.0f) {
		if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			// rect - rect
			if (fabs(leftCenterPos.mX - rightCenterPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
				fabs(leftCenterPos.mY - rightCenterPos.mY) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f)) {

				return true;
			}
		}
		else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {// circle - circle

			if (distanceSq <= (leftSize.mX / 2.0f + rightSize.mX / 2.0f) * (leftSize.mX / 2.0f + rightSize.mX / 2.0f)) {
				return true;
			}
		}
		else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Circle2D ||
			leftType == eColliderType::Circle2D && rightType == eColliderType::Rect2D) { // circle - rect

			SVector2D circlePos, rectPos, rectHalfSize;
			float circleRadius;

			if (leftType == eColliderType::Circle2D) {
				circlePos = leftCenterPos;
				circleRadius = leftSize.mX / 2.0f;
				rectPos = rightCenterPos;
				rectHalfSize = rightSize / 2.0f;
			}
			else {
				circlePos = rightCenterPos;
				circleRadius = rightSize.mX / 2.0f;
				rectPos = leftCenterPos;
				rectHalfSize = leftSize / 2.0f;
			}

			// 클램핑(Clamping) 기법 이용 충돌 코드
			SVector2D rectMin = rectPos - rectHalfSize;
			SVector2D rectMax = rectPos + rectHalfSize;

			float closestX = std::max(rectMin.mX, std::min(circlePos.mX, rectMax.mX));
			float closestY = std::max(rectMin.mY, std::min(circlePos.mY, rectMax.mY));

			SVector2D closestPoint(closestX, closestY);

			float distanceSquared = (circlePos - closestPoint).LengthSq();
			float radiusSquared = circleRadius * circleRadius;

			if (distanceSquared < radiusSquared) {
				return true;
			}
		}
	}
	else {
		if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {
			float radiusSum = (leftSize.mX / 2.0f + rightSize.mX / 2.0f);
			if (distanceSq <= radiusSum * radiusSum) {
				return true;
			}
		}
		else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			SVector2D dist = leftCenterPos.GetDistanceVector(rightCenterPos);

			SVector2D vec[4] = { leftSize.GetWidthVector(leftTr->GetRot()), leftSize.GetHeightVector(leftTr->GetRot()), rightSize.GetWidthVector(rightTr->GetRot()), rightSize.GetHeightVector(rightTr->GetRot()) };

			for (int i = 0; i < 4; i++) {
				float sum = 0.0f;
				SVector2D unit = vec[i].Normalize();

				for (int j = 0; j < 4; j++) {
					sum += (vec[j].Dot(unit) > 0) ? vec[j].Dot(unit) : -vec[j].Dot(unit);
				}

				float dot = (dist.Dot(unit) > 0) ? dist.Dot(unit) : -dist.Dot(unit);

				if (dot > sum) {
					return false;
				}
			}
			return true;
		}
		else {
			CCollider* rectCol = (leftType == eColliderType::Rect2D) ? tLeft : tRight;
			CCollider* circleCol = (leftType == eColliderType::Circle2D) ? tLeft : tRight;

			SVector2D rectPos = ObjectCenterPos(rectCol->GetOwner());
			SVector2D circlePos = ObjectCenterPos(circleCol->GetOwner());

			SVector2D rectHalfSize = (ObjectSize(rectCol->GetOwner()) * rectCol->GetSize()) / 2.0f;
			float circleRadius = (ObjectSize(circleCol->GetOwner()) * circleCol->GetSize()).mX / 2.0f;

			float rectRot = rectCol->GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetRot();

			SVector2D dir = circlePos - rectPos;

			SVector2D rectRight = rectCol->GetSize().GetWidthVector(rectRot).Normalize();
			SVector2D rectUp = rectCol->GetSize().GetHeightVector(rectRot).Normalize();  

			float localX = dir.Dot(rectRight);
			float localY = dir.Dot(rectUp);

			float closestX = std::max(-rectHalfSize.mX, std::min(localX, rectHalfSize.mX));
			float closestY = std::max(-rectHalfSize.mY, std::min(localY, rectHalfSize.mY));

			float distanceX = localX - closestX;
			float distanceY = localY - closestY;

			float distSq = (distanceX * distanceX) + (distanceY * distanceY);

			if (distSq <= (circleRadius * circleRadius)) {
				return true;
			}
		}
	}

	return false;
}