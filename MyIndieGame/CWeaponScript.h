#pragma once

#include "CScript.h"

#include <vector>

class CEnemy;

class CWeaponScript : public CScript
{
public:
	CWeaponScript() : CScript() {}
	virtual ~CWeaponScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

	void SetRotForClosedEnemyWatch(std::vector<GameObject*> tEnemies);

private:
};

