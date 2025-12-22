#pragma once

#include "CMeleeWeaponScript.h"

class CSpearScript : public CMeleeWeaponScript
{
public:
	CSpearScript() {}
	virtual ~CSpearScript() {}

	virtual SDamageInfo GetFinalDamage() override;
private:
};

