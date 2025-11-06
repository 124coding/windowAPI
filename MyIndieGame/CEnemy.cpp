#include "CEnemy.h"

CEnemy::CEnemy(const CEnemy& tEnemy) : GameObject(tEnemy)
{
}

CEnemy& CEnemy::operator=(const CEnemy& tEnemy) {
	if (this == &tEnemy)
	{
		return *this;
	}

	GameObject::operator=(tEnemy);
}

void CEnemy::OnCreate()
{
	GameObject::OnCreate();
}

void CEnemy::OnDestroy()
{
	GameObject::OnDestroy();
}

void CEnemy::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CEnemy::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CEnemy::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
