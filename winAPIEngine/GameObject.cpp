#include "GameObject.h"
#include "CTexture.h"

#include "winMacro.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::OnCreate()
{

	for (CComponent* comp : mComponents) {
		comp->OnCreate();
	}
}

void GameObject::OnDestroy()
{

	for (CComponent* comp : mComponents) {
		comp->OnDestroy();
	}

	SAFE_DELETE(mTexture);
}

void GameObject::OnUpdate(float tDeltaTime)
{

	for (CComponent* comp : mComponents) {
		comp->OnUpdate(tDeltaTime);
	}
}

void GameObject::OnLateUpdate(float tDeltaTime)
{
	for (CComponent* comp : mComponents) {
		comp->OnLateUpdate(tDeltaTime);
	}
}

void GameObject::Render(HDC hDC)
{
	for (CComponent* comp : mComponents) {
		comp->Render(hDC);
	}
}
