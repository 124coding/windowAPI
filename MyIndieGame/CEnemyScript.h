#pragma once

#include "CScript.h"

#include "GameObject.h"

class CAnimator;
class CTransform;
class CCollider;

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

	virtual void ButtDamageToPlayer(GameObject* tPlayer);

public:
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	GameObject* GetTarget() {
		return this->mTarget;
	}


	void DecreaseHP(int tDecreaseAmount);

	void SetHP(int tHP) {
		this->mHP = tHP;
	}

	int GetHP() {
		return this->mHP;
	}

	void SetButtDamage(int tButtDamage) {
		this->mButtDamage = tButtDamage;
	}

	int GetButtDamage() {
		return this->mButtDamage;
	}

	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	void SetDistanceToPlayer(float tDistance) {
		this->mDistanceToPlayer = tDistance;
	}

	float GetDistanceToPlayer() {
		return this->mDistanceToPlayer;
	}

// Move
private:
	virtual void Idle() = 0;
	virtual void Translate(CTransform* tr) = 0;

private:
	eState mState;
	CAnimator* mAnimator;
	GameObject* mTarget;

	int mHP = 0;
	int mButtDamage = 0;
	float mSpeed = 0.0f;
	float mDistanceToPlayer = 9999.9f;
};

