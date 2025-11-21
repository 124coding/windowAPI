#include "CMeleeWeaponScript.h"

#include "Object.h"

#include "CSceneMgr.h"

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

	mTotalTime += tDeltaTime;

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

void CMeleeWeaponScript::CanAttackCheck(std::vector<GameObject*> tEnemies)
{
	if (tEnemies.empty()) {
		return;
	}

	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CCollider* cl = GetOwner()->GetComponent<CCollider>();

	float distanceToEnemy = (targetPos - GetClosedEnemyPos()).Length();

	if (distanceToEnemy <= GetRange() && mTotalTime > GetDelay()) {
		tr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());
		cl->SetActivate(true);

		mState = eState::Attack;
	};
}

void CMeleeWeaponScript::AttackEndCheck()
{
	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	SVector2D rangeSize = (GetClosedEnemyPos() - targetPos).Normalize() * GetRange();
	rangeSize.mX = fabs(rangeSize.mX);
	rangeSize.mY = fabs(rangeSize.mY);

	SVector2D currentRange = tr->GetPos() - targetPos;
	currentRange.mX = fabs(currentRange.mX);
	currentRange.mY = fabs(currentRange.mY);

	if (currentRange.mX > rangeSize.mX && currentRange.mY > rangeSize.mY) {
		mState = eState::Back;
	}
}

void CMeleeWeaponScript::BackToPlayer()
{
	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CCollider* cl = GetOwner()->GetComponent<CCollider>();

	SVector2D initialPos = targetPos + GetOffset();

	tr->SetVelocity((initialPos - tr->GetPos()).Normalize() * GetSpeed());

	if ((tr->GetPos() - initialPos).Length() < 10.0f) {
		tr->SetPos(initialPos);
		mTotalTime = 0.0f;
		cl->SetActivate(true);

		mState = eState::Idle;
	}
}
