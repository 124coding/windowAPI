#include "CBabyAlien.h"

CBabyAlien::CBabyAlien(const CBabyAlien& tBabyAlien) : CEnemy(tBabyAlien)
{
}

CBabyAlien& CBabyAlien::operator=(const CBabyAlien& tBabyAlien) {
	if (this == &tBabyAlien)
	{
		return *this;
	}

	CEnemy::operator=(tBabyAlien);
}

void CBabyAlien::OnCreate()
{
	CEnemy::OnCreate();
}

void CBabyAlien::OnDestroy()
{
	CEnemy::OnDestroy();
}

void CBabyAlien::OnUpdate(float tDeltaTime)
{
	CEnemy::OnUpdate(tDeltaTime);
}

void CBabyAlien::OnLateUpdate(float tDeltaTime)
{
	CEnemy::OnLateUpdate(tDeltaTime);
}

void CBabyAlien::Render(HDC tHDC)
{
	CEnemy::Render(tHDC);
}
