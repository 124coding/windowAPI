#include "GameObject.h"
#include "CTexture.h"

#include "winMacro.h"

GameObject::GameObject() : mState(eState::Active), mSize(SVector2D(1.0f, 1.0f)), mAnchorPoint(SVector2D()), mLayerType(eLayerType::None)
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

void GameObject::Render(HDC tHDC)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->Render(tHDC);
	}
}

void GameObject::InitializeComponent()
{
	AddComponent<CTransform>();
}
