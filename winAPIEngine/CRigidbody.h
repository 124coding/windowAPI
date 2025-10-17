#pragma once

#include "CComponent.h"

class CRigidbody : public CComponent
{
public:
	CRigidbody() : 
		CComponent(eComponentType::Rigidbody),
		mMass(1.0f),
		mFriction(10.0f),
		mForce(SVector2D()),
		mAccelation(SVector2D()),
		mGravity(SVector2D())
	{}

	virtual ~CRigidbody() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetMass(float tMass) {
		this->mMass = tMass;
	}

	void AddForce(SVector2D tForce) {
		this->mForce = this->mForce + tForce;
	}

private:
	float mMass;
	float mFriction;

	SVector2D mForce;
	SVector2D mAccelation;
	SVector2D mGravity;
};

