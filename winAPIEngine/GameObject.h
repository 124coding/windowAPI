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
	friend void ObjDestroy(GameObject* tObj);

	enum class eState {
		Active,
		Paused,
		Dead,
		End
	};

	GameObject();
	virtual ~GameObject();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);

	template <typename T>
	T* AddComponent() {
		T* comp = new T();
		comp->SetOwner(this);
		comp->OnCreate();

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

	void SetAnchorPoint(float tTextureWidth, float tTextureHeight, SVector2D tScale, SVector2D tSize, SVector2D tOffset) {
		this->mAnchorPoint = SVector2D((tTextureWidth * tScale.mX * tSize.mX) / 2 + tOffset.mX, tTextureHeight * tScale.mY * tSize.mY + tOffset.mY);
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
	void Death() {
		this->mState = eState::Dead;
	}

private:
	eState mState;
	CTexture* mTexture = nullptr;

	SVector2D mSize;
	SVector2D mAnchorPoint;

	std::vector<CComponent*> mComponents;
	eLayerType mLayerType;
};