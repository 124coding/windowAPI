#include "CBulletScript.h"

#include "GameObject.h"
#include "Object.h"

void CBulletScript::OnCreate()
{
}

void CBulletScript::OnDestroy()
{
}

void CBulletScript::OnUpdate(float tDeltaTime)
{
}

void CBulletScript::OnLateUpdate(float tDeltaTime)
{
}

void CBulletScript::Render(HDC tHDC)
{
}

void CBulletScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	ObjDestroy(GetOwner());
}

void CBulletScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CBulletScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}
