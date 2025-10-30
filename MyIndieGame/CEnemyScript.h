#pragma once

#include "CScript.h"

#include "GameObject.h"

class CAnimator;
class CTransform;
class CCollider;
class CPlayer;

class CEnemyScript : public CScript
{
public:
	enum class eState {
		Idle,
		Attack,
		Walk
	};

	CEnemyScript() : 
		CScript(), 
		mState(eState::Walk), 
		mAnimator(nullptr), 
		mTarget(nullptr) {}
	virtual ~CEnemyScript() {}

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

	virtual void ButtDamageToPlayer(CPlayer* tPlayer);

public:
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	GameObject* GetTarget() {
		return this->mTarget;
	}

// Move
private:
	virtual void Idle() = 0;
	virtual void Translate(CTransform* tr) = 0;

private:
	eState mState;
	CAnimator* mAnimator;
	GameObject* mTarget;
};

