#pragma once
#include "CEntity.h"
#include "Enums.h"
#include "SVector2D.h"
 
#include <windows.h>

class GameObject;

class CComponent : public CEntity
{
public:
	// 모든 컴포넌트(기능)들의 최상위 부모 클래스
	// GameObject는 여러 개의 CComponent를 소유하는 컨테이너 역할을 함
	CComponent(eComponentType tType) : mActive(true), mType(tType) {}	
	virtual ~CComponent() {}

	virtual CComponent* Clone() = 0;

	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnUpdate(float tDeltaTime) {};
	virtual void OnLateUpdate(float tDeltaTime) {};
	virtual void Render(HDC tHDC) {};

	// 이 컴포넌트를 소유한 게임 오브젝트 설정
	void SetOwner(GameObject* tOwner) {
		mOwner = tOwner;
	}

	GameObject* GetOwner() {
		return this->mOwner;
	}

	eComponentType GetType() {
		return this->mType;
	}

	// 컴포넌트 활성화/비활성화 (비활성 시 Update, Render가 호출되지 않음)
	void SetActive(bool tActive) {
		this->mActive = tActive;
	}

	bool IsActive() {
		return this->mActive;
	}

private:
	bool mActive;						// 활성화 여부
	GameObject* mOwner = nullptr;		// 나를 가지고 있는 주인(GameObject)의 포인터
	eComponentType mType;				// 컴포넌트 타입 (Transform, Collider, Script 등)
};

