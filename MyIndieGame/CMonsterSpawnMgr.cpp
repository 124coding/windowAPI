#include "CMonsterSpawnMgr.h"

std::unordered_map<std::string, std::function<GameObject* ()>> CMonsterSpawnMgr::mCreator;


json CMonsterSpawnMgr::mMonsterStats = nullptr;
float CMonsterSpawnMgr::mHPMultiplier = 1.0f;
std::vector<CMonsterSpawnMgr::SpawnEvent> CMonsterSpawnMgr::mActiveStageSpawnEvents;
int CMonsterSpawnMgr::mEventId = 0;