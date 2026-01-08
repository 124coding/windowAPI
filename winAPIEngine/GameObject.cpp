#include "GameObject.h"
#include "CTexture.h"

#include "winMacro.h"

GameObject::GameObject() : mState(eState::Active), mSize(SVector2D(1.0f, 1.0f)), mAnchorPoint(SVector2D()), mLayerType(eLayerType::None)
{
	// 컴포넌트 벡터 크기를 미리 확보 (인덱스 접근을 위해)
	mComponents.resize((UINT)eComponentType::End);
	InitializeComponent();
}

GameObject::~GameObject()
{

}

GameObject::GameObject(const GameObject& tObj) : 
	mState(tObj.mState), 
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
	// 보유한 모든 컴포넌트의 초기화(OnCreate) 호출
	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnCreate();
	}
}

void GameObject::OnDestroy()
{
	// 객체 소멸 시 컴포넌트들도 함께 정리 및 메모리 해제
	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnDestroy();
		SAFE_DELETE(comp);
	}

	mComponents.clear();
}

void GameObject::OnUpdate(float tDeltaTime)
{
	// 활성화된 컴포넌트만 업데이트 수행
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
	// 렌더링도 컴포넌트(주로 Renderer 계열)에게 위임
	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->Render(tHDC);
	}
}

void GameObject::InitializeComponent()
{
	// 모든 게임 오브젝트는 기본적으로 위치/크기 정보(Transform)를 가짐
	AddComponent<CTransform>();
}
