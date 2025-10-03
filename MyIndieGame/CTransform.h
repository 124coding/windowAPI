#pragma once
#include "CComponent.h"

#include <Windows.h>

class CTransform : public CComponent
{
public:
	CTransform() : CComponent(eComponentType::Transform), mPosition(SVector2D(0.0f, 0.0f)), mVelocity(SVector2D(0.0f, 0.0f)), mRadius(0.0f) {}
	~CTransform() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;

	void SetPos(SVector2D tPosition) {
		this->mPosition.mX = tPosition.mX;
		this->mPosition.mY = tPosition.mY;
	}

	SVector2D GetPos() {
		return this->mPosition;
	}

	void SetVelocity(const SVector2D& tVelocity) {
		this->mVelocity = tVelocity;
	}

	SVector2D GetVelocity() {
		return this->mVelocity;
	}

private:
	SVector2D mPosition;
	SVector2D mVelocity;

	float mRadius;
};

