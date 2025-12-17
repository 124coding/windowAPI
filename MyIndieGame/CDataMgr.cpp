#include "CDataMgr.h"

#include "CBabyAlienScript.h"

#include "CMeleeWeaponScript.h"
#include "CRangedWeaponScript.h"

#include "CDaggerScript.h"

std::unordered_map<std::wstring, CDataMgr::SMonster> CDataMgr::mMonsterStats;
std::unordered_map<std::wstring, CDataMgr::SWeapon> CDataMgr::mWeaponDatas;
std::unordered_map<std::wstring, CDataMgr::SCharacter> CDataMgr::mCharacterDatas;
std::unordered_map<std::wstring, CDataMgr::SEffect> CDataMgr::mEffectDatas;
std::unordered_map<std::wstring, CDataMgr::SItem> CDataMgr::mItemDatas;


std::unordered_map<std::wstring, std::function<CEnemy* ()>> CDataMgr::mMonsterCreator;
std::unordered_map<std::wstring, std::function<CWeapon* ()>> CDataMgr::mWeaponCreator;


void CDataMgr::OnCreate() {
	LoadDatas();

	// Monsters
	MonsterRegister<CBabyAlienScript>(L"BabyAlien");

	// MeleeWeapons
	WeaponRegister<CDaggerScript>(L"MW_001", eLayerType::MeleeWeapon);

	// RangedWeapons
	WeaponRegister<CRangedWeaponScript>(L"RW_001", eLayerType::RangedWeapon);
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
	json monsterStats;
	json weaponDatas;
	json characterDatas;
	json effectDatas;
	json itemDatas;

	std::ifstream monsterFile("../Data/MonsterStats.json", std::ios::in);
	monsterFile >> monsterStats;

	for (auto& monsterStat : monsterStats["Monsters"]) {
		SMonster monster;

		monster.ID = ToWString(monsterStat["M_ID"]);
		monster.name = ToWString(monsterStat["Name"]);
		monster.hp = monsterStat["HP"];
		monster.hpIncreasedEachWave = monsterStat["HP_increase_each_wave"];
		monster.damage = monsterStat["Damage"];
		monster.damageIncreaseEachWave = monsterStat["Damage_increase_each_wave"];
		monster.speed = monsterStat["Speed"];
		monster.sizeX = monsterStat["SizeX"];
		monster.sizeY = monsterStat["SizeY"];
		monster.collisionSizeX = monsterStat["CollisionSizeX"];
		monster.collisionSizeY = monsterStat["CollisionSizeY"];
		monster.minNumber = monsterStat["min_number"];
		monster.maxNumber = monsterStat["max_number"];
		monster.minDistanceFromPlayer = monsterStat["min_distance_from_player"];

		mMonsterStats.insert({ monster.ID, monster });
	}

	std::ifstream weaponFile("../Data/Weapons.json", std::ios::in);
	weaponFile >> weaponDatas;

	SWeapon weapon;

	for (auto& weaponData : weaponDatas["MeleeWeapons"]) {
		weapon.weaponType = L"근거리";
		weapon.ID = ToWString(weaponData["W_ID"]);
		weapon.classType = ToWString(weaponData["Class"]);
		weapon.name = ToWString(weaponData["Name"]);
		weapon.iconTexture = ToWString(weaponData["IconTexture"]);
		weapon.speed = weaponData["Speed"];
		weapon.sizeX = weaponData["SizeX"];
		weapon.sizeY = weaponData["SizeY"];
		weapon.collisionSizeX = weaponData["CollisionSizeX"];
		weapon.collisionSizeY = weaponData["CollisionSizeY"];

		int i = 0;
		for (const auto& t : weaponData["Tiers"]) {
			weapon.tier[i].tier = t["Tier"];
			weapon.tier[i].damage = t["Damage"];
			weapon.tier[i].critDamagePer = t["CriticalDamage"];
			weapon.tier[i].critChancePer = t["CriticalChance"];
			weapon.tier[i].delay = t["Delay"];
			weapon.tier[i].range = t["Range"];
			weapon.tier[i++].basePrice = t["BasePrice"];
		}
		weapon.specialEffect = ToWString(weaponData["SpecialEffect"]);
		mWeaponDatas.insert({ weapon.ID, weapon });
	}
	for (auto& weaponData : weaponDatas["RangedWeapons"]) {
		weapon.weaponType = L"원거리";
		weapon.ID = ToWString(weaponData["W_ID"]);
		weapon.classType = ToWString(weaponData["Class"]);
		weapon.name = ToWString(weaponData["Name"]);
		weapon.iconTexture = ToWString(weaponData["IconTexture"]);
		weapon.speed = weaponData["Speed"];
		weapon.sizeX = weaponData["SizeX"];
		weapon.sizeY = weaponData["SizeY"];
		weapon.collisionSizeX = weaponData["CollisionSizeX"];
		weapon.collisionSizeY = weaponData["CollisionSizeY"];

		weapon.bulletName = ToWString(weaponData["BulletName"]);
		weapon.bulletSizeX = weaponData["BulletSizeX"];
		weapon.bulletSizeY = weaponData["BulletSizeY"];

		int i = 0;
		for (const auto& t : weaponData["Tiers"]) {
			weapon.tier[i].tier = t["Tier"];
			weapon.tier[i].damage = t["Damage"];
			weapon.tier[i].critDamagePer = t["CriticalDamage"];
			weapon.tier[i].critChancePer = t["CriticalChance"];
			weapon.tier[i].delay = t["Delay"];
			weapon.tier[i].range = t["Range"];
			weapon.tier[i].lifeSteal = t["LifeSteal"];
			weapon.tier[i++].basePrice = t["BasePrice"];
		}

		weapon.specialEffect = ToWString(weaponData["SpecialEffect"]);
		mWeaponDatas.insert({ weapon.ID, weapon });
	}

	std::ifstream characterFile("../Data/Characters.json", std::ios::in);
	characterFile >> characterDatas;

	for (auto& characterData : characterDatas["Characters"]) {
		SCharacter character;

		character.ID = ToWString(characterData["C_ID"]);
		character.name = ToWString(characterData["Name"]);
		character.hairTexture = ToWString(characterData["HairTexture"]);
		character.eyesTexture = ToWString(characterData["EyesTexture"]);
		character.mouthTexture = ToWString(characterData["MouthTexture"]);
		character.clothTexture = ToWString(characterData["ClothTexture"]);
		character.iconTexture = ToWString(characterData["IconTexture"]);

		for (auto& effect : characterData["Effects"]) {

			std::wstring effectID = ToWString(effect["E_ID"]);
			std::vector<SArg> effectArgs;

			for (auto& arg : effect["Args"]) {
				SArg effectArg;
				effectArg.value = ToWString(arg["Value"]);
				effectArg.color = ToWString(arg["Color"]);

				effectArgs.push_back(effectArg);
			}

			character.effects.insert({effectID, effectArgs});
		}

		for (auto& weapon : characterData["Weapons"]) {
			character.weapons.push_back(ToWString(weapon));
		}

		mCharacterDatas.insert({ character.ID, character });
	}

	std::ifstream effectFile("../Data/Effects.json", std::ios::in);
	effectFile >> effectDatas;

	for (auto& effectData : effectDatas["Effects"]) {
		SEffect effect;
		
		effect.ID = ToWString(effectData["E_ID"]);
		effect.name = ToWString(effectData["Name"]);
		effect.description = ToWString(effectData["Description"]);

		mEffectDatas.insert({ effect.ID, effect });
	}

	std::ifstream itemFile("../Data/Items.json", std::ios::in);
	itemFile >> itemDatas;

	for (auto& itemData : itemDatas["Items"]) {
		SItem item;

		item.ID = ToWString(itemData["I_ID"]);
		item.name = ToWString(itemData["Name"]);
		item.tier = itemData["Tier"];
		item.pos = ToWString(itemData["Pos"]);
		item.basePrice = itemData["Base_Price"];

		for (auto& effect : itemData["Effects"]) {

			if (effect.contains("comment")) {
				break;
			}

			std::wstring effectID = ToWString(effect["E_ID"]);
			std::vector<SArg> effectArgs;

			for (auto& arg : effect["Args"]) {
				SArg effectArg;
				effectArg.value = ToWString(arg["Value"]);
				effectArg.color = ToWString(arg["Color"]);

				effectArgs.push_back(effectArg);
			}

			item.effects.insert({ effectID, effectArgs });
		}

		mItemDatas.insert({ item.ID, item });
	}
}