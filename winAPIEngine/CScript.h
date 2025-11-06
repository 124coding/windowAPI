#pragma once

#include "CComponent.h"

class CCollider;

class CScript : public CComponent
{
public:
	CScript() : CComponent(eComponentType::Script) {}
	virtual ~CScript() {}

	virtual CComponent* Clone() override {
		return new CScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);

	virtual void Bounce();

	void SetBobbingSpeed(float tBobbingSpeed) {
		this->mBobbingSpeed = tBobbingSpeed;
	}

	void SetSquashMagnitude(float tSquashMagnitude) {
		this->mSquashMagnitude = tSquashMagnitude;
	}
	
protected:
	float mTotalTime = 0.0f;

private:
	float mBobbingSpeed;
	float mSquashMagnitude;
};

