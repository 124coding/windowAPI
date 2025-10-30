#include "CWeaponScript.h"

#include "CEnemy.h"

#include "CSceneMgr.h"

#include "CEnemyScript.h"

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

	SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
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

	for (GameObject* enemy : tEnemies) {
		if (closedEnemy == nullptr || enemy->GetComponent<CEnemyScript>()->GetDistanceToPlayer() < closedEnemy->GetComponent<CEnemyScript>()->GetDistanceToPlayer()) {
			closedEnemy = enemy;
		}
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CTransform* enemyTr = closedEnemy->GetComponent<CTransform>();

	if (tr->GetPos().mX > enemyTr->GetPos().mX && tr->GetPos().mY > enemyTr->GetPos().mX) {
		tr->SetRot(atan(fabs(tr->GetPos().mY - enemyTr->GetPos().mY) / fabs(tr->GetPos().mX - enemyTr->GetPos().mX)) - 180.0f);
	}
	else if (tr->GetPos().mX < enemyTr->GetPos().mX && tr->GetPos().mY > enemyTr->GetPos().mX){
		tr->SetRot(atan(fabs(tr->GetPos().mY - enemyTr->GetPos().mY) / fabs(tr->GetPos().mX - enemyTr->GetPos().mX)) * -1.0f);
	}
	else if (tr->GetPos().mX > enemyTr->GetPos().mX && tr->GetPos().mY < enemyTr->GetPos().mX) {
		tr->SetRot(180.0f - atan(fabs(tr->GetPos().mY - enemyTr->GetPos().mY) / fabs(tr->GetPos().mX - enemyTr->GetPos().mX)));
	}
	else if (tr->GetPos().mX < enemyTr->GetPos().mX && tr->GetPos().mY < enemyTr->GetPos().mX) {
		tr->SetRot(atan(fabs(tr->GetPos().mY - enemyTr->GetPos().mY) / fabs(tr->GetPos().mX - enemyTr->GetPos().mX)));
	}
}