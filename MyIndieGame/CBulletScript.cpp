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
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	if (tr->GetPos().mX < -tileSizeX || tr->GetPos().mX > mapWidth + tileSizeX
		|| tr->GetPos().mY < - tileSizeY * 2 || tr->GetPos().mY > mapHeight + tileSizeY) {
		ObjDestroy(GetOwner());
	}
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

CWeaponScript::SDamageInfo CBulletScript::GetFinalDamage(float tRangedDamage) {
	CWeaponScript* wpSc = mWeapon->GetComponent<CWeaponScript>();
	return wpSc->ApplyDamageModifiers(this->mDamage + tRangedDamage);
}

