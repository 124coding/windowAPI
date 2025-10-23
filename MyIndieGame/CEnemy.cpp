#include "CEnemy.h"

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

void CEnemy::DecreaseHP(int tDecreaseAmount) {
	mHP = mHP - tDecreaseAmount;

	if (mHP < 0) {
		mHP = 0;
	}
}
