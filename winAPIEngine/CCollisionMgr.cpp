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

	// 충돌 매트릭스를 순회하며, 충돌 검사가 활성화된 레이어 쌍에 대해서만 연산 수행
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

	// 매트릭스의 중복 검사를 방지하기 위해 작은 값을 행으로 설정 (Symmetric)
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
	// 두 레이어의 모든 오브젝트 리스트를 가져옴
	const std::vector<GameObject*>& lefts = CSceneMgr::GetGameObjects(tLeft);
	const std::vector<GameObject*>& rights = CSceneMgr::GetGameObjects(tRight);

	// 이중 루프를 통해 모든 조합(Pair) 검사
	for (GameObject* left : lefts) {
		if (left->IsActive() == false) continue;

		CCollider* leftCollider = left->GetComponent<CCollider>(eComponentType::Collider);
		if (leftCollider == nullptr) continue;
		
		for (GameObject* right : rights) {
			if (right->IsActive() == false) continue;

			CCollider* rightCollider = right->GetComponent<CCollider>(eComponentType::Collider);
			if (rightCollider == nullptr) continue;

			// 자기 자신과의 충돌 제외
			if (left == right) {
				continue;
			}

			ColliderCollision(tDeltaTime, leftCollider, rightCollider);
		}
	}
}

void CCollisionMgr::ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight) {
	
	// 둘 중 하나라도 비활성화된 충돌체라면 패스
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

	// 교차 검사 (실제 기하학적 충돌 판정)
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

// 기하학적 충돌 연산 (Math)
bool CCollisionMgr::Intersect(CCollider* tLeft, CCollider* tRight)
{
	CTransform* leftTr = tLeft->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CTransform* rightTr = tRight->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	SVector2D leftSize = ObjectSize(tLeft->GetOwner()) * tLeft->GetSize();
	SVector2D rightSize = ObjectSize(tRight->GetOwner()) * tRight->GetSize();

	SVector2D leftCenterPos = ObjectCenterPos(tLeft->GetOwner());
	SVector2D rightCenterPos = ObjectCenterPos(tRight->GetOwner()); 

	float distanceSq = (leftCenterPos - rightCenterPos).LengthSq();

	eColliderType leftType = tLeft->GetColliderType();
	eColliderType rightType = tRight->GetColliderType();

	// ==========================================
	// CASE 1: 회전이 없는 경우 (AABB vs AABB, Circle vs Circle) - 연산이 빠름
	// ==========================================
	if (leftTr->GetRot() == 0.0f && rightTr->GetRot() == 0.0f) {
		if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			// rect - rect
			// 두 중심점의 거리 차이가 (두 변의 절반의 합)보다 작으면 충돌
			if (fabs(leftCenterPos.mX - rightCenterPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
				fabs(leftCenterPos.mY - rightCenterPos.mY) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f)) {

				return true;
			}
		}
		else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {// circle - circle
			// 중심 거리^2 <= (반지름 합)^2
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

			// 사각형 영역 내에서 원의 중심과 가장 가까운 점(Closest Point)을 찾음
			SVector2D rectMin = rectPos - rectHalfSize;
			SVector2D rectMax = rectPos + rectHalfSize;

			// 원의 중심 좌표를 사각형의 최소/최대 좌표 사이로 자름(Clamp)
			float closestX = std::max(rectMin.mX, std::min(circlePos.mX, rectMax.mX));
			float closestY = std::max(rectMin.mY, std::min(circlePos.mY, rectMax.mY));

			SVector2D closestPoint(closestX, closestY);

			// 가장 가까운 점과 원의 중심 사이의 거리가 반지름보다 작으면 충돌
			float distanceSquared = (circlePos - closestPoint).LengthSq();
			float radiusSquared = circleRadius * circleRadius;

			if (distanceSquared < radiusSquared) {
				return true;
			}
		}
	}
	// ==========================================
	// CASE 2: 회전이 있는 경우 (OBB) - 분리축 이론(SAT) 등 복잡한 연산 필요
	// ==========================================
	else {
		if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {
			float radiusSum = (leftSize.mX / 2.0f + rightSize.mX / 2.0f);
			if (distanceSq <= radiusSum * radiusSum) {
				return true;
			}
		}
		else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			// OBB vs OBB - 분리축 이론(SAT)
			SVector2D dist = leftCenterPos.GetDistanceVector(rightCenterPos);

			// 검사해야 할 4개의 축 (왼쪽 사각형의 가로/세로축, 오른쪽 사각형의 가로/세로축)
			SVector2D vec[4] = { leftSize.GetWidthVector(leftTr->GetRot()), leftSize.GetHeightVector(leftTr->GetRot()), rightSize.GetWidthVector(rightTr->GetRot()), rightSize.GetHeightVector(rightTr->GetRot()) };

			for (int i = 0; i < 4; i++) {
				float sum = 0.0f;
				SVector2D unit = vec[i].Normalize();	// 단위 벡터(축)

				// 4개의 반지름 투영 길이 합 계산
				for (int j = 0; j < 4; j++) {
					sum += (vec[j].Dot(unit) > 0) ? vec[j].Dot(unit) : -vec[j].Dot(unit);
				}

				// 두 중심점 거리의 투영 길이 계산
				float dot = (dist.Dot(unit) > 0) ? dist.Dot(unit) : -dist.Dot(unit);

				// 하나라도 분리축이 발견되면(거리가 반지름 합보다 크면) 충돌 아님
				if (dot > sum) {
					return false;
				}
			}
			return true;	// 모든 축에서 겹침 -> 충돌
		}
		// OBB vs circle - 좌표계 변환 방식
		else {
			CCollider* rectCol = (leftType == eColliderType::Rect2D) ? tLeft : tRight;
			CCollider* circleCol = (leftType == eColliderType::Circle2D) ? tLeft : tRight;

			SVector2D rectPos = ObjectCenterPos(rectCol->GetOwner());
			SVector2D circlePos = ObjectCenterPos(circleCol->GetOwner());

			SVector2D rectHalfSize = (ObjectSize(rectCol->GetOwner()) * rectCol->GetSize()) / 2.0f;
			float circleRadius = (ObjectSize(circleCol->GetOwner()) * circleCol->GetSize()).mX / 2.0f;

			float rectRot = rectCol->GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetRot();

			// 원의 위치를 사각형의 로컬 좌표계로 변환 (사각형을 회전 안 한 상태로 간주하기 위해)
			SVector2D dir = circlePos - rectPos;

			// 사각형의 로컬 축 구하기
			SVector2D rectRight = rectCol->GetSize().GetWidthVector(rectRot).Normalize();
			SVector2D rectUp = rectCol->GetSize().GetHeightVector(rectRot).Normalize();  

			// 내적(Dot)을 통해 사각형 로컬 축에 투영된 거리(Local X, Y) 구하기
			float localX = dir.Dot(rectRight);
			float localY = dir.Dot(rectUp);

			// AABB vs Circle과 동일하게 클램핑 수행
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