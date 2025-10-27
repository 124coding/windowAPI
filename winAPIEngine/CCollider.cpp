#include "CCollider.h"

#include "GameObject.h"

#include "CScript.h"

UINT32 CCollider::mCollisionID = 1;
void CCollider::OnCreate()
{
}

void CCollider::OnDestroy()
{
}

void CCollider::OnUpdate(float tDeltaTime)
{
}

void CCollider::OnLateUpdate(float tDeltaTime)
{
}

void CCollider::Render(HDC tHDC)
{
}

void CCollider::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionEnter(tDeltaTime, tOther);
}

void CCollider::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionStay(tDeltaTime, tOther);
}

void CCollider::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionExit(tDeltaTime, tOther);
}
