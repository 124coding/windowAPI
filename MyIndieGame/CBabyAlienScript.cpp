#include "CBabyAlienScript.h"

#include "CBabyAlien.h"

void CBabyAlienScript::OnCreate()
{
}

void CBabyAlienScript::OnDestroy()
{
}

void CBabyAlienScript::OnUpdate(float tDeltaTime)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Translate(tr);
}

void CBabyAlienScript::OnLateUpdate(float tDeltaTime)
{
}

void CBabyAlienScript::Render(HDC tHDC)
{
}

void CBabyAlienScript::OnCollisionEnter(CCollider* tOther)
{
	CEnemyScript::OnCollisionEnter(tOther);
}

void CBabyAlienScript::OnCollisionStay(CCollider* tOther)
{
	CEnemyScript::OnCollisionStay(tOther);
}

void CBabyAlienScript::OnCollisionExit(CCollider* tOther)
{
	CEnemyScript::OnCollisionExit(tOther);
}

void CBabyAlienScript::Idle()
{
}

void CBabyAlienScript::Move(float tDeltaTime)
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
