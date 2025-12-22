#include "CScissorsScript.h"

#include "CPlayer.h"

#include "CPlayScene.h"

CScissorsScript::SDamageInfo CScissorsScript::GetFinalDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

	float damage = CWeaponScript::GetDamage() + plSc->GetMeleeDamage() * 0.8f;
	return ApplyDamageModifiers(damage);
}