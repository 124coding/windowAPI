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
	// [Monster Data] 몬스터 기본 스탯
	struct SMonster {
		std::wstring ID = L"";
		std::wstring name = L"";
		int hp = 0;
		float hpIncreasedEachWave = 0.0f;			// 웨이브 진행에 따른 체력 증가량 (Scaling)
		float damage = 0;
		float damageIncreaseEachWave = 0.0f;		// 웨이브 진행에 따른 공격력 증가량
		float speed = 0.0f;

		// 렌더링 및 충돌체 크기
		float sizeX = 0.0f;
		float sizeY = 0.0f;
		float collisionSizeX = 0.0f;
		float collisionSizeY = 0.0f;

		// 스폰 규칙 (무리 지어 나올 때의 수량)
		int minNumber = 0.0f;
		int maxNumber = 0.0f;
		float minDistanceFromPlayer = 0.0f;
	};

	// [Weapon Data] 무기 등급별 상세 스탯
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

	// 무기 전체 정보 (공통 속성 + 티어별 속성)
	struct SWeapon {
		std::wstring weaponType = L"";			// Melee / Ranged
		std::wstring ID = L"";
		std::wstring name = L"";
		std::wstring classType = L"";			// 무기 계열 (예: "검", "총")
		std::wstring bulletName = L"";			// 원거리 무기일 경우 사용할 총알 텍스처
		std::wstring iconTexture = L"";

		// 투사체 속성
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

	// [Effect & Text]
	struct SEffect {
		std::wstring ID = L"";
		std::wstring name = L"";
		std::wstring description = L"";
	};

	// 리치 텍스트 파싱을 위한 인자 값 (Value + Color)
	// 예: "공격력 <c=#FF0000>+10</c>" 등을 처리하기 위함
	struct SArg {
		std::wstring value = L"";
		std::wstring color = L"";
	};

	// [Character Data] 플레이어 캐릭터 정보
	struct SCharacter {
		std::wstring ID = L"";
		std::wstring name = L"";

		// 파츠별 텍스처 (Composite Rendering용)
		std::wstring hairTexture = L"";
		std::wstring eyesTexture = L"";
		std::wstring mouthTexture = L"";
		std::wstring clothTexture = L"";
		std::wstring iconTexture = L"";

		std::vector<std::wstring> weapons;	// 시작 무기 목록
		std::unordered_map<std::wstring, std::vector<SArg>> effects;	// 고유 특성
	};

	// [Item Data] 아이템
	struct SItem {
		std::wstring ID = L"";
		std::wstring name = L"";
		int tier = 0;
		std::wstring pos = L"";		// 장착 부위 (Eye, Mouth 등 외형 변경용)
		int basePrice = 0;
		std::unordered_map<std::wstring, std::vector<SArg>> effects;	// 아이템 효과
	};

	// 레벨업 보상 데이터
	struct SUpgradeTier {
		int tier = 0;
		SArg arg;	// 상승 수치
	};

	struct SUpgrades {
		std::wstring ID = L"";
		std::wstring effectID = L"";
		std::wstring texName = L"";
		std::wstring name = L"";
		std::array<SUpgradeTier, 4> tiers;
	};

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	// String Conversion Helper (std::string <-> std::wstring)
	// JSON 라이브러리는 string을 쓰고, WinAPI는 wstring을 쓰기 때문에 변환 필요
	static std::wstring ToWString(const std::string& tStr) {
		if (tStr.empty()) return std::wstring();

		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &tStr[0], (int)tStr.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &tStr[0], (int)tStr.size(), &wstrTo[0], size_needed);

		return wstrTo;
	}

public:

	// 템플릿(T)을 사용하여 특정 스크립트(CWeaponScript 자식들)를 부착하는 생성 함수를 등록함
	// 람다 함수 내부에 '객체 생성 -> 컴포넌트 부착' 로직을 캡슐화하여 맵에 저장
	template<typename T>
	static void WeaponRegister(const std::wstring& tID, eLayerType tType) {
		mWeaponCreator[tID] = [=]() -> CWeapon* {
			CWeapon* weapon = Instantiate<CWeapon>(tType, CSceneMgr::GetWantScene(L"PlayScene"));
			weapon->AddComponent<T>();
			return weapon;
			};
	}

	// 몬스터 생성기 등록
	template<typename T>
	static void MonsterRegister(const std::wstring& tID) {
		mMonsterCreator[tID] = []() -> CEnemy* {
			CEnemy* enemy = Instantiate<CEnemy>(eLayerType::Enemy, CSceneMgr::GetWantScene(L"PlayScene"));
			enemy->AddComponent<T>();
			return enemy;
			};
	}

	// JSON 데이터 로드 진입점
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

	static const std::unordered_map<std::wstring, SUpgrades>& GetUpgradeDatas() {
		return mUpgradeDatas;
	}
	static const std::vector<SWeapon>& GetWeaponList() {
		return mWeaponList;
	}

	static const std::vector<std::vector<SItem>>& GetItemClassificationByTier() {
		return mItemClassificationByTier;
	}

	// [RNG Logic] 스테이지 가중치를 적용한 랜덤 뽑기
	static std::pair<int, SWeapon> GetRandomWeaponByStage(int curStage);
	static SItem GetRandomItemByStage(int curStage);


	static const std::unordered_map<std::wstring, std::function<CEnemy* ()>>& GetMonsterCreator() {
		return mMonsterCreator;
	}

	static const std::unordered_map<std::wstring, std::function<CWeapon* ()>>& GetWeaponCreator() {
		return mWeaponCreator;
	}

private:
	// ID를 키(Key)로 사용하여 O(1) 속도로 데이터 조회
	static std::unordered_map<std::wstring, SMonster> mMonsterStats;
	static std::unordered_map<std::wstring, SWeapon> mWeaponDatas;
	static std::unordered_map<std::wstring, SCharacter> mCharacterDatas;
	static std::unordered_map<std::wstring, SEffect> mEffectDatas;
	static std::unordered_map<std::wstring, SItem> mItemDatas;
	static std::unordered_map<std::wstring, SUpgrades> mUpgradeDatas;

	// 리스트형 컨테이너
	static std::vector<SWeapon> mWeaponList;
	static std::vector<std::vector<SItem>> mItemClassificationByTier;
	
	// [Creator Maps]
	// 문자열 ID -> 객체 생성 함수(std::function) 매핑
	static std::unordered_map<std::wstring, std::function<CEnemy* ()>> mMonsterCreator;
	static std::unordered_map<std::wstring, std::function<CWeapon* ()>> mWeaponCreator;
};

