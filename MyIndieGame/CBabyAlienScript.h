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
	virtual void OnCollisionEnter(CCollider* tOther) override;
	virtual void OnCollisionStay(CCollider* tOther) override;
	virtual void OnCollisionExit(CCollider* tOther) override;	

	// Move
private:
	virtual void Idle() override;
	virtual void Move(float tDeltaTime) override;
	virtual void Translate(CTransform* tr) override;
};

