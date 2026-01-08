#include "CMeleeWeaponScript.h"

#include "Object.h"

#include "CSceneMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CPlayerScript.h"

#include "CCollider.h"

void CMeleeWeaponScript::OnCreate()
{
	CWeaponScript::OnCreate();
}

void CMeleeWeaponScript::OnDestroy()
{
	CWeaponScript::OnDestroy();
}

void CMeleeWeaponScript::OnUpdate(float tDeltaTime)
{
	CWeaponScript::OnUpdate(tDeltaTime);

	// 쿨타임 계산을 위한 시간 누적
	mTotalTime += tDeltaTime;

	// 상태에 따른 행동 분기
	switch (mState) {
	case eState::Idle:
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		CalculatePosNextToTarget();
		CanAttackCheck(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		break;
	case eState::Attack:
		AttackEndCheck();
		break;
	case eState::Back:
		BackToPlayer();
		break;
	default:
		break;
	}
}

void CMeleeWeaponScript::OnLateUpdate(float tDeltaTime)
{
	CWeaponScript::OnLateUpdate(tDeltaTime);
}

void CMeleeWeaponScript::Render(HDC tHDC)
{
	CWeaponScript::Render(tHDC);
}

void CMeleeWeaponScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	CWeaponScript::OnCollisionEnter(tDeltaTime, tOther);
}

void CMeleeWeaponScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
	CWeaponScript::OnCollisionStay(tDeltaTime, tOther);
}

void CMeleeWeaponScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
	CWeaponScript::OnCollisionExit(tDeltaTime, tOther);
}

// [Transition: Idle -> Attack] 공격 조건 확인 및 시작
void CMeleeWeaponScript::CanAttackCheck(std::vector<GameObject*> tEnemies)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float range = plSc->GetRange();
	int attackSpeed = plSc->GetAttackSpeedPercent();

	if (tEnemies.empty()) {
		return;
	}

	// 타겟팅된 적이 없으면 리턴 (SetRotForClosedEnemyWatch에서 설정됨)
	if (GetClosedEnemyPos().mX == 9999.0f && GetClosedEnemyPos().mY == 9999.0f) {
		return;
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CCollider* cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);

	// 적과의 거리 계산
	float distanceToEnemy = (ObjectCenterPos(GetOwner()) - GetClosedEnemyPos()).Length();

	// 공격 조건: 1. 사거리 내 진입, 2. 쿨타임 종료 (공격 속도 공식 적용)
	if (distanceToEnemy <= GetRange() + range / 2 && mTotalTime > GetDelay() / (1 + attackSpeed / 100.0f)) {
		// [Physics] 적 방향으로 속도(Velocity)를 주어 찌르는 모션 구현
		tr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());

		// [Optimization] 공격 순간에만 충돌체를 활성화하여 불필요한 연산 방지
		cl->SetActivate(true);

		// 상태 전이: Idle -> Attack
		mState = eState::Attack;
	};
}

// [Transition: Attack -> Back] 공격 범위 이탈 체크
void CMeleeWeaponScript::AttackEndCheck()
{
	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 최대 사거리 벡터 계산
	SVector2D rangeSize = (GetClosedEnemyPos() - targetPos).Normalize() * GetRange();
	rangeSize.mX = fabs(rangeSize.mX);
	rangeSize.mY = fabs(rangeSize.mY);

	// 현재 무기가 이동한 거리 (플레이어 기준)
	SVector2D currentRange = tr->GetPos() - targetPos;
	currentRange.mX = fabs(currentRange.mX);
	currentRange.mY = fabs(currentRange.mY);

	// [Optimization] sqrt 연산을 피하기 위해 LengthSq(거리 제곱)로 비교
	if (currentRange.LengthSq() > rangeSize.LengthSq()) {
		mState = eState::Back;		// 사거리를 벗어나면 복귀 상태로 전환
	}
}

// [Transition: Back -> Idle] 원위치 복귀 로직
void CMeleeWeaponScript::BackToPlayer()
{
	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CCollider* cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);

	// 돌아가야 할 목표 위치 (플레이어 위치 + 오프셋)
	SVector2D initialPos = targetPos + GetOffset();

	// 목표 위치를 향해 이동 벡터 설정
	tr->SetVelocity((initialPos - tr->GetPos()).Normalize() * GetSpeed());

	// [Snap Logic] 목표 지점에 매우 근접했을 때 (10.0f 미만)
	if ((tr->GetPos() - initialPos).Length() < 10.0f) {
		tr->SetPos(initialPos);		// 위치를 강제로 고정 (떨림 방지)
		mTotalTime = 0.0f;			// 쿨타임 초기화
		cl->SetActivate(false);		// 공격 판정 비활성화

		mState = eState::Idle;		// 대기 상태로 전환
	}
}
