#include "CBabyAlien.h"

void CBabyAlien::OnCreate()
{
	CEnemy::OnCreate();
	// stage prolportional 하게 하기.
	SetHP(2);
	SetButtDamage(1);

	SetSpeed(50.0f);
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
