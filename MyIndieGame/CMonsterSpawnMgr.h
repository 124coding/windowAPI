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

	// [Spawn Event Structure]
	// 하나의 몬스터 웨이브 정보를 담는 데이터 구조체
	// JSON 파일의 객체 하나와 1:1로 매핑됨
	struct SpawnEvent {
		float spawnTiming = 0.0f;				// 웨이브 시작 시간(게임 시작 후 몇 초 뒤에 등장하는가)

		// [Difficulty Curve Logic] 난이도 조절 변수들
		float repeatingInterval = 0.0f;			// 초기 스폰 주기 (몇 초마다 생성하는가)
		float minRepeatingInterval = 0.0f;		// 스폰 주기의 최소값 (더 이상 빨라지지 않는 한계선)
		float reduceRepeatingInterval = 0.0f;	// 스폰 주기 감소량 (시간이 지날수록 점점 빨리 나오게 함)
		float nextSpawnTime = 0.0f;				// 다음 스폰까지 남은 시간 (Runtime 변수)
		float currentInterval = 0.0f;			// 현재 적용 중인 스폰 주기 (Runtime 변수)
		std::vector<std::wstring>IDs;			// 스폰될 몬스터들의 ID 목록 (랜덤 선택 등 활용)
		std::wstring spawnType = L"";			// 스폰 패턴 (예: "Individual"(개별))
	};

	// 스테이지 번호에 맞는 JSON 파일을 읽어 SpawnEvent 리스트(mActiveStageSpawnEvents)를 채움
	static void LoadStageSpawnEvents(int tStageNum);

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void DestroyStageSpawnEvents();

	// [Position Calculation]
	// 플레이어 주변 도넛 모양 범위(minR ~ maxR) 내에서 랜덤한 좌표를 반환
	// 플레이어의 시야 밖에서 몬스터가 생성되어 접근하는 연출을 위해 사용
	static SVector2D GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR);

	// 매 프레임 호출되어 현재 시간(mTime)에 활성화되어야 할 스폰 이벤트가 있는지 체크
	static void MonsterSpawnEvent(GameObject* tTarget);

	// 실제 몬스터 객체(Instance)를 생성하고 씬에 배치하는 함수
	static void MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, float tMinDistance, float tMaxDistance, bool tIndividual);

	static void MinusTime(float tDeltaTime) {
		mTime -= tDeltaTime;
	}

	static float GetTime() {
		return mTime;
	}

	// [Reward Multiplier]
	// 스테이지나 난이도에 따른 보상(경험치, 돈) 배율
	static float GetExpMultiple() {
		return mExpMultiple;
	}

	static float GetMoneyMultiple() {
		return mMoneyMultiple;
	}

private:
	static float mExpMultiple;
	static float mMoneyMultiple;
	static float mInitialTime;		// 스테이지 제한 시간
	static float mTime;				// 남은 시간
	static int mStageNum;

	// 현재 로드된 스테이지의 모든 스폰 이벤트 목록
	static std::vector<SpawnEvent> mActiveStageSpawnEvents;
};