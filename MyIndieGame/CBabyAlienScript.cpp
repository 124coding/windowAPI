#include "CBabyAlienScript.h"

#include "CBabyAlien.h"

void CBabyAlienScript::OnCreate()
{
	CEnemyScript::OnCreate();
}

void CBabyAlienScript::OnDestroy()
{
	CEnemyScript::OnDestroy();
}

void CBabyAlienScript::OnUpdate(float tDeltaTime)
{
	CEnemyScript::OnUpdate(tDeltaTime);

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
		
		currentVelocity = targetTr->GetPos() - tr->GetPos();

		if (currentVelocity.Length() > 0.0f) {
			currentVelocity = currentVelocity.Normalize();
		}

		tr->SetVelocity(currentVelocity * 50.0f/*dynamic_cast<CBabyAlien*>(GetOwner())->GetSpeed()*/);
	}
}
