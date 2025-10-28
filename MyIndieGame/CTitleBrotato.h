#pragma once

#include "CScript.h"

class CTitleBrotato : public CScript
{
public:

	CTitleBrotato() {}
	~CTitleBrotato() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	float mTotalTime = 0.0f;
	float mBobbingSpeed = 1.0f;
	float mSquashMagnitude = 0.1f;
};

