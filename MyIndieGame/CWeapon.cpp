#include "CWeapon.h"

CWeapon::CWeapon(const CWeapon& tWeapon) : GameObject(tWeapon)
{
}

CWeapon& CWeapon::operator=(const CWeapon& tWeapon) {
	if (this == &tWeapon)
	{
		return *this;
	}

	GameObject::operator=(tWeapon);
}

void CWeapon::OnCreate()
{
	GameObject::OnCreate();
}

void CWeapon::OnDestroy()
{
	GameObject::OnDestroy();
}

void CWeapon::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CWeapon::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CWeapon::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
