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
		float time = 0.0f;
		std::string ID = "";
		int count = 0;
		std::string spawnType = "";
	};

	static void LoadStageSpawnEvents(int tStageNum);

	static void OnCreate(CAPIEngine* tEngine);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void DestroyStageSpawnEvents();

	static SVector2D GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR);

	static void MonsterSpawnEvent(float tDeltaTime, GameObject* tTarget);

	static void MonsterSpawn(const std::string tMonsterId, GameObject* tTarget, SVector2D tPosition);

	static void SetEventIdxZero();

private:
	static float mHPMultiplier;
	static std::vector<SpawnEvent> mActiveStageSpawnEvents;
	static int mEventIdx;
};