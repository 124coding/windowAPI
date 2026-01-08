#pragma once

#include "framework.h"
#include "CTexture.h"
#include "SVector2D.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CSpriteRenderer.h"

#include <vector>

#include "winMacro.h"


class CTexture;

class GameObject : public CEntity
{
public:
	// 외부 유틸리티 함수(ObjDestroy)가 private 멤버인 Death()를 호출할 수 있게 허용
	friend void ObjDestroy(GameObject* tObj);

	enum class eState {
		Active,
		Paused,
		Dead,
		End
	};

	GameObject();
	virtual ~GameObject();

	GameObject(const GameObject& tObj);
	GameObject& operator=(const GameObject& tObj);


	// ==========================================
	// Lifecycle Methods (생명주기)
	// ==========================================
	virtual void OnCreate();						// 객체 생성 직후 1회 호출
	virtual void OnDestroy();						// 객체 소멸 직전 1회 호출
	virtual void OnUpdate(float tDeltaTime);		// 매 프레임 로직 처리
	virtual void OnLateUpdate(float tDeltaTime);	// Update 이후 후처리 (카메라, 충돌 보정 등)
	virtual void Render(HDC tHDC);					// 렌더링 처리

	// ==========================================
	// Component System (컴포넌트 관리)
	// ==========================================

	// 컴포넌트 추가: 동적 할당 후 소유자를 설정하고 컨테이너에 보관
	template <typename T>
	T* AddComponent() {
		T* comp = new T();
		comp->SetOwner(this);

		// 컴포넌트 타입(Enum)을 인덱스로 사용하여 배열/벡터에 저장
		mComponents[(UINT)comp->GetType()] = comp;

		return comp;
	}

	// 컴포넌트 제거: 메모리 해제 및 포인터 초기화
	template <typename T>
	void RemoveComponent(eComponentType type) {
		int index = (int)type;

		if (mComponents[index] != nullptr) {
			mComponents[index]->OnDestroy(); // 삭제 전 정리 작업 수행
			SAFE_DELETE(mComponents[index]);
			mComponents[index] = nullptr;
		}
	}

	// 컴포넌트 가져오기: 저장된 기반 클래스 포인터를 원래 타입으로 캐스팅하여 반환
	template <typename T>
	T* GetComponent(eComponentType type) {
		if (mComponents[(UINT)type] != nullptr)
			return static_cast<T*>(mComponents[(UINT)type]);
		return nullptr;
	}

public:
	void SetState(bool tPower) {
		if (tPower) {
			mState = eState::Active;
		}
		else {
			mState = eState::Paused;
		}
	}

	eState GetState() {
		return this->mState;
	}

	bool IsActive() {
		return this->mState == eState::Active;
	}

	bool IsDead() {
		return this->mState == eState::Dead;
	}

	void SetSize(SVector2D tSize) {
		this->mSize = SVector2D(this->mSize.mX * tSize.mX, this->mSize.mY * tSize.mY);
	}

	SVector2D GetSize() {
		return this->mSize;
	}

	void SetAnchorPoint(float tWidth, float tHeight) {
		this->mAnchorPoint = SVector2D(tWidth, tHeight);
	}

	SVector2D GetAnchorPoint() {
		return this->mAnchorPoint;
	}

	void SetLayerType(eLayerType tLayerType) {
		this->mLayerType = tLayerType;
	}

	eLayerType GetLayerType() {
		return this->mLayerType;
	}

private:
	void InitializeComponent();

	// 객체를 즉시 삭제하지 않고 상태만 'Dead'로 변경 (지연 삭제 처리용)
	void Death() {
		this->mState = eState::Dead;
	}

private:
	eState mState;

	SVector2D mSize;
	SVector2D mAnchorPoint;

	std::vector<CComponent*> mComponents;
	eLayerType mLayerType;
};