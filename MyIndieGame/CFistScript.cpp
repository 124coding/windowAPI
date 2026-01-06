#include "CFistScript.h"

#include "CPlayer.h"

#include "CPlayScene.h"

CFistScript::SDamageInfo CFistScript::GetFinalDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float damage = CWeaponScript::GetDamage() + plSc->GetMeleeDamage() * 1.0f;
	return ApplyDamageModifiers(damage);
}