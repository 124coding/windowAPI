#pragma once

#include "CComponent.h"
#include "CWeapon.h"

class CWeaponMgr : public CComponent
{
public:
	CWeaponMgr() : CComponent(eComponentType::WeaponMgr) {};
	virtual ~CWeaponMgr() {};

	virtual CComponent* Clone() override {
		return new CWeaponMgr(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	bool PlusWeapon(eLayerType tType, std::string tWeaponId, int tWeaponTier);

	void WeaponsPosition();

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

