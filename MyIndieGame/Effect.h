#pragma once

#include "CPlayScene.h"

#include "CSceneMgr.h"
#include "CDataMgr.h"

#include "CPlayer.h"

#include "CWeaponMgr.h"

#include <Windows.h>

static void ApplyEffect(const std::wstring& tEffectID, const std::vector<CDataMgr::SArg>& args) {
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	CWeaponMgr* plWpMgr = CPlayScene::GetPlayer()->GetComponent<CWeaponMgr>();

	auto GetValueStr = [&](int index = 0) -> std::wstring {
		if (index < args.size()) return args[index].value;
		return L"";
		};

	if (tEffectID == L"E_001") {
		plSc->IncreaseMaxHP(std::stoi(GetValueStr(0)));
		plSc->IncreaseHP(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_002") {
		plSc->InCreaseHPGeneration(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_003") {
		plSc->IncreaseDodge(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_004") {
		plSc->IncreaseArmor(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_005") {
		plSc->IncreaseRange(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_006") {
		plSc->IncreaseDamagePercent(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_007") {
		plSc->IncreaseCriticalChancePercent(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_008") {
		plSc->IncreaseRangedDamage(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_009") {
		plSc->IncreaseMeleeDamage(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_010") {
		plSc->IncreaseAttackSpeedPercent(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_011") {
		plSc->IncreaseSpeedPercent(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_012") {
		plSc->IncreaseLifeSteal(std::stoi(GetValueStr(0)));
	}
	else if (tEffectID == L"E_013") {
		for (int i = 0; i < std::stoi(GetValueStr(0)); i++) {
			if (GetValueStr(2)[0] == L'M') {
				plWpMgr->PlusWeapon(eLayerType::MeleeWeapon, GetValueStr(2), 1);
			}
			else if (GetValueStr(2)[0] == L'R') {
				plWpMgr->PlusWeapon(eLayerType::RangedWeapon, GetValueStr(2), 1);
			}
		}
	}
	else if (tEffectID == L"E_014") {
		plSc->AddStatModifier(GetValueStr(2), std::stof(GetValueStr(0)));
	}
	else if (tEffectID == L"E_015") {
		plSc->AddStatModifier(GetValueStr(2), -std::stof(GetValueStr(0)));
	}
}