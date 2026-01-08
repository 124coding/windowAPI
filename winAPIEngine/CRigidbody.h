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

	// 외부에서 오브젝트에 힘을 가함 (이동, 점프, 넉백 등)
	// AddForce로 힘을 누적시키면, OnUpdate에서 이를 기반으로 이동을 처리함
	void AddForce(SVector2D tForce) {
		this->mForce = this->mForce + tForce;
	}

	// 땅에 닿아있는지 여부 설정 (점프 로직 등에서 필수)
	void SetGround(bool tGround) {
		this->mbGround = tGround;
	}

private:
	bool mbGround;					// 땅 착지 여부 (true면 중력 가속도를 0으로 만들거나 속도 리셋)
	float mMass;					// 질량
	float mFriction;				// 마찰 계수

	SVector2D mForce;				// 누적된 힘
	SVector2D mAccelation;			// 가속도
	SVector2D mLimitedVelocity;		// 속도 제한
	SVector2D mGravity;				// 중력값
};

