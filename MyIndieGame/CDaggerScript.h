#pragma once

#include "CMeleeWeaponScript.h"

class CDaggerScript : public CMeleeWeaponScript
{
public:
	CDaggerScript() {}
	virtual ~CDaggerScript() {}

	virtual SDamageInfo GetFinalDamage() override;
private:
};

