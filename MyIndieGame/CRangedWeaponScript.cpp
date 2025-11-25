#include "CRangedWeaponScript.h"

#include "CBullet.h"

#include "CSceneMgr.h"
#include "CResourceMgr.h"

#include "CCircleCollider2D.h"
#include "CBulletScript.h"

#include "Object.h"

void CRangedWeaponScript::OnCreate()
{
	CWeaponScript::OnCreate();
}

void CRangedWeaponScript::OnDestroy()
{
	CWeaponScript::OnDestroy();

	mBullet->OnDestroy();
	SAFE_DELETE(mBullet);
}

void CRangedWeaponScript::OnUpdate(float tDeltaTime)
{
	CWeaponScript::OnUpdate(tDeltaTime);

	mTotalTime += tDeltaTime;

	switch (mState) {
	case eState::Idle:
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		CalculatePosNextToTarget();
		CanAttackCheck(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		break;
	default:
		break;
	}
}

void CRangedWeaponScript::OnLateUpdate(float tDeltaTime)
{
	CWeaponScript::OnLateUpdate(tDeltaTime);
}

void CRangedWeaponScript::Render(HDC tHDC)
{
	CWeaponScript::Render(tHDC);
}

void CRangedWeaponScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
}

void CRangedWeaponScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CRangedWeaponScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}

void CRangedWeaponScript::CanAttackCheck(std::vector<GameObject*> tEnemies)
{
	if (tEnemies.empty()) {
		return;
	}

	if (GetClosedEnemyPos().mX == 9999.0f && GetClosedEnemyPos().mY == 9999.0f) {
		return;
	}

	SVector2D targetPos = ObjectCenterPos(GetTarget());
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	float distanceToEnemy = (targetPos - GetClosedEnemyPos()).Length();

	if (distanceToEnemy <= GetRange() && mTotalTime > GetDelay()) {
		mTotalTime = 0.0f;

		CTransform* blTr = mBullet->GetComponent<CTransform>();

		blTr->SetPos(tr->GetPos());
		blTr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());

		CBullet* bullet = new CBullet();
		*bullet = *mBullet;
		AddObjectToScene<CBullet>(bullet);
	};
}

void CRangedWeaponScript::SetBullet(SVector2D tSize, SVector2D tColliderSize, const std::wstring& tTextureName)
{
	mBullet = new CBullet();
	mBullet->SetLayerType(eLayerType::Bullet);

	CBulletScript* blSc = mBullet->GetComponent<CBulletScript>();

	blSc->SetDamage(GetDamage());

	CCircleCollider2D* cBlCollider = mBullet->AddComponent<CCircleCollider2D>();
	cBlCollider->SetSize(tColliderSize);

	CTexture* blImg = CResourceMgr::Find<CTexture>(tTextureName);

	CSpriteRenderer* blSr = mBullet->AddComponent<CSpriteRenderer>();
	blSr->SetTexture(blImg);

	mBullet->SetSize(tSize);
	mBullet->SetAnchorPoint(blImg->GetWidth() / 2, blImg->GetHeight() / 2);
}
