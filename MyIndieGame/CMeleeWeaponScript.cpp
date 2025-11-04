#include "CMeleeWeaponScript.h"

#include "CSceneMgr.h"

void CMeleeWeaponScript::OnCreate()
{
	CWeaponScript::OnCreate();

	SetDamage(0.0f);
	SetDelay(1.0f); // 공격 간 딜레이 즉, 공격 속도
	SetRange(200.0f);
	SetSpeed(700.0f);
}

void CMeleeWeaponScript::OnDestroy()
{
	CWeaponScript::OnDestroy();
}

void CMeleeWeaponScript::OnUpdate(float tDeltaTime)
{
	CWeaponScript::OnUpdate(tDeltaTime);

	mTotalTime += tDeltaTime;

	switch (mState) {
	case eState::Idle:
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		SetFollowPlayer();
		CanAttackCheck();
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

void CMeleeWeaponScript::CanAttackCheck()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	float distanceToEnemy = (tr->GetPos() - GetClosedEnemyPos()).Length();

	if (distanceToEnemy <= GetRange() && mTotalTime > GetDelay()) {
		AttackStartPos = tr->GetPos();
		tr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());

		mState = eState::Attack;
	};
}

void CMeleeWeaponScript::AttackEndCheck()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	SVector2D rangeSize = (GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetRange();
	rangeSize.mX = fabs(rangeSize.mX);
	rangeSize.mY = fabs(rangeSize.mY);

	SVector2D currentRange = tr->GetPos() - AttackStartPos;
	currentRange.mX = fabs(currentRange.mX);
	currentRange.mY = fabs(currentRange.mY);

	if (currentRange.mX > rangeSize.mX && currentRange.mY > rangeSize.mY) {
		mState = eState::Back;
	}
}

void CMeleeWeaponScript::BackToPlayer()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetVelocity((CalculatePosNextToPlayer() - tr->GetPos()).Normalize() * GetSpeed());

	if ((tr->GetPos() - CalculatePosNextToPlayer()).Length() < 10.0f) {
		tr->SetPos(CalculatePosNextToPlayer());
		mTotalTime = 0.0f;
		mState = eState::Idle;
	}
}
