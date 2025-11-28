#pragma once

#include "CPlayScene.h"

#include "CSceneMgr.h"

#include "CPlayer.h"

#include <Windows.h>

static void ApplyEffect(const std::wstring& tEffectID, float tValue = 0.0f) {
	CPlayerScript* playerSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	std::vector<GameObject*> enemies = CSceneMgr::GetGameObjects(eLayerType::Enemy);

	if (tEffectID == L"E_001") {
		playerSc->IncreaseMaxHP(tValue);
		playerSc->IncreaseHP(tValue);
	}
	else if (tEffectID == L"E_002") {
		playerSc->InCreaseHPGeneration(tValue);
	}
	else if (tEffectID == L"E_003") {
		playerSc->IncreaseDodge(tValue);
	}
	else if (tEffectID == L"E_004") {
		playerSc->IncreaseArmor(tValue);
	}
	else if (tEffectID == L"E_005") {
		playerSc->IncreaseRange(tValue);
	}
	else if (tEffectID == L"E_006") {
		playerSc->IncreaseDamagePercent(tValue);
	}
	else if (tEffectID == L"E_007") {
		playerSc->IncreaseCriticalChancePercent(tValue);
	}
	else if (tEffectID == L"E_008") {
		playerSc->IncreaseRangedDamage(tValue);
	}
	else if (tEffectID == L"E_009") {
		playerSc->IncreaseMeleeDamage(tValue);
	}
	else if (tEffectID == L"E_010") {
		playerSc->IncreaseAttackSpeedPercent(tValue);
	}
	else if (tEffectID == L"E_011") {
		playerSc->IncreaseSpeedPercent(tValue);
	}
	else if (tEffectID == L"E_012") {
		playerSc->IncreaseLifeSteal(tValue);
	}
}