#pragma once

#include "CScript.h"
#include "CInputMgr.h"

#include "CTexture.h"

#include <vector>

class CAnimator;
class CTransform;
class CCollider;
class CWeapon;

class CPlayerScript : public CScript
{
public:
	enum class eState {
		Idle,
		Walk
	};

	CPlayerScript() : 
		CScript(), 
		mState(eState::Idle), 
		mAnimator(nullptr), 
		mHP(5),
		mMaxHP(10),
		mLevel(1),
		mExp(0.0f),
		mMoney(0),
		mSpeed(100.0f) {}
	~CPlayerScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

private:
	void Idle();
	void Move();
	void Translate(CTransform* tr);
	// void GiveWater();

public:
	void IncreaseHP(int tIncreaseAmount);
	void DecreaseHP(int tDecreaseAmount);

	void SetHP(int tHP) {
		this->mHP = tHP;

		if (this->mHP > mMaxHP) {
			this->mHP = mMaxHP;
		}
	}

	int GetHP() {
		return this->mHP;
	}

	void IncreaseMaxHP(int tIncreaseAmount);
	void DecreaseMaxHP(int tDecreaseAmount);

	void SetMaxHP(int tMaxHP) {
		this->mMaxHP = tMaxHP;
	}

	int GetMaxHP() {
		return this->mMaxHP;
	}

	int GetLevel() {
		return this->mLevel;
	}

	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	void SetCanCollideEnemy(bool tCanCollideEnemey) {
		this->mCanCollideEnemy = tCanCollideEnemey;
	}

	bool GetCanCollideEnemy() {
		return this->mCanCollideEnemy;
	}

public:
	void SetEyesTexture(CTexture* tTexture) {
		this->mEyesTexture = tTexture;
	}

	void SetMouthTexture(CTexture* tTexture) {
		this->mMouthTexture = tTexture;
	}

	void SetBodyTexture(CTexture* tTexture) {
		this->mBodyTexture = tTexture;
	}

	bool PlusWeapon(CWeapon* tWeapon) {
		if (mWeapons.size() < mWeaponCount) {
			mWeapons.push_back(tWeapon);
			return true;
		}

		return false;
	}

	std::vector<CWeapon*> GetWeapons() {
		return this->mWeapons;
	}

	void SetWeaponCount(int tCount) {
		this->mWeaponCount = tCount;
	}

private:
	eState mState;
	CAnimator* mAnimator;
	CInputMgr* mInputMgr = CInputMgr::GetInst();

	int mHP;
	int mMaxHP;
	int mLevel;
	float mExp;
	int mMoney;

	float mSpeed;

	float mGracePeriod = 1.0f;
	bool mCanCollideEnemy = true;

	std::vector<CWeapon*> mWeapons;
	int mWeaponCount = 6;

	CTexture* mBaseTexture = nullptr;
	CTexture* mEyesTexture = nullptr;
	CTexture* mMouthTexture = nullptr;
	CTexture* mBodyTexture = nullptr;
};

