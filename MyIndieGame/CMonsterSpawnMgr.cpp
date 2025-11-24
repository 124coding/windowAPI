#include "CMonsterSpawnMgr.h"

#include "CEnemyScript.h"
#include "CCircleCollider2D.h"

float CMonsterSpawnMgr::mHPMultiplier = 1.0f;
std::vector<CMonsterSpawnMgr::SpawnEvent> CMonsterSpawnMgr::mActiveStageSpawnEvents;
int CMonsterSpawnMgr::mEventIdx = 0;

void CMonsterSpawnMgr::LoadStageSpawnEvents(int tStageNum) {
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
		event.spawnType = e["SpawnType"];
	}

	mActiveStageSpawnEvents.push_back(event);
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

	return tPlayerPos + SVector2D(x, y);
}

void CMonsterSpawnMgr::MonsterSpawnEvent(float tDeltaTime, GameObject* tTarget) {

	// json 파일을 확인하여 개별적으로 스폰하는지 무리를 지어 스폰하는지
	SVector2D targetPos = tTarget->GetComponent<CTransform>()->GetPos();

	for (mEventIdx; mEventIdx < mActiveStageSpawnEvents.size(); mEventIdx++) {
		if (mActiveStageSpawnEvents[mEventIdx].time <= tDeltaTime) {
			if (mActiveStageSpawnEvents[mEventIdx].spawnType == "Individual") {
				for (int i = 0; i < mActiveStageSpawnEvents[mEventIdx].count; i++) {
					MonsterSpawn(mActiveStageSpawnEvents[mEventIdx].ID, tTarget, GetRandomPosAroundObject(targetPos, 600.0f, 1000.0f));
				}
			}
			else if (mActiveStageSpawnEvents[mEventIdx].spawnType == "cluster") {
				SVector2D anchorPos = GetRandomPosAroundObject(targetPos, 600.0f, 1000.0f);

				for (int i = 0; i < mActiveStageSpawnEvents[mEventIdx].count; i++) {
					MonsterSpawn(mActiveStageSpawnEvents[mEventIdx].ID, tTarget, GetRandomPosAroundObject(anchorPos, 0.0f, 50.0f));
				}
			}

		}
		else {
			break;
		}
	}
}

void CMonsterSpawnMgr::MonsterSpawn(const std::string tMonsterId, GameObject* tTarget, SVector2D tPosition) {
	CDataMgr::SMonster currentMonster;

	for (const auto& monsterData : CDataMgr::GetMonsterBasicStats()["Monsters"]) {
		if (monsterData["M_ID"] == tMonsterId) {
			currentMonster.ID = monsterData["M_ID"];
			currentMonster.name = monsterData["Name"];
			currentMonster.hp = monsterData["HP"];
			currentMonster.buttDamage = monsterData["ButtDamage"];
			currentMonster.attackDamage = monsterData["AttackDamage"];
			currentMonster.speed = monsterData["Speed"];
			currentMonster.sizeX = monsterData["SizeX"];
			currentMonster.sizeY = monsterData["SizeY"];
			currentMonster.collisionSizeX = monsterData["CollisionSizeX"];
			currentMonster.collisionSizeY = monsterData["CollisionSizeY"];

			currentMonster.hp *= mHPMultiplier;

			break;
		}
	}

	if (currentMonster.ID != tMonsterId) {
		return;
	}

	auto iter = CDataMgr::GetMonsterCreator().find(currentMonster.name);
	if (iter == CDataMgr::GetMonsterCreator().end()) {
		return;
	}

	CEnemy* enemy = iter->second();

	CEnemyScript* enemyScript = enemy->GetComponent<CEnemyScript>();
	enemyScript->SetTarget(tTarget);

	enemyScript->SetHP(currentMonster.hp);
	enemyScript->SetButtDamage(currentMonster.buttDamage);

	enemyScript->SetSpeed(currentMonster.speed);

	CTexture* enemyImg = CResourceMgr::Find<CTexture>(std::wstring(currentMonster.name.begin(), currentMonster.name.end()));

	CSpriteRenderer* enemySr = enemy->AddComponent<CSpriteRenderer>();
	enemySr->SetTexture(enemyImg);

	CTransform* enemyTr = enemy->GetComponent<CTransform>();
	enemyTr->SetPos(tPosition);

	enemy->SetSize(SVector2D(currentMonster.sizeX, currentMonster.sizeY));
	enemy->SetAnchorPoint(enemyImg->GetWidth() / 2, enemyImg->GetHeight());

	CCollider* enemyCl = enemy->AddComponent<CCircleCollider2D>();
	enemyCl->SetSize(SVector2D(currentMonster.collisionSizeX, currentMonster.collisionSizeY));

	enemy->OnCreate();
}

void CMonsterSpawnMgr::SetEventIdxZero() {
	mEventIdx = 0;
}