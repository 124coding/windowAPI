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

void CCollider::OnCollisionEnter(CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionEnter(tOther);
}

void CCollider::OnCollisionStay(CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionStay(tOther);
}

void CCollider::OnCollisionExit(CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>();
	script->OnCollisionExit(tOther);
}
