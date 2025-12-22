#pragma once

#include "CRangedWeaponScript.h"

class CSMGScript : public CRangedWeaponScript
{
public:
	CSMGScript() {}
	virtual ~CSMGScript() {}

	virtual float GetDamage() override;
private:
};