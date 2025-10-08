#pragma once
#include "CEntity.h"
#include "Enums.h"
#include "SVector2D.h"
 
#include <windows.h>

class GameObject;

class CComponent : public CEntity
{
public:
	CComponent(eComponentType tType) : mType(tType) {}
	virtual ~CComponent() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float tDeltaTime) = 0;
	virtual void OnLateUpdate(float tDeltaTime) = 0;
	virtual void Render(HDC tHDC) = 0;

	void SetOwner(GameObject* tOwner) {
		mOwner = tOwner;
	}

	GameObject* GetOwner() {
		return this->mOwner;
	}

	eComponentType GetType() {
		return this->mType;
	}

private:
	GameObject* mOwner = nullptr;
	eComponentType mType;
};

