#pragma once

#include "CScript.h"

class CTitleBrotato : public CScript
{
public:

	CTitleBrotato() {}
	virtual ~CTitleBrotato() {}

	virtual CComponent* Clone() override {
		return new CTitleBrotato(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

