#pragma once

#include "CScript.h"

class CTitleObjectScript : public CScript
{
public:

	CTitleObjectScript() {}
	~CTitleObjectScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetSwing(float tSwing) {
		this->mSwing = tSwing;
	}

private:
	SVector2D mRegularVelocity = SVector2D(0.0f, 0.0f);
	float mRadian = 0.0f;
	float mSwing = 1.0f;
};

