#pragma once

#include "CComponent.h"

class CRigidbody : public CComponent
{
public:
	CRigidbody() :
		CComponent(eComponentType::Rigidbody),
		mbGround(false),
		mMass(1.0f),
		mFriction(10.0f),
		mForce(SVector2D()),
		mAccelation(SVector2D()),
		mLimitedVelocity(SVector2D(200.0f, 1000.0f)),
		mGravity(SVector2D(0.0f, 800.0f))
	{}

	virtual ~CRigidbody() {}

	virtual CComponent* Clone() override {
		return new CRigidbody(*this);
	}

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

	void SetGround(bool tGround) {
		this->mbGround = tGround;
	}

private:
	bool mbGround;
	float mMass;
	float mFriction;

	SVector2D mForce;
	SVector2D mAccelation;
	SVector2D mLimitedVelocity;
	SVector2D mGravity;
};

