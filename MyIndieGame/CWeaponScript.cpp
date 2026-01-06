#include "CWeaponScript.h"

#include "CPlayer.h"
#include "CEnemy.h"

#include "CPlayScene.h"
#include "CSceneMgr.h"

#include "CTransform.h"
#include "CEnemyScript.h"

#include "Object.h"

void CWeaponScript::OnCreate()
{
	CScript::OnCreate();
}

void CWeaponScript::OnDestroy()
{
	CScript::OnDestroy();
}

void CWeaponScript::OnUpdate(float tDeltaTime)
{
	CScript::OnUpdate(tDeltaTime);
}

void CWeaponScript::OnLateUpdate(float tDeltaTime)
{
	CScript::OnLateUpdate(tDeltaTime);
}

void CWeaponScript::Render(HDC tHDC)
{
	CScript::Render(tHDC);
}

void CWeaponScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	CScript::OnCollisionEnter(tDeltaTime, tOther);
}

void CWeaponScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
	CScript::OnCollisionStay(tDeltaTime, tOther);
}

void CWeaponScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
	CScript::OnCollisionExit(tDeltaTime, tOther);
}

void CWeaponScript::SetRotForClosedEnemyWatch(std::vector<GameObject*> tEnemies)
{
	GameObject* closedEnemy = nullptr;
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	if (tEnemies.empty()) {
		mClosedEnemyPos = SVector2D(9999.0f, 9999.0f);
		tr->SetRot(0.0f);
		return;
	}

	for (GameObject* enemy : tEnemies) {
		if (closedEnemy == nullptr || (enemy->GetComponent<CTransform>(eComponentType::Transform)->GetPos() - tr->GetPos()).LengthSq() < (closedEnemy->GetComponent<CTransform>(eComponentType::Transform)->GetPos() - tr->GetPos()).LengthSq()) {
			CEnemyScript* enemySc = enemy->GetComponent<CEnemyScript>(eComponentType::Script);

			if (enemySc->GetState() == CEnemyScript::eState::Spawn || enemySc->GetState() == CEnemyScript::eState::Dead) continue;
			closedEnemy = enemy;
		}
	}

	if (closedEnemy == nullptr) {
		mClosedEnemyPos = SVector2D(9999.0f, 9999.0f);
		tr->SetRot(0.0f);
		return;
	}

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
	CTransform* enemyTr = closedEnemy->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* enemySr = closedEnemy->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	SVector2D enemySize = ObjectSize(closedEnemy);

	SVector2D enemyCenterPos = ObjectCenterPos(closedEnemy);

	mClosedEnemyPos = enemyCenterPos;

	if (tr->GetPos().mX > enemyCenterPos.mX && tr->GetPos().mY > enemyCenterPos.mY) {
		tr->SetRot(RadToDeg(atan(fabs(tr->GetPos().mY - enemyCenterPos.mY) / fabs(tr->GetPos().mX - enemyCenterPos.mX))) - 180.0f);
	}
	else if (tr->GetPos().mX < enemyCenterPos.mX && tr->GetPos().mY > enemyCenterPos.mY){
		tr->SetRot(RadToDeg(atan(fabs(tr->GetPos().mY - enemyCenterPos.mY) / fabs(tr->GetPos().mX - enemyCenterPos.mX))) * -1.0f);
	}
	else if (tr->GetPos().mX > enemyCenterPos.mX && tr->GetPos().mY < enemyCenterPos.mY) {
		tr->SetRot(180.0f - RadToDeg(atan(fabs(tr->GetPos().mY - enemyCenterPos.mY) / fabs(tr->GetPos().mX - enemyCenterPos.mX))));
	}
	else if (tr->GetPos().mX < enemyCenterPos.mX && tr->GetPos().mY < enemyCenterPos.mY) {
		tr->SetRot(RadToDeg(atan(fabs(tr->GetPos().mY - enemyCenterPos.mY) / fabs(tr->GetPos().mX - enemyCenterPos.mX))));
	}
}


void CWeaponScript::CalculatePosNextToTarget()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CTransform* plTr = mTarget->GetComponent<CTransform>(eComponentType::Transform);

	tr->SetPos(ObjectCenterPos(mTarget) + mOffset);
}

CWeaponScript::SDamageInfo CWeaponScript::ApplyDamageModifiers(float tBaseDamage)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	int damagePercent = plSc->GetDamagePercent();

	float finalDamage = tBaseDamage * (1.0f + (damagePercent / 100.0f));

	int rand = std::rand() % 100;

	bool isCritical = false;

	if (rand <= mCriticalChance + plSc->GetCriticalChancePercent()) {
		finalDamage *= mCriticalDamage;
		isCritical = true;
	}

	if (finalDamage < 1.0f)
	{
		finalDamage = 1.0f;
	}

	return SDamageInfo{ finalDamage, isCritical };
}
