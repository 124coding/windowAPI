#pragma once

#include "CScript.h"

#include "CWeaponScript.h"

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
	void SetWeapon(GameObject* tWeapon) {
		this->mWeapon = tWeapon;
	}

	void SetDamage(float tDamage) {
		this->mDamage = tDamage;
	}

	CWeaponScript::SDamageInfo GetFinalDamage();
private:
	GameObject* mWeapon;
	float mDamage;
};

