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
		std::wstring ID = L"";
		std::wstring name = L"";
		int hp = 0;
		float hpIncreasedEachWave = 0.0f;
		float damage = 0;
		float damageIncreaseEachWave = 0.0f;
		float speed = 0.0f;
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		float collisionSizeX = 0.0f;
		float collisionSizeY = 0.0f;
		int minNumber = 0.0f;
		int maxNumber = 0.0f;
		float minDistanceFromPlayer = 0.0f;
	};

	struct SWeaponTier {
		int tier = 0;
		float damage = 0.0f;
		float critDamagePer = 0.0f;
		float critChancePer = 0.0f;
		float delay = 0.0f;
		float range = 0.0f;
		float lifeSteal = 0.0f;
		int basePrice = 0;
	};

	struct SWeapon {
		std::wstring weaponType = L"";
		std::wstring ID = L"";
		std::wstring name = L"";
		std::wstring classType = L"";
		std::wstring bulletName = L"";
		std::wstring iconTexture = L"";
		float speed = 0.0f;
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		float bulletSizeX = 0.0f;
		float bulletSizeY = 0.0f;
		float collisionSizeX = 0.0f;
		float collisionSizeY = 0.0f;
		std::array<SWeaponTier, 4> tier;
		std::wstring specialEffect = L"";
	};

	struct SEffect {
		std::wstring ID = L"";
		std::wstring name = L"";
		std::wstring description = L"";
	};

	struct SArg {
		std::wstring value = L"";
		std::wstring color = L"";
	};

	struct SCharacter {
		std::wstring ID = L"";
		std::wstring name = L"";
		std::wstring hairTexture = L"";
		std::wstring eyesTexture = L"";
		std::wstring mouthTexture = L"";
		std::wstring clothTexture = L"";
		std::wstring iconTexture = L"";
		std::vector<std::wstring> weapons;
		std::unordered_map<std::wstring, std::vector<SArg>> effects;
	};

	struct SItem {
		std::wstring ID = L"";
		std::wstring name = L"";
		int tier = 0;
		std::wstring pos = L"";
		int basePrice = 0;
		std::unordered_map<std::wstring, std::vector<SArg>> effects;
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
	static void WeaponRegister(const std::wstring& tID, eLayerType tType) {
		mWeaponCreator[tID] = [=]() -> CWeapon* {
			CWeapon* weapon = Instantiate<CWeapon>(tType, CSceneMgr::GetWantScene(L"PlayScene"));
			weapon->AddComponent<T>();
			return weapon;
			};
	}

	template<typename T>
	static void MonsterRegister(const std::wstring& tID) {
		mMonsterCreator[tID] = []() -> CEnemy* {
			CEnemy* enemy = Instantiate<CEnemy>(eLayerType::Enemy, CSceneMgr::GetWantScene(L"PlayScene"));
			enemy->AddComponent<T>();
			return enemy;
			};
	}

	static void LoadDatas();

	static const std::unordered_map<std::wstring, SMonster>& GetMonsterBasicStats() {
		return mMonsterStats;
	}

	static const std::unordered_map<std::wstring, SWeapon>& GetWeaponDatas() {
		return mWeaponDatas;
	}

	static const std::unordered_map<std::wstring, SCharacter>& GetCharacterDatas() {
		return mCharacterDatas;
	}

	static const std::unordered_map<std::wstring, SEffect>& GetEffectDatas() {
		return mEffectDatas;
	}

	static const std::unordered_map<std::wstring, SItem>& GetItemDatas() {
		return mItemDatas;
	}

	static const std::vector<SWeapon>& GetWeaponList() {
		return mWeaponList;
	}

	static const std::vector<std::vector<SItem>>& GetItemClassificationByTier() {
		return mItemClassificationByTier;
	}

	static std::pair<int, SWeapon> GetRandomWeaponByStage(int curStage);
	static SItem GetRandomItemByStage(int curStage);



	static const std::unordered_map<std::wstring, std::function<CEnemy* ()>>& GetMonsterCreator() {
		return mMonsterCreator;
	}

	static const std::unordered_map<std::wstring, std::function<CWeapon* ()>>& GetWeaponCreator() {
		return mWeaponCreator;
	}

private:
	static std::unordered_map<std::wstring, SMonster> mMonsterStats;
	static std::unordered_map<std::wstring, SWeapon> mWeaponDatas;
	static std::unordered_map<std::wstring, SCharacter> mCharacterDatas;
	static std::unordered_map<std::wstring, SEffect> mEffectDatas;
	static std::unordered_map<std::wstring, SItem> mItemDatas;
	static std::vector<SWeapon> mWeaponList;
	static std::vector<std::vector<SItem>> mItemClassificationByTier;
	
	static std::unordered_map<std::wstring, std::function<CEnemy* ()>> mMonsterCreator;
	static std::unordered_map<std::wstring, std::function<CWeapon* ()>> mWeaponCreator;
};

