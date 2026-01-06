#include "CDaggerScript.h"

#include "CPlayer.h"

#include "CPlayScene.h"

CDaggerScript::SDamageInfo CDaggerScript::GetFinalDamage()
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float damage = CWeaponScript::GetDamage() + plSc->GetMeleeDamage() * 0.8f;
	return ApplyDamageModifiers(damage);
}