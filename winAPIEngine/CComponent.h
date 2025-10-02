#pragma once
#include "CEntity.h"
#include <windows.h>

class GameObject;

class CComponent : public CEntity
{
public:
	CComponent() {}
	virtual ~CComponent() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float tDeltaTime) = 0;
	virtual void OnLateUpdate(float tDeltaTime) = 0;
	virtual void Render(HDC hDC) = 0;

	void SetOwner(GameObject* owner) {
		mOwner = owner;
	}

	GameObject* GetOwner() {
		return this->mOwner;
	}

private:
	GameObject* mOwner = nullptr;
};

