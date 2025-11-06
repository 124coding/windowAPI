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

GameObject::GameObject(const GameObject& tObj) : 
	mState(tObj.mState), 
	mTexture(tObj.mTexture), 
	mSize(tObj.mSize), 
	mAnchorPoint(tObj.mAnchorPoint), 
	mLayerType(tObj.mLayerType) 
{
	mComponents.resize((UINT)eComponentType::End);

	for (CComponent* tObjComponent : tObj.mComponents) {
		if (tObjComponent == nullptr) continue;
		CComponent* newComp = tObjComponent->Clone();
		newComp->SetOwner(this);

		mComponents[(UINT)newComp->GetType()] = newComp;
	}
}

GameObject& GameObject::operator=(const GameObject& tObj) {
	if (this == &tObj)
	{
		return *this;
	}

	mState = tObj.mState;
	mTexture = tObj.mTexture;
	mSize = tObj.mSize;
	mAnchorPoint = tObj.mAnchorPoint;
	mLayerType = tObj.mLayerType;

	for (CComponent* pComp : mComponents)
	{
		if (pComp == nullptr) continue;
		pComp->OnDestroy();
		SAFE_DELETE(pComp);
	}

	for (CComponent* tObjComponent : tObj.mComponents) {
		if (tObjComponent == nullptr) continue;
		CComponent* newComp = tObjComponent->Clone();
		newComp->SetOwner(this);

		mComponents[(UINT)newComp->GetType()] = newComp;
	}

	return *this;
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

	mComponents.clear();
}

void GameObject::OnUpdate(float tDeltaTime)
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->OnUpdate(tDeltaTime);
	}
}

void GameObject::OnLateUpdate(float tDeltaTime)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->OnLateUpdate(tDeltaTime);
	}
}

void GameObject::Render(HDC tHDC)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->Render(tHDC);
	}
}

void GameObject::InitializeComponent()
{
	AddComponent<CTransform>();
}
