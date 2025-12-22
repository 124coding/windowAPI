#pragma once

#include "CRangedWeaponScript.h"

class CMedicalGunScript : public CRangedWeaponScript
{
public:
	CMedicalGunScript() {}
	virtual ~CMedicalGunScript() {}

	virtual float GetDamage() override;
private:
};