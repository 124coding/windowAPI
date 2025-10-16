#include "CTile.h"

void CTile::OnCreate()
{
	GameObject::OnCreate();
}

void CTile::OnDestroy() {
	GameObject::OnDestroy();
}

void CTile::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CTile::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CTile::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
