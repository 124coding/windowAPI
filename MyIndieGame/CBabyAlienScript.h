#pragma once

#include "CEnemyScript.h"

class CBabyAlienScript : public CEnemyScript
{
public:
	CBabyAlienScript() {}
	virtual ~CBabyAlienScript() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	// Collision
public:
	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

	// Move
private:
	virtual void Idle() override;
	virtual void Translate(CTransform* tr) override;
};

