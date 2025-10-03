#include "GameObject.h"
#include "CTexture.h"

#include "winMacro.h"

GameObject::GameObject()
{
	mComponents.resize((UINT)eComponentType::End);
	InitializeComponent();
}

GameObject::~GameObject()
{

}

void GameObject::OnCreate()
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnCreate();
	}
}

void GameObject::OnDestroy()
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnDestroy();
		SAFE_DELETE(comp);
	}

	SAFE_DELETE(mTexture);
}

void GameObject::OnUpdate(float tDeltaTime)
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnUpdate(tDeltaTime);
	}
}

void GameObject::OnLateUpdate(float tDeltaTime)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnLateUpdate(tDeltaTime);
	}
}

void GameObject::Render(HDC hDC)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->Render(hDC);
	}
}

void GameObject::InitializeComponent()
{
	AddComponent<CTransform>();
	AddComponent<CSpriteRenderer>();
}
