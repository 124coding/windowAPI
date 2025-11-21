#pragma once

#include "CWeapon.h"
#include "CTexture.h"

#include "CScript.h"

class CPlayerWeaponMgr : public CScript
{
public:
	bool PlusWeapon(CWeapon* tWeapon) {
		if (mWeapons.size() < mWeaponCount) {
			mWeapons.push_back(tWeapon);
			return true;
		}

		return false;
	}

	std::vector<CWeapon*> GetWeapons() {
		return this->mWeapons;
	}

	void SetWeaponCount(int tCount) {
		this->mWeaponCount = tCount;
	}

private:
	std::vector<CWeapon*> mWeapons;
	int mWeaponCount = 6;
};

