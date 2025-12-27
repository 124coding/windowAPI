#include "CMonsterSpawnMgr.h"

#include "CEnemyScript.h"
#include "CCircleCollider2D.h"
#include "CTilemapRenderer.h"

#include "CDataMgr.h"

float CMonsterSpawnMgr::mExpMultiple = 0.0f;
float CMonsterSpawnMgr::mMoneyMultiple = 0.0f;
float CMonsterSpawnMgr::mInitialTime = 0.0f;
float CMonsterSpawnMgr::mTime = 0.0f;
int CMonsterSpawnMgr::mStageNum = 0;
std::vector<CMonsterSpawnMgr::SpawnEvent> CMonsterSpawnMgr::mActiveStageSpawnEvents;

void CMonsterSpawnMgr::LoadStageSpawnEvents(int tStageNum) {
	mActiveStageSpawnEvents.clear();

	std::string stageFileName = "../Data/Stages/Stage_" + std::to_string(tStageNum) + ".json";
	std::ifstream stageFile(stageFileName, std::ios::in);

	if (!stageFile.is_open()) {
		return;
	}

	json j;
	stageFile >> j;

	mStageNum = tStageNum;

	mInitialTime = j["DefaultModifiers"]["Time"];
	mExpMultiple = j["DefaultModifiers"]["Exp_Multiplier"];
	mMoneyMultiple = j["DefaultModifiers"]["Money_Multiplier"];
	mTime = mInitialTime;

	for (auto& e : j["Events"]) {
		SpawnEvent event;

		event.spawnTiming = e["SpawnTiming"];
		event.repeatingInterval = e["repeating_interval"];
		event.minRepeatingInterval = e["min_repeating_interval"];
		event.reduceRepeatingInterval = e["reduce_repeating_interval"];

		event.nextSpawnTime = event.spawnTiming;
		event.currentInterval = event.repeatingInterval;

		for (auto& mId : e["M_IDS"]) {
			event.IDs.push_back(CDataMgr::ToWString(mId));
		}
		event.spawnType = CDataMgr::ToWString(e["SpawnType"]);

		mActiveStageSpawnEvents.push_back(event);
	}
}

void CMonsterSpawnMgr::OnCreate() {
}

void CMonsterSpawnMgr::OnDestroy() {

}

void CMonsterSpawnMgr::OnUpdate(float tDeltaTime) {

}

void CMonsterSpawnMgr::OnLateUpdate(float tDeltaTime) {

}

void CMonsterSpawnMgr::Render(HDC tHDC) {

}

void CMonsterSpawnMgr::DestroyStageSpawnEvents() {
	mActiveStageSpawnEvents.clear();
}

SVector2D CMonsterSpawnMgr::GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR) {
	float angle = DegToRad(((float)rand() / RAND_MAX) * 360);
	float dist = minR + ((float)rand() / RAND_MAX) * (maxR - minR);

	float x = cosf(angle) * dist;
	float y = sinf(angle) * dist;

	SVector2D resultPos = tPlayerPos + SVector2D(x, y);

	if (resultPos.mX < 0 + tileSizeX) {
		resultPos.mX = 0 + tileSizeX;
	}
	else if (resultPos.mX > mapWidth - tileSizeX) {
		resultPos.mX = mapWidth - tileSizeX;
	}

	if (resultPos.mY < 0 + tileSizeY) {
		resultPos.mY = 0 + tileSizeY;
	}
	else if (resultPos.mY > mapHeight - tileSizeY) {
		resultPos.mY = mapHeight - tileSizeY;
	}

	return resultPos;
}

void CMonsterSpawnMgr::MonsterSpawnEvent(GameObject* tTarget) {
	for (auto& spawnEvent: mActiveStageSpawnEvents) {
	
		if (mInitialTime - mTime >= spawnEvent.nextSpawnTime) {
			if (spawnEvent.spawnType == L"Individual") {
				for (auto& mID : spawnEvent.IDs) {
					MonsterSpawn(mID, tTarget, 500.0f, 1000.0f, true);
				}
			}
			else if (spawnEvent.spawnType == L"cluster") {
				for (auto& mID : spawnEvent.IDs) {
					MonsterSpawn(mID, tTarget, 0.0f, 50.0f, true);
				}
			}

			if (spawnEvent.currentInterval > spawnEvent.minRepeatingInterval) {
				spawnEvent.currentInterval -= spawnEvent.reduceRepeatingInterval;
			}
			spawnEvent.nextSpawnTime += spawnEvent.currentInterval;
		}
		else {
			break;
		}
	}
}

void CMonsterSpawnMgr::MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, float tMinDistance, float tMaxDistance, bool tIndividual) {
	// json 파일을 확인하여 개별적으로 스폰하는지 무리를 지어 스폰하는지
	SVector2D targetPos = tTarget->GetComponent<CTransform>()->GetPos();

	SVector2D anchorPos = GetRandomPosAroundObject(targetPos, 500.0f, 800.0f);

	CDataMgr::SMonster currentMonster;

	auto it = CDataMgr::GetMonsterBasicStats().find(tMonsterId);

	if (it == CDataMgr::GetMonsterBasicStats().end()) {
		return;
	}

	currentMonster = it->second;

	auto iter = CDataMgr::GetMonsterCreator().find(currentMonster.name);
	if (iter == CDataMgr::GetMonsterCreator().end()) {
		return;
	}

	int offset = currentMonster.maxNumber - currentMonster.minNumber;
	int rand = 0;
	if (offset == 0) {
		rand = currentMonster.minNumber;
	}
	else {
		rand = std::rand() % offset + currentMonster.minNumber;
	}

	for (int i = 0; i < rand; i++) {
		if (CSceneMgr::GetGameObjects(eLayerType::Enemy).size() == 100) {
			break;
		}

		CEnemy* enemy = iter->second();

		enemy->SetName(currentMonster.name);

		CEnemyScript* enemyScript = enemy->GetComponent<CEnemyScript>();
		enemyScript->SetTarget(tTarget);

		enemyScript->SetHP(currentMonster.hp + (mStageNum * currentMonster.hpIncreasedEachWave));
		enemyScript->SetDamage(currentMonster.damage + (mStageNum * currentMonster.damageIncreaseEachWave));

		enemyScript->SetSpeed(currentMonster.speed);

		CTexture* enemyImg = CResourceMgr::Find<CTexture>(currentMonster.name);
		enemyScript->SetBaseTexture(enemyImg);

		CSpriteRenderer* sr = enemy->AddComponent<CSpriteRenderer>();
		sr->SetTexture(CResourceMgr::Find<CTexture>(L"EnemyBirth"));
		sr->GetTexture()->SetWidth(80.0f);
		sr->GetTexture()->SetHeight(80.0f);

		sr->GetTexture()->CreateHBitmapFromGdiPlus(true);

		CTransform* enemyTr = enemy->GetComponent<CTransform>();
		if(tIndividual) {
			enemyTr->SetPos(GetRandomPosAroundObject(targetPos, tMinDistance, tMaxDistance));
		}
		else {
			enemyTr->SetPos(GetRandomPosAroundObject(anchorPos, tMinDistance, tMaxDistance));
		}

		enemy->SetSize(SVector2D(currentMonster.sizeX, currentMonster.sizeY));
		enemy->SetAnchorPoint(enemyImg->GetWidth() / 2, enemyImg->GetHeight());

		CCollider* enemyCl = enemy->AddComponent<CCircleCollider2D>();
		enemyCl->SetSize(SVector2D(currentMonster.collisionSizeX, currentMonster.collisionSizeY));
		enemyCl->SetActivate(false);

		enemy->OnCreate();
	}
}