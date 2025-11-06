#pragma once

#include "CWeaponScript.h"

class CBullet;

class CRangedWeaponScript : public CWeaponScript
{
public:
	CRangedWeaponScript() : mBullet(nullptr) {}
	virtual ~CRangedWeaponScript() {}

	virtual CComponent* Clone() override {
		return new CRangedWeaponScript(*this);
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
	void CanAttackCheck(std::vector<GameObject*> tEnemies);

public:
	CBullet* GetBullet() {
		return this->mBullet;
	}

private:
	CBullet* mBullet;
};

