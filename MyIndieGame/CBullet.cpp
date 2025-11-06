#include "CBullet.h"

#include "CBulletScript.h"

CBullet::CBullet(const CBullet& tBullet) : CWeapon(tBullet)
{
}

CBullet& CBullet::operator=(const CBullet& tBullet) {
	if (this == &tBullet)
	{
		return *this;
	}

	CWeapon::operator=(tBullet);
}

void CBullet::OnCreate()
{
	CWeapon::OnCreate();
}

void CBullet::OnDestroy()
{
	CWeapon::OnDestroy();
}

void CBullet::OnUpdate(float tDeltaTime)
{
	CWeapon::OnUpdate(tDeltaTime);
}

void CBullet::OnLateUpdate(float tDeltaTime)
{
	CWeapon::OnLateUpdate(tDeltaTime);
}

void CBullet::Render(HDC tHDC)
{
	CWeapon::Render(tHDC);
}
