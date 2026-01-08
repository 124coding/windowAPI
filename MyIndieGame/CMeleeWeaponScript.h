#pragma once

#include "CWeaponScript.h"

class CMeleeWeaponScript : public CWeaponScript
{
public:
	CMeleeWeaponScript() {}
	virtual ~CMeleeWeaponScript() {}

	virtual CComponent* Clone() override {
		return new CMeleeWeaponScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

public:
	// ==========================================
	// Attack State Logic (FSM)
	// ==========================================

	// [Condition: Idle -> Attack]
	// 사거리 내에 적이 있고, 쿨타임이 돌았는지 확인하여 공격 상태로 전이
	void CanAttackCheck(std::vector<GameObject*> tEnemies);

	// [Condition: Attack -> Back]
	// 무기가 최대 사거리까지 뻗었는지 확인하여 복귀 상태로 전이
	void AttackEndCheck();

	// [State: Back]
	// 공격 후 플레이어의 손 위치(원점)로 선형 보간(Lerp)하거나 벡터 이동하여 복귀
	// 복귀 완료 시 다시 Idle 상태로 전환
	void BackToPlayer();

private:
};

