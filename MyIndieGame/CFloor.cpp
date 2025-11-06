#include "CFloor.h"

CFloor::CFloor(const CFloor& tFloor) : GameObject(tFloor)
{
}

CFloor& CFloor::operator=(const CFloor& tFloor) {
	if (this == &tFloor)
	{
		return *this;
	}

	GameObject::operator=(tFloor);
}

void CFloor::OnCreate()
{
	GameObject::OnCreate();
}

void CFloor::OnDestroy()
{
	GameObject::OnDestroy();
}

void CFloor::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CFloor::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CFloor::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
