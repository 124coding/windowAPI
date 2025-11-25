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
		Spawn,
		Idle,
		Attack,
		Dead,
		Walk
	};

	CEnemyScript() : 
		CScript(), 
		mState(eState::Spawn), 
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
	virtual void DamageByWeapon(GameObject* tWeapon);

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

	void SetAttackDamage(int tAttackDamage) {
		this->mAttackDamage = tAttackDamage;
	}

	int GetAttackDamage() {
		return this->mAttackDamage;
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

	void SetBaseTexture(CTexture* tTex) {
		this->mBaseTexture = tTex;
	}

	eState GetState() {
		return this->mState;
	}

// Move
private:
	virtual void Idle() = 0;
	virtual void Translate(CTransform* tr) = 0;
	void Spawn(float tDeltaTime);

protected:
	eState mState;

private:
	CAnimator* mAnimator;
	GameObject* mTarget;

	float mSpawnDelay = 2.0f;

	int mHP = 0;
	int mButtDamage = 0;
	int mAttackDamage = 0;
	float mSpeed = 0.0f;

	float mDistanceToPlayer = 9999.9f;

	float mBlinkTime = 3.0f;
	float mCurrentBlinkAlpha = 1.0f;
	float mBlinkSpeed = (static_cast<float>(std::rand() % 20) / 10.0f) + 0.5f;
	bool mBlinkFadeOut = true;

	CTexture* mBaseTexture = nullptr;
	float mTextureChangeDelay = 0.0f;
};

