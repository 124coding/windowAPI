#include "CSMGScript.h"

#include "CPlayScene.h"

#include "CPlayer.h"

float CSMGScript::GetDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float percent = 0.0f;

	switch (this->GetTier()) {
	case 1:
		percent = 0.5;
		break;
	case 2:
		percent = 0.6;
		break;
	case 3:
		percent = 0.7;
		break;
	case 4:
		percent = 0.8;
		break;
	}

	float damage = CWeaponScript::GetDamage() + plSc->GetRangedDamage() * percent;
	return damage;
}
