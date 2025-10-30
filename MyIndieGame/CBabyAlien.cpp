#include "CBabyAlien.h"

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
