#include "CMedicalGunScript.h"

#include "CPlayScene.h"

#include "CPlayer.h"

float CMedicalGunScript::GetDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

	float damage = CWeaponScript::GetDamage() + plSc->GetRangedDamage() * 1.0f;
	return damage;
}
