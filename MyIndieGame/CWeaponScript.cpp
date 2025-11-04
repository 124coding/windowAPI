#include "CWeaponScript.h"

#include "CPlayer.h"
#include "CEnemy.h"

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

	if (tEnemies.empty()) {
		return;
	}

	for (GameObject* enemy : tEnemies) {
		if (closedEnemy == nullptr || enemy->GetComponent<CEnemyScript>()->GetDistanceToPlayer() < closedEnemy->GetComponent<CEnemyScript>()->GetDistanceToPlayer()) {
			closedEnemy = enemy;
		}
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	CTransform* enemyTr = closedEnemy->GetComponent<CTransform>();
	CSpriteRenderer* enemySr = closedEnemy->GetComponent<CSpriteRenderer>();

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

void CWeaponScript::SetFollowPlayer()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetPos(CalculatePosNextToPlayer());
}

SVector2D CWeaponScript::CalculatePosNextToPlayer()
{
	CTransform* plTr = mPlayer->GetComponent<CTransform>();

	return plTr->GetPos() + SVector2D(-10.0f, 0.0f);
}
