#include "CDataMgr.h"

#include "CBabyAlienScript.h"

#include "CMeleeWeaponScript.h"
#include "CRangedWeaponScript.h"

std::unordered_map<std::string, CDataMgr::SMonster> CDataMgr::mMonsterStats;
std::unordered_map<std::string, CDataMgr::SWeapon> CDataMgr::mWeaponDatas;
std::unordered_map<std::string, CDataMgr::SCharacter> CDataMgr::mCharacterDatas;
std::unordered_map<std::string, CDataMgr::SEffect> CDataMgr::mEffectDatas;


std::unordered_map<std::string, std::function<CEnemy* ()>> CDataMgr::mMonsterCreator;
std::unordered_map<std::string, std::function<CWeapon* ()>> CDataMgr::mWeaponCreator;


void CDataMgr::OnCreate() {
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
	json monsterStats;
	json weaponDatas;
	json characterDatas;
	json effectDatas;

	std::ifstream monsterFile("../Data/MonsterStats.json", std::ios::in);
	monsterFile >> monsterStats;

	for (auto& monsterStat : monsterStats["Monsters"]) {
		SMonster monster;

		monster.ID = monsterStat["M_ID"];
		monster.name = monsterStat["Name"];
		monster.hp = monsterStat["HP"];
		monster.buttDamage = monsterStat["ButtDamage"];
		monster.attackDamage = monsterStat["AttackDamage"];
		monster.speed = monsterStat["Speed"];
		monster.sizeX = monsterStat["SizeX"];
		monster.sizeY = monsterStat["SizeY"];
		monster.collisionSizeX = monsterStat["CollisionSizeX"];
		monster.collisionSizeY = monsterStat["CollisionSizeY"];

		mMonsterStats.insert({ monster.ID, monster });
	}

	std::ifstream weaponFile("../Data/Weapons.json", std::ios::in);
	weaponFile >> weaponDatas;

	for (auto& weaponType : weaponDatas["Monsters"]) {
		SWeapon weapon;

		for (auto& weaponData : weaponType["MeleeWeapons"]) {
			weapon.ID = weaponData["W_ID"];
			weapon.name = weaponData["Name"];
			weapon.range = weaponData["Range"];
			weapon.speed = weaponData["Speed"];
			weapon.sizeX = weaponData["SizeX"];
			weapon.sizeY = weaponData["SizeY"];
			weapon.collisionSizeX = weaponData["CollisionSizeX"];
			weapon.collisionSizeY = weaponData["CollisionSizeY"];

			int i = 0;
			for (const auto& t : weaponData["Tiers"]) {
				weapon.tier[i].tier = t["Tier"];
				weapon.tier[i].damage = t["Damage"];
				weapon.tier[i++].delay = t["Delay"];
			}
		}
		for (auto& weaponData : weaponType["RangedWeapons"]) {

			weapon.ID = weaponData["W_ID"];
			weapon.name = weaponData["Name"];
			weapon.range = weaponData["Range"];
			weapon.speed = weaponData["Speed"];
			weapon.sizeX = weaponData["SizeX"];
			weapon.sizeY = weaponData["SizeY"];
			weapon.collisionSizeX = weaponData["CollisionSizeX"];
			weapon.collisionSizeY = weaponData["CollisionSizeY"];

			weapon.bulletName = weaponData["BulletName"];
			weapon.bulletSizeX = weaponData["BulletSizeX"];
			weapon.bulletSizeY = weaponData["BulletSizeY"];

			int i = 0;
			for (const auto& t : weaponData["Tiers"]) {
				weapon.tier[i].tier = t["Tier"];
				weapon.tier[i].damage = t["Damage"];
				weapon.tier[i++].delay = t["Delay"];
			}
		}

		mWeaponDatas.insert({ weapon.ID, weapon });
	}

	std::ifstream characterFile("../Data/Characters.json", std::ios::in);
	characterFile >> characterDatas;

	for (auto& characterData : characterDatas["Characters"]) {
		SCharacter character;

		character.ID = characterData["C_ID"];
		character.name = characterData["Name"];
		character.hairTexture = characterData["HairTexture"];
		character.eyesTexture = characterData["EyesTexture"];
		character.mouthTexture = characterData["MouthTexture"];
		character.clothTexture = characterData["ClothTexture"];
		character.iconTexture = characterData["IconTexture"];

		for (auto& effect : characterData["Effects"]) {

			std::string effectID = effect["E_ID"];
			std::vector<SArg> effectArgs;

			for (auto& arg : effect["Args"]) {
				SArg effectArg;
				effectArg.value = arg["Value"];
				effectArg.color = arg["Color"];

				effectArgs.push_back(effectArg);
			}

			character.effects.insert({effectID, effectArgs});
		}

		for (auto& weapon : characterData["Weapons"]) {
			character.weapons.push_back(weapon);
		}

		mCharacterDatas.insert({ character.ID, character });
	}

	std::ifstream effectFile("../Data/Effects.json", std::ios::in);
	effectFile >> effectDatas;

	for (auto& effectData : effectDatas["Effects"]) {
		SEffect effect;
		
		effect.ID = effectData["E_ID"];
		effect.name = effectData["Name"];
		effect.description = effectData["Description"];

		mEffectDatas.insert({ effect.ID, effect });
	}
}