#pragma once

#include "Object.h"
#include "CEnemyScript.h"
#include "json.hpp"

#include "CBabyAlien.h"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

class CMonsterSpawnMgr
{
public:
	struct SMonster {
		// 각 몬스터가 가지는 값들을 저장하기 위한 구조체
		std::string ID = "";
		std::string className = "";
		std::string image = "";
		int hp = 0;
		int buttDamage = 0;
		int attackDamage = 0;
		float speed = 0.0f;
		float sizeX = 0.0f;
		float sizeY = 0.0f;
	};

	struct SpawnEvent {
		// 몬스터 생성 이벤트
		float time = 0.0f;
		std::string ID = "";
		int count = 0;
	};

	static void LoadMonsterBasicStats() {
		std::ifstream monsterFile("../Data/MonsterStats.json", std::ios::in);

		monsterFile >> mMonsterStats;
	}

	static void LoadStageSpawnEvents(int tStageNum) {
		std::string stageFileName = "../Data/Stage_" + std::to_string(tStageNum) + ".json";
		std::ifstream stageFile(stageFileName, std::ios::in);

		json j;
		stageFile >> j;

		mHPMultiplier = j["DefaultModifiers"]["HP_Multiplier"];
		
		SpawnEvent event;

		for (auto& e : j["Events"]) {
			event.time = e["Time"];
			event.ID = e["M_ID"];
			event.count = e["Count"];
		}

		mActiveStageSpawnEvents.push_back(event);
	}

	static void DestroyStageSpawnEvents() {
		mActiveStageSpawnEvents.clear();
	}

	template<typename T>
	static void Register(const std::string& tClassName) {
		mCreator[tClassName] = []() -> GameObject* {
			return Instantiate<T>(eLayerType::Enemy);
		};
	}

	static void MonsterSpawnEvent(float tDeltaTime, GameObject* tTarget) {
		for (mEventIdx; mEventIdx < mActiveStageSpawnEvents.size(); mEventIdx++) {
			if (mActiveStageSpawnEvents[mEventIdx].time <= tDeltaTime) {
				for (int i = 0; i < mActiveStageSpawnEvents[mEventIdx].count; i++) {
					MonsterSpawn(mActiveStageSpawnEvents[mEventIdx].ID, tTarget, SVector2D());
				}
			}
			else {
				break;
			}
		}
	}

	static void MonsterSpawn(const std::string tMonsterId, GameObject* tTarget, SVector2D tPosition) {
		SMonster currentMonster;

		for (const auto& monsterData : mMonsterStats["Monsters"]) {
			if (monsterData["M_ID"] == tMonsterId){
				currentMonster.ID = monsterData["M_ID"];
				currentMonster.className = monsterData["ClassName"];
				currentMonster.image = monsterData["Image"];
				currentMonster.hp = monsterData["HP"];
				currentMonster.buttDamage = monsterData["ButtDamage"];
				currentMonster.attackDamage = monsterData["AttackDamage"];
				currentMonster.speed = monsterData["Speed"];
				currentMonster.sizeX = monsterData["SizeX"];
				currentMonster.sizeY = monsterData["SizeY"];

				currentMonster.hp *= mHPMultiplier;

				break;
			}
		}

		if (currentMonster.ID != tMonsterId) {
			return;
		}

		auto iter = mCreator.find(currentMonster.className);
		if (iter == mCreator.end()) {
			return;
		}

		GameObject* enemy = iter->second();

		CEnemyScript* enemyScript = enemy->GetComponent<CEnemyScript>();
		enemyScript->SetTarget(tTarget);

		enemyScript->SetHP(currentMonster.hp);
		enemyScript->SetButtDamage(currentMonster.buttDamage);

		enemyScript->SetSpeed(currentMonster.speed);

		CTexture* enemyImg = CResourceMgr::Find<CTexture>(std::wstring(currentMonster.image.begin(), currentMonster.image.end()));

		CSpriteRenderer* enemySr = enemy->AddComponent<CSpriteRenderer>();
		enemySr->SetTexture(enemyImg);

		CTransform* enemyTr = enemy->GetComponent<CTransform>();
		enemyTr->SetPos(tPosition);

		enemy->SetSize(SVector2D(currentMonster.sizeX, currentMonster.sizeY));
		enemy->SetAnchorPoint(enemyImg->GetWidth() / 2, enemyImg->GetHeight());
	}

	static void SetEventIdxZero() {
		mEventIdx = 0;
	}

private:
	static std::unordered_map<std::string, std::function<GameObject* ()>> mCreator;
	static json mMonsterStats;
	static float mHPMultiplier;
	static std::vector<SpawnEvent> mActiveStageSpawnEvents;
	static int mEventIdx;
};