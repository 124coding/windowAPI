#pragma once

#include "CScript.h"

class CTitleObjectScript : public CScript
{
public:

	CTitleObjectScript() {}
	virtual ~CTitleObjectScript() {}

	virtual CComponent* Clone() override {
		return new CTitleObjectScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

private:
	SVector2D mRegularVelocity = SVector2D(0.0f, 0.0f);
	float mTotalTime = 0.0f;
	float mSpeed = 1.0f;
	float mSquashMagnitude = 0.2f;
};

