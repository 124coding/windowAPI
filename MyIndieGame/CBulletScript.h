#pragma once

#include "CScript.h"

class CBulletScript : public CScript
{
public:
	CBulletScript() : CScript() {}
	virtual ~CBulletScript() {}

	virtual CComponent* Clone() override {
		return new CBulletScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

public:
	void SetDamage(float tDamage) {
		this->mDamage = tDamage;
	}

	float GetDamage() {
		return this->mDamage;
	}
private:
	float mDamage;
};

