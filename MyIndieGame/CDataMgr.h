#pragma once

#include "Object.h"

#include "CEnemy.h"
#include "CWeapon.h"

#include "json.hpp"

#include <string>

#include <iostream>
#include <fstream>

using json = nlohmann::json;

class CDataMgr
{
public:
	struct SMonster {
		// 각 몬스터가 가지는 값들을 저장하기 위한 구조체
		std::string ID = "";
		std::string name = "";
		int hp = 0;
		int buttDamage = 0;
		int attackDamage = 0;
		float speed = 0.0f;
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		float collisionSizeX = 0.0f;
		float collisionSizeY = 0.0f;
	};

	struct SWeaponTier {
		int tier = 0;
		float damage = 0.0f;
		float delay = 0.0f;
	};

	struct SWeapon {
		std::string ID = "";
		std::string name = "";
		std::string bulletName = "";
		float range = 0.0f;
		float speed = 0.0f;
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		float bulletSizeX = 0.0f;
		float bulletSizeY = 0.0f;
		float collisionSizeX = 0.0f;
		float collisionSizeY = 0.0f;
		SWeaponTier tier[4];
	};

	struct SCharacter {
		std::string ID = "";
		std::string name = "";
		std::string hairTexture = "";
		std::string eyesTexture = "";
		std::string mouthTexture = "";
		std::string clothTexture = "";
		std::vector<std::string> Weapons;
	};

	struct SEffect {
		std::string ID = "";
		std::string name = "";
		std::string description = "";
	};

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static std::wstring ToWString(const std::string& tStr) {
		if (tStr.empty()) return std::wstring();

		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &tStr[0], (int)tStr.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &tStr[0], (int)tStr.size(), &wstrTo[0], size_needed);

		return wstrTo;
	}

public:

	template<typename T>
	static void WeaponRegister(const std::string& tName, eLayerType tType) {
		mWeaponCreator[tName] = [=]() -> CWeapon* {
			CWeapon* weapon = Instantiate<CWeapon>(tType);
			weapon->AddComponent<T>();
			return weapon;
			};
	}

	template<typename T>
	static void MonsterRegister(const std::string& tName) {
		mMonsterCreator[tName] = []() -> CEnemy* {
			CEnemy* enemy = Instantiate<CEnemy>(eLayerType::Enemy);
			enemy->AddComponent<T>();
			return enemy;
			};
	}

	static void LoadDatas();

	static const json& GetMonsterBasicStats() {
		return mMonsterStats;
	}

	static const json& GetWeaponDatas() {
		return mWeaponDatas;
	}

	static const json& GetCharacterDatas() {
		return mCharacterDatas;
	}

	static const json& GetEffectDatas() {
		return mEffectDatas;
	}

	static const std::unordered_map<std::string, std::function<CEnemy* ()>>& GetMonsterCreator() {
		return mMonsterCreator;
	}

	static const std::unordered_map<std::string, std::function<CWeapon* ()>>& GetWeaponCreator() {
		return mWeaponCreator;
	}

private:
	static json mMonsterStats;
	static json mWeaponDatas;
	static json mCharacterDatas;
	static json mEffectDatas;
	
	static std::unordered_map<std::string, std::function<CEnemy* ()>> mMonsterCreator;
	static std::unordered_map<std::string, std::function<CWeapon* ()>> mWeaponCreator;
};

