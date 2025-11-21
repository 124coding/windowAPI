#include "CDataMgr.h"

#include "CBabyAlienScript.h"

#include "CMeleeWeaponScript.h"
#include "CRangedWeaponScript.h"

json CDataMgr::mMonsterStats = nullptr;
json CDataMgr::mWeaponDatas = nullptr;
std::unordered_map<std::string, std::function<CEnemy* ()>> CDataMgr::mMonsterCreator;
std::unordered_map<std::string, std::function<CWeapon* ()>> CDataMgr::mWeaponCreator;


void CDataMgr::OnCreate(CAPIEngine* tEngine) {
	LoadDatas();

	// Monsters
	MonsterRegister<CBabyAlienScript>("BabyAlien");

	// MeleeWeapons
	WeaponRegister<CMeleeWeaponScript>("Dagger", eLayerType::MeleeWeapon);

	// RangedWeapons
	WeaponRegister<CRangedWeaponScript>("Pistol", eLayerType::RangedWeapon);
}

void CDataMgr::OnDestroy() {
}

void CDataMgr::OnUpdate(float tDeltaTime) {
}

void CDataMgr::OnLateUpdate(float tDeltaTime) {
}

void CDataMgr::Render(HDC tHDC) {
}

void CDataMgr::LoadDatas() {
	std::ifstream monsterFile("../Data/MonsterStats.json", std::ios::in);
	monsterFile >> mMonsterStats;

	std::ifstream weaponFile("../Data/Weapons.json", std::ios::in);
	weaponFile >> mWeaponDatas;
}