#include "CWeapon.h"

#include "CPlayer.h"

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
