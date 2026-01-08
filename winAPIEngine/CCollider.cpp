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

// 물리 엔진(CollisionMgr) -> 충돌체(Collider) -> 스크립트(Script) 순서로 호출됨
// 즉, Collider는 단순히 충돌 사실을 로직 담당자인 Script에게 전달하는 역할
void CCollider::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>(eComponentType::Script);
	if (script) script->OnCollisionEnter(tDeltaTime, tOther);
}

void CCollider::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>(eComponentType::Script);
	if (script) script->OnCollisionStay(tDeltaTime, tOther);
}

void CCollider::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
	CScript* script = GetOwner()->GetComponent<CScript>(eComponentType::Script);
	if (script) script->OnCollisionExit(tDeltaTime, tOther);
}
