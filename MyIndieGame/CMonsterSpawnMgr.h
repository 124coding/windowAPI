#pragma once

#include "Object.h"
#include "CEnemy.h"

#include "json.hpp"

#include "CDataMgr.h"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

class CMonsterSpawnMgr
{
public:

	struct SpawnEvent {
		// 몬스터 생성 이벤트
		float spawnTiming = 0.0f;
		float repeatingInterval = 0.0f;
		float minRepeatingInterval = 0.0f;
		float reduceRepeatingInterval = 0.0f;
		float nextSpawnTime = 0.0f;
		float currentInterval = 0.0f;
		std::vector<std::wstring>IDs;
		std::wstring spawnType = L"";
	};

	static void LoadStageSpawnEvents(int tStageNum);

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void DestroyStageSpawnEvents();

	static SVector2D GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR);

	static void MonsterSpawnEvent(GameObject* tTarget);

	static void MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, float tMinDistance, float tMaxDistance, bool tIndividual);

	static void MinusTime(float tDeltaTime) {
		mTime -= tDeltaTime;
	}

	static float GetTime() {
		return mTime;
	}

private:
	static float mInitialTime;
	static float mTime;
	static int mStageNum;
	static std::vector<SpawnEvent> mActiveStageSpawnEvents;
};