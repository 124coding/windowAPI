#pragma once
#include "CEntity.h"
#include "Enums.h"
#include "SVector2D.h"
 
#include <windows.h>

class GameObject;

class CComponent : public CEntity
{
public:
	CComponent(eComponentType tType) : mActive(true), mType(tType) {}	
	virtual ~CComponent() {}

	virtual CComponent* Clone() = 0;

	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnUpdate(float tDeltaTime) {};
	virtual void OnLateUpdate(float tDeltaTime) {};
	virtual void Render(HDC tHDC) {};

	void SetOwner(GameObject* tOwner) {
		mOwner = tOwner;
	}

	GameObject* GetOwner() {
		return this->mOwner;
	}

	eComponentType GetType() {
		return this->mType;
	}

	void SetActive(bool tActive) {
		this->mActive = tActive;
	}

	bool IsActive() {
		return this->mActive;
	}

private:
	bool mActive;
	GameObject* mOwner = nullptr;
	eComponentType mType;
};

