#include "CBabyAlien.h"

#include "CBabyAlienScript.h"

void CBabyAlien::OnCreate()
{
	GameObject::OnCreate();
	// stage prolportional 하게 하기.
	SetHP(2);
	SetButtDamage(1);

	SetSpeed(50.0f);

	this->AddComponent<CBabyAlienScript>();
}

void CBabyAlien::OnDestroy()
{
	GameObject::OnDestroy();
}

void CBabyAlien::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CBabyAlien::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CBabyAlien::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
