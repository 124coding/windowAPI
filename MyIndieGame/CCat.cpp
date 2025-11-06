#include "CCat.h"

CCat::CCat(const CCat& tCat) : GameObject(tCat)
{
}

CCat& CCat::operator=(const CCat& tCat) {
	if (this == &tCat)
	{
		return *this;
	}

	GameObject::operator=(tCat);
}

void CCat::OnCreate()
{
	GameObject::OnCreate();
}

void CCat::OnDestroy()
{
	GameObject::OnDestroy();
}

void CCat::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CCat::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CCat::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
