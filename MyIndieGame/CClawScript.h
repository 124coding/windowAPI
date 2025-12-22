#pragma once

#include "CMeleeWeaponScript.h"

class CClawScript : public CMeleeWeaponScript
{
public:
	CClawScript() {}
	virtual ~CClawScript() {}

	virtual SDamageInfo GetFinalDamage() override;
private:
};

