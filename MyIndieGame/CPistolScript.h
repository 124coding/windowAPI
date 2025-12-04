#pragma once

#include "CRangedWeaponScript.h"

class CPistolScript : public CRangedWeaponScript
{
public:
	CPistolScript() {}
	virtual ~CPistolScript() {}

	virtual float GetDamage() override;
private:
};