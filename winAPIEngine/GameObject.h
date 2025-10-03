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

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC hDC);

	template <typename T>
	T* AddComponent() {
		T* comp = new T();

		comp->OnCreate();
		comp->SetOwner(this);

		mComponents[(UINT)comp->GetType()] = comp;

		return comp;
	}

	template <typename T>
	T* GetComponent() {
		T* component = nullptr;
		for (CComponent* comp : mComponents) {
			component = dynamic_cast<T*>(comp);
			if (component) break;
		}

		return component;
	}

private:
	void InitializeComponent();

private:

	CTexture* mTexture = nullptr;

	std::vector<CComponent*> mComponents;
};