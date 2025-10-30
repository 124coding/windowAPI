#include "CMeleeWeaponScript.h"

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
