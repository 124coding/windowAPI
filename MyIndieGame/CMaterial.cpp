#include "CMaterial.h"

CMaterial::CMaterial(const CMaterial& tObj)
{
}

CMaterial& CMaterial::operator=(const CMaterial& tObj)
{
	if (this == &tObj)
	{
		return *this;
	}

	GameObject::operator=(tObj);
}

void CMaterial::OnCreate()
{
	GameObject::OnCreate();
}

void CMaterial::OnDestroy()
{
	GameObject::OnDestroy();
}

void CMaterial::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CMaterial::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CMaterial::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
