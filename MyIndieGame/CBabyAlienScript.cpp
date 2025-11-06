#include "CBabyAlienScript.h"

#include "CBabyAlien.h"

#include "CResourceMgr.h"

#include "CCollider.h"

void CBabyAlienScript::OnCreate()
{
	CEnemyScript::OnCreate();

	// stage prolportional 하게 하기.

	SetHP(2);
	SetButtDamage(1);

	SetSpeed(50.0f);
}

void CBabyAlienScript::OnDestroy()
{
	CEnemyScript::OnDestroy();
}

void CBabyAlienScript::OnUpdate(float tDeltaTime)
{
	CEnemyScript::OnUpdate(tDeltaTime);

	if (mTextureChangeDelay > 0) {
		mTextureChangeDelay -= tDeltaTime;
	}

	if (mTextureChangeDelay < 0) {
		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>();
		thisSr->SetTexture(CResourceMgr::Find<CTexture>(L"BabyAlien"));
		mTextureChangeDelay = 0.0f;
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Translate(tr);
}

void CBabyAlienScript::OnLateUpdate(float tDeltaTime)
{
	CEnemyScript::OnLateUpdate(tDeltaTime);
}

void CBabyAlienScript::Render(HDC tHDC)
{
	CEnemyScript::Render(tHDC);
}

void CBabyAlienScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	CEnemyScript::OnCollisionEnter(tDeltaTime, tOther);

	if (tOther->GetOwner()->GetLayerType() == eLayerType::MeleeWeapon || tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>();
		thisSr->SetTexture(CResourceMgr::Find<CTexture>(L"BabyAlienCollsion"));
	}
}

void CBabyAlienScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
	CEnemyScript::OnCollisionStay(tDeltaTime, tOther);
}

void CBabyAlienScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
	CEnemyScript::OnCollisionExit(tDeltaTime, tOther);
}

void CBabyAlienScript::Idle()
{
}

void CBabyAlienScript::Translate(CTransform* tr)
{
	if (GetTarget() == nullptr) {
		return;
	}
	else {
		CTransform* targetTr = GetTarget()->GetComponent<CTransform>();

		SVector2D currentVelocity = SVector2D();
		
		currentVelocity = targetTr->GetPos() + GetTarget()->GetComponent<CCollider>()->GetOffset() - tr->GetPos();

		if (currentVelocity.Length() > 0.0f) {
			currentVelocity = currentVelocity.Normalize();
		}

		tr->SetVelocity(currentVelocity * 50.0f/*dynamic_cast<CBabyAlien*>(GetOwner())->GetSpeed()*/);
	}
}
