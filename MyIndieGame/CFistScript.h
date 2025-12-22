#pragma once

#include "CMeleeWeaponScript.h"

class CFistScript : public CMeleeWeaponScript
{
public:
	CFistScript() {}
	virtual ~CFistScript() {}

	virtual SDamageInfo GetFinalDamage() override;
private:
};

