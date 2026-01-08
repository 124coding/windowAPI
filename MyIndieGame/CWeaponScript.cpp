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

// 가장 가까운 적 탐색 및 회전 (Targeting AI)
void CWeaponScript::SetRotForClosedEnemyWatch(std::vector<GameObject*> tEnemies)
{
	GameObject* closedEnemy = nullptr;
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 1. 적이 없으면 회전값 초기화 후 리턴
	if (tEnemies.empty()) {
		mClosedEnemyPos = SVector2D(9999.0f, 9999.0f);	// 타겟 위치 초기화
		tr->SetRot(0.0f);
		return;
	}

	// 2. 가장 가까운 적 찾기 (Nearest Neighbor Search)
	for (GameObject* enemy : tEnemies) {
		// [Optimization] 거리 비교 시 sqrt를 쓰지 않고 LengthSq(거리의 제곱)를 사용하여 연산 비용 절감
		if (closedEnemy == nullptr || (enemy->GetComponent<CTransform>(eComponentType::Transform)->GetPos() - tr->GetPos()).LengthSq() < (closedEnemy->GetComponent<CTransform>(eComponentType::Transform)->GetPos() - tr->GetPos()).LengthSq()) {
			// 적의 상태 확인 (스폰 중이거나 죽은 적은 타겟 제외)
			CEnemyScript* enemySc = enemy->GetComponent<CEnemyScript>(eComponentType::Script);

			if (enemySc->GetState() == CEnemyScript::eState::Spawn || enemySc->GetState() == CEnemyScript::eState::Dead) continue;
			closedEnemy = enemy;
		}
	}

	// 유효한 타겟이 없는 경우 처리
	if (closedEnemy == nullptr) {
		mClosedEnemyPos = SVector2D(9999.0f, 9999.0f);
		tr->SetRot(0.0f);
		return;
	}

	// 3. 타겟을 바라보도록 회전 (LookAt Logic)
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
	CTransform* enemyTr = closedEnemy->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* enemySr = closedEnemy->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	SVector2D enemySize = ObjectSize(closedEnemy);

	SVector2D enemyCenterPos = ObjectCenterPos(closedEnemy);	// 적의 중심 좌표

	mClosedEnemyPos = enemyCenterPos;		// 현재 타겟 위치 갱신 (디버깅이나 투사체 발사 방향용)

	// [Trigonometry] 아크탄젠트(atan)를 이용한 각도 계산
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

// 무기 위치 동기화
void CWeaponScript::CalculatePosNextToTarget()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	// 타겟(플레이어)의 중심 위치에 오프셋(mOffset)을 더해 위치를 고정시킴
	// 오프셋은 무기가 캐릭터의 손 위치나 옆에 떠 있도록 조정하는 값
	tr->SetPos(ObjectCenterPos(mTarget) + mOffset);
}


// 최종 데미지 산출 (Damage Calculation Pipeline)
CWeaponScript::SDamageInfo CWeaponScript::ApplyDamageModifiers(float tBaseDamage)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	// 1. 퍼센트 데미지 증가 적용 (합연산)
	// 예: 데미지 증가가 50%라면, 기본 데미지 * 1.5
	int damagePercent = plSc->GetDamagePercent();

	float finalDamage = tBaseDamage * (1.0f + (damagePercent / 100.0f));

	// 2. 치명타(Critical Hit) 확률 계산
	int rand = std::rand() % 100;

	bool isCritical = false;

	// 무기 자체 치명타 확률 + 플레이어 치명타 확률 보정
	if (rand <= mCriticalChance + plSc->GetCriticalChancePercent()) {
		finalDamage *= mCriticalDamage;
		isCritical = true;
	}

	// 3. 최소 데미지 보정 (음수 데미지로 인한 힐링 방지)
	if (finalDamage < 1.0f)
	{
		finalDamage = 1.0f;
	}

	// 최종 데미지와 치명타 여부를 구조체로 반환하여 UI 출력(노란색/흰색 텍스트) 등에 활용
	return SDamageInfo{ finalDamage, isCritical };
}
