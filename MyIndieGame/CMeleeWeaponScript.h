#pragma once

#include "CWeaponScript.h"

class CMeleeWeaponScript : public CWeaponScript
{
public:
	CMeleeWeaponScript() {}
	virtual ~CMeleeWeaponScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

private:
};

