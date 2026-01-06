#include "CClawScript.h"

#include "CPlayer.h"

#include "CPlayScene.h"

CClawScript::SDamageInfo CClawScript::GetFinalDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float percent = 0.0f;

	switch (this->GetTier()) {
	case 1:
		percent = 0.15;
		break;
	case 2:
		percent = 0.2;
		break;
	case 3:
		percent = 0.25;
		break;
	case 4:
		percent = 0.3;
		break;
	}

	float damage = CWeaponScript::GetDamage() + plSc->GetAttackSpeedPercent() * percent + plSc->GetMeleeDamage() * 0.5f;
	return ApplyDamageModifiers(damage);
}