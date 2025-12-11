#include "CMonsterSpawnMgr.h"

#include "CEnemyScript.h"
#include "CCircleCollider2D.h"
#include "CTilemapRenderer.h"

float CMonsterSpawnMgr::mHPMultiplier = 1.0f;
std::vector<CMonsterSpawnMgr::SpawnEvent> CMonsterSpawnMgr::mActiveStageSpawnEvents;
int CMonsterSpawnMgr::mEventIdx = 0;

void CMonsterSpawnMgr::LoadStageSpawnEvents(int tStageNum) {
	std::string stageFileName = "../Data/Stages/Stage_" + std::to_string(tStageNum) + ".json";
	std::ifstream stageFile(stageFileName, std::ios::in);

	json j;
	stageFile >> j;

	mHPMultiplier = j["DefaultModifiers"]["HP_Multiplier"];

	SpawnEvent event;

	for (auto& e : j["Events"]) {
		event.time = e["SpawnTiming"];
		event.ID = CDataMgr::ToWString(e["M_ID"]);
		event.count = e["Count"];
		event.spawnType = CDataMgr::ToWString(e["SpawnType"]);
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

void CMonsterSpawnMgr::MonsterSpawnEvent(float tDeltaTime, GameObject* tTarget) {

	// json 파일을 확인하여 개별적으로 스폰하는지 무리를 지어 스폰하는지
	SVector2D targetPos = tTarget->GetComponent<CTransform>()->GetPos();

	for (mEventIdx; mEventIdx < mActiveStageSpawnEvents.size(); mEventIdx++) {
		if (mActiveStageSpawnEvents[mEventIdx].time <= tDeltaTime) {
			if (mActiveStageSpawnEvents[mEventIdx].spawnType == L"Individual") {
				for (int i = 0; i < mActiveStageSpawnEvents[mEventIdx].count; i++) {
					MonsterSpawn(mActiveStageSpawnEvents[mEventIdx].ID, tTarget, GetRandomPosAroundObject(targetPos, 500.0f, 800.0f));
				}
			}
			else if (mActiveStageSpawnEvents[mEventIdx].spawnType == L"cluster") {
				SVector2D anchorPos = GetRandomPosAroundObject(targetPos, 500.0f, 800.0f);

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

void CMonsterSpawnMgr::MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, SVector2D tPosition) {
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

	CEnemy* enemy = iter->second();

	enemy->SetName(currentMonster.name);

	CEnemyScript* enemyScript = enemy->GetComponent<CEnemyScript>();
	enemyScript->SetTarget(tTarget);

	enemyScript->SetHP(currentMonster.hp);
	enemyScript->SetButtDamage(currentMonster.buttDamage);

	enemyScript->SetSpeed(currentMonster.speed);

	CTexture* enemyImg = CResourceMgr::Find<CTexture>(currentMonster.name);
	enemyScript->SetBaseTexture(enemyImg);

	CSpriteRenderer* sr = enemy->AddComponent<CSpriteRenderer>();
	sr->SetTexture(CResourceMgr::Find<CTexture>(L"EnemyBirth"));
	sr->GetTexture()->SetWidth(80.0f);
	sr->GetTexture()->SetHeight(80.0f);

	sr->GetTexture()->CreateHBitmapFromGdiPlus(true);

	CTransform* enemyTr = enemy->GetComponent<CTransform>();
	enemyTr->SetPos(tPosition);

	enemy->SetSize(SVector2D(currentMonster.sizeX, currentMonster.sizeY));
	enemy->SetAnchorPoint(enemyImg->GetWidth() / 2, enemyImg->GetHeight());

	CCollider* enemyCl = enemy->AddComponent<CCircleCollider2D>();
	enemyCl->SetSize(SVector2D(currentMonster.collisionSizeX, currentMonster.collisionSizeY));
	enemyCl->SetActivate(false);

	enemy->OnCreate();
}

void CMonsterSpawnMgr::SetEventIdxZero() {
	mEventIdx = 0;
}