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

// [Data-Driven] JSON 파일에서 스테이지별 스폰 정보 로드
void CMonsterSpawnMgr::LoadStageSpawnEvents(int tStageNum) {
	mActiveStageSpawnEvents.clear();

	// 파일 경로 동적 생성 (예: Stage_1.json)
	std::string stageFileName = "../Data/Stages/Stage_" + std::to_string(tStageNum) + ".json";
	std::ifstream stageFile(stageFileName, std::ios::in);

	if (!stageFile.is_open()) {
		return;
	}

	json j;
	stageFile >> j;

	mStageNum = tStageNum;

	// 스테이지 기본 설정 (제한 시간, 보상 배율)
	mInitialTime = j["DefaultModifiers"]["Time"];
	mExpMultiple = j["DefaultModifiers"]["Exp_Multiplier"];
	mMoneyMultiple = j["DefaultModifiers"]["Money_Multiplier"];
	mTime = mInitialTime;

	// 이벤트 목록 파싱
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

// [Math Logic] 플레이어 주변 랜덤 위치 생성
// 극좌표계(각도와 거리)를 사용하여 원형 범위를 계산한 뒤 직교좌표계(x, y)로 변환
SVector2D CMonsterSpawnMgr::GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR) {
	// 1. 랜덤 각도 (0 ~ 360도)
	float angle = DegToRad(((float)rand() / RAND_MAX) * 360);

	// 2. 랜덤 거리 (최소 반경 ~ 최대 반경)
	float dist = minR + ((float)rand() / RAND_MAX) * (maxR - minR);

	// 3. 극좌표 -> 직교좌표 변환 (x = r * cos, y = r * sin)
	float x = cosf(angle) * dist;
	float y = sinf(angle) * dist;

	SVector2D resultPos = tPlayerPos + SVector2D(x, y);

	// [Map Boundary Clamping]
	// 생성된 좌표가 맵 밖으로 나가지 않도록 타일 크기만큼 여유를 두고 가둠
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

// 스폰 타이밍 체크 및 난이도 조절
void CMonsterSpawnMgr::MonsterSpawnEvent(GameObject* tTarget) {
	for (auto& spawnEvent: mActiveStageSpawnEvents) {
		
		// 현재 진행 시간이 다음 스폰 시간을 넘었는지 체크 (Timer Check)
		if (mInitialTime - mTime >= spawnEvent.nextSpawnTime) {

			// 스폰 타입에 따른 로직 분기
			if (spawnEvent.spawnType == L"Individual") {
				for (auto& mID : spawnEvent.IDs) {

					// 플레이어 주변 넓은 범위에 산개하여 스폰
					MonsterSpawn(mID, tTarget, 500.0f, 1000.0f, true);
				}
			}
			else if (spawnEvent.spawnType == L"cluster") {
				for (auto& mID : spawnEvent.IDs) {

					// 특정 지점에 뭉쳐서 스폰 (Cluster)
					MonsterSpawn(mID, tTarget, 0.0f, 50.0f, true);
				}
			}

			// [Dynamic Difficulty Adjustment]
			// 다음 스폰까지의 간격을 점점 줄여서 게임을 어렵게 만듦 (Soft Enrage)
			if (spawnEvent.currentInterval > spawnEvent.minRepeatingInterval) {
				spawnEvent.currentInterval -= spawnEvent.reduceRepeatingInterval;
			}
			spawnEvent.nextSpawnTime += spawnEvent.currentInterval;
		}
		else {
			// 이벤트 리스트는 시간순으로 정렬되어 있다고 가정하면, 
			// 아직 시간이 안 된 이벤트가 나오면 뒤쪽은 볼 필요 없이 break 가능 (최적화)
			break;
		}
	}
}

// 실제 몬스터 객체 생성 및 초기화
void CMonsterSpawnMgr::MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, float tMinDistance, float tMaxDistance, bool tIndividual) {
	SVector2D targetPos = tTarget->GetComponent<CTransform>(eComponentType::Transform)->GetPos();

	// 뭉쳐서 나올 경우를 대비한 기준점(Anchor) 생성
	SVector2D anchorPos = GetRandomPosAroundObject(targetPos, 500.0f, 800.0f);

	// 데이터 매니저에서 몬스터 스탯 조회
	CDataMgr::SMonster currentMonster;

	auto it = CDataMgr::GetMonsterBasicStats().find(tMonsterId);

	if (it == CDataMgr::GetMonsterBasicStats().end()) {
		return;
	}

	currentMonster = it->second;

	// 몬스터 이름에 매핑된 생성 함수 호출
	auto iter = CDataMgr::GetMonsterCreator().find(currentMonster.name);
	if (iter == CDataMgr::GetMonsterCreator().end()) {
		return;
	}

	// 랜덤 마리수 결정 (Min ~ Max)
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

		// 몬스터 객체 생성
		CEnemy* enemy = iter->second();

		enemy->SetName(currentMonster.name);

		// 스크립트 설정 (타겟, HP, 데미지, 속도 등)
		CEnemyScript* enemyScript = enemy->GetComponent<CEnemyScript>(eComponentType::Script);
		enemyScript->SetTarget(tTarget);

		// 스테이지 진행도에 따른 능력치 스케일링 (Level Scaling)
		enemyScript->SetHP(currentMonster.hp + (mStageNum * currentMonster.hpIncreasedEachWave));
		enemyScript->SetDamage(currentMonster.damage + (mStageNum * currentMonster.damageIncreaseEachWave));

		enemyScript->SetSpeed(currentMonster.speed);

		// 텍스처 설정
		CTexture* enemyImg = CResourceMgr::Find<CTexture>(currentMonster.name);
		enemyScript->SetBaseTexture(enemyImg);

		// 생성 연출용 스프라이트 설정
		CSpriteRenderer* sr = enemy->AddComponent<CSpriteRenderer>();
		sr->SetTexture(CResourceMgr::Find<CTexture>(L"EnemyBirth"));
		sr->GetTexture()->SetWidth(80.0f);
		sr->GetTexture()->SetHeight(80.0f);

		sr->GetTexture()->CreateHBitmapFromGdiPlus(true);		// GDI+ 사용 (투명도/회전)

		// 위치 설정 (개별 산개 or 뭉치기)
		CTransform* enemyTr = enemy->GetComponent<CTransform>(eComponentType::Transform);
		if(tIndividual) {
			enemyTr->SetPos(GetRandomPosAroundObject(targetPos, tMinDistance, tMaxDistance));
		}
		else {
			enemyTr->SetPos(GetRandomPosAroundObject(anchorPos, tMinDistance, tMaxDistance));
		}

		enemy->SetSize(SVector2D(currentMonster.sizeX, currentMonster.sizeY));
		enemy->SetAnchorPoint(enemyImg->GetWidth() / 2, enemyImg->GetHeight());

		// 충돌체 설정 (초기엔 비활성화 -> Spawn 애니메이션 후 활성화됨)
		CCollider* enemyCl = enemy->AddComponent<CCircleCollider2D>();
		enemyCl->SetSize(SVector2D(currentMonster.collisionSizeX, currentMonster.collisionSizeY));
		enemyCl->SetActivate(false);

		enemy->OnCreate();
	}
}