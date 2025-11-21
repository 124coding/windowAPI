#pragma once

#include "CScript.h"

#include <vector>

class GameObject;
class CEnemy;

class CWeaponScript : public CScript
{
public:
	enum class eState {
		Idle,
		Attack,
		Back
	};

	CWeaponScript() : CScript(), mState(eState::Idle) {}
	virtual ~CWeaponScript() {}

	virtual CComponent* Clone() override {
		return new CWeaponScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

	void SetRotForClosedEnemyWatch(std::vector<GameObject*> tEnemies);
	void CalculatePosNextToTarget();

public:
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	GameObject* GetTarget() {
		return this->mTarget;
	}


	void SetDamage(float tDamage) {
		this->mDamage = tDamage;
	}

	float GetDamage() {
		return this->mDamage;
	}


	void SetDelay(float tDelay) {
		this->mDelay = tDelay;
	}

	float GetDelay() {
		return this->mDelay;
	}


	void SetRange(float tRange) {
		this->mRange = tRange;
	}

	float GetRange() {
		return this->mRange;
	}


	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	void PlusTier() {
		this->mTier++;
	}

	void SetOffset(SVector2D tOffset) {
		this->mOffset = tOffset;
	}

	SVector2D GetOffset() {
		return this->mOffset;
	}

	SVector2D GetClosedEnemyPos() {
		return this->mClosedEnemyPos;
	}

protected:
	eState mState;

private:
	GameObject* mTarget = nullptr;

	float mDamage;
	float mDelay; // 공격 간 딜레이 즉, 공격 속도
	float mRange;
	float mSpeed; // 근접 무기가 날아가는 속도 겸 원거리 무기의 총알의 속도
	int mTier = 1;

	SVector2D mOffset;

	SVector2D mClosedEnemyPos;
};

