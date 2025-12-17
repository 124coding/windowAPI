#pragma once

#include "CComponent.h"
#include "CWeapon.h"

class CWeaponMgr : public CComponent
{
public:
	CWeaponMgr() : CComponent(eComponentType::WeaponMgr) {
		PlusWeapon(eLayerType::MeleeWeapon, L"MW_001", 1);
		PlusWeapon(eLayerType::MeleeWeapon, L"MW_001", 1);
	};
	virtual ~CWeaponMgr() {};

	virtual CComponent* Clone() override {
		return new CWeaponMgr(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	bool PlusWeapon(eLayerType tType, std::wstring tWeaponId, int tWeaponTier);
	bool RemoveWeapon(int tIndex);

	void WeaponsPosition();

	std::vector<CWeapon*> GetWeapons() {
		return this->mWeapons;
	}

	void SetWeaponCount(int tCount) {
		this->mWeaponCount = tCount;
	}

	void ChangeWeaponTier(CWeapon* tWeapon, int tTier);

private:
	std::vector<CWeapon*> mWeapons;
	int mWeaponCount = 6;
};

