#include "CSpearScript.h"

#include "CPlayer.h"

#include "CPlayScene.h"

CSpearScript::SDamageInfo CSpearScript::GetFinalDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float damage = CWeaponScript::GetDamage() + plSc->GetMeleeDamage() * 1.0f;
	return ApplyDamageModifiers(damage);
}