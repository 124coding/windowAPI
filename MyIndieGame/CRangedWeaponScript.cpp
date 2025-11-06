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

	SetDamage(1.0f);
	SetDelay(1.0f); // 공격 간 딜레이 즉, 공격 속도
	SetRange(400.0f);
	SetSpeed(700.0f);

	mBullet = new CBullet();
	mBullet->SetLayerType(eLayerType::Bullet);

	mBullet->AddComponent<CCircleCollider2D>();
	CBulletScript* blSc = mBullet->GetComponent<CBulletScript>();

	blSc->SetDamage(GetDamage());

	CTexture* blImg = CResourceMgr::Find<CTexture>(L"PlayerBullet");

	CSpriteRenderer* blSr = mBullet->AddComponent<CSpriteRenderer>();
	blSr->SetTexture(blImg);
	
	mBullet->SetSize(SVector2D(0.3f, 0.3f));
	mBullet->SetAnchorPoint(blImg->GetWidth() / 2, blImg->GetHeight() / 2);
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
		SetFollowPlayer();
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

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	float distanceToEnemy = (tr->GetPos() - GetClosedEnemyPos()).Length();

	if (distanceToEnemy <= GetRange() && mTotalTime > GetDelay()) {
		mTotalTime = 0.0f;

		CTransform* blTr = mBullet->GetComponent<CTransform>();

		blTr->SetPos(tr->GetPos());
		blTr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());

		CBullet* bullet = new CBullet();
		*bullet = *mBullet;
		AddObjectToScene<CBullet>(bullet);
		int a = 0;
	};
}