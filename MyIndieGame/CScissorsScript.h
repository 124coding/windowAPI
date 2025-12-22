#pragma once

#include "CMeleeWeaponScript.h"

class CScissorsScript : public CMeleeWeaponScript
{
public:
	CScissorsScript() {}
	virtual ~CScissorsScript() {}

	virtual SDamageInfo GetFinalDamage() override;
private:
};

