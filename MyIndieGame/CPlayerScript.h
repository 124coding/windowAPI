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
		mHP(10),
		mMaxHP(10),
		mHPRegeneration(0),

		mDodge(0.0f),
		mArmor(0.0f),
		mBasicMoveSpeed(450.0f),
		mSpeedPercent(0.0f),

		mRange(0.0f),
		mDamagePer(0.0f),
		mCriticalChancePer(0.0f),
		mMeleeDamage(0),
		mRangedDamage(0),
		mAttackSpeedPer(0.0f),

		mLifeSteal(0.0f),
		
		mLevel(0),
		mExp(0.0f),
		mMoney(0) {}
	virtual ~CPlayerScript() {}

	virtual CComponent* Clone() override {
		return new CPlayerScript(*this);
	}

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
	void SetStartingCharacterID(std::wstring tCharacterID) {
		this->mStartingCharacterID = tCharacterID;
	}

	std::wstring GetStartingCharacterID() {
		return this->mStartingCharacterID;
	}

	void SetHP(int tHP) {
		this->mHP = tHP;

		if (this->mHP > mMaxHP) {
			this->mHP = mMaxHP;
		}
	}

	void IncreaseHP(int tIncreaseAmount);

	int GetHP() {
		return this->mHP;
	}



	void SetMaxHP(int tAmount) {
		this->mMaxHP = tAmount;
	}
	void IncreaseMaxHP(int tIncreaseAmount);

	int GetMaxHP() {
		return this->mMaxHP;
	}

	void SetHPGeneration(int tAmount) {
		this->mHPRegeneration = tAmount;
	}

	void InCreaseHPGeneration(int tIncreaseAmount) {
		this->mHPRegeneration += tIncreaseAmount;
	}

	int GetHPGeneration() {
		return this->mHPRegeneration;
	}


	void SetDodge(int tAmount) {
		this->mDodge = tAmount;
	}
	void IncreaseDodge(int tIncreaseAmount) {
		this->mDodge += tIncreaseAmount;
	}

	int GetDodge() {
		return this->mDodge;
	}



	void SetArmor(int tAmount) {
		this->mArmor = tAmount;
	}
	void IncreaseArmor(int tIncreaseAmount) {
		this->mArmor += tIncreaseAmount;
	}

	int GetArmor() {
		return this->mArmor;
	}


	void SetRange(int tAmount) {
		this->mRange = tAmount;
	}
	void IncreaseRange(int tIncreaseAmount) {
		this->mRange += tIncreaseAmount;
	}

	int GetRange() {
		return this->mRange;
	}


	void SetDamagePecent(int tAmount) {
		this->mDamagePer = tAmount;
	}
	void IncreaseDamagePercent(int tIncreaseAmount) {
		this->mDamagePer += tIncreaseAmount;
	}

	int GetDamagePercent() {
		return this->mDamagePer;
	}

	
	void SetCriticalChancePercent(int tAmount) {
		this->mCriticalChancePer = tAmount;
	}
	void IncreaseCriticalChancePercent(int tIncreaseAmount) {
		this->mCriticalChancePer += tIncreaseAmount;
	}

	int GetCriticalChancePercent() {
		return this->mCriticalChancePer;
	}


	void SetMeleeDamage(int tAmount) {
		this->mMeleeDamage = tAmount;
	}
	void IncreaseMeleeDamage(int tIncreaseAmount) {
		this->mMeleeDamage += tIncreaseAmount;
	}

	int GetMeleeDamage() {
		return this->mMeleeDamage;
	}


	void SetRangedDamage(int tAmount) {
		this->mRangedDamage = tAmount;
	}
	void IncreaseRangedDamage(int tIncreaseAmount) {
		this->mRangedDamage += tIncreaseAmount;
	}

	int GetRangedDamage() {
		return this->mRangedDamage;
	}


	void SetAttackSpeedPercent(int tAmount) {
		this->mAttackSpeedPer = tAmount;
	}
	void IncreaseAttackSpeedPercent(int tIncreaseAmount) {
		this->mAttackSpeedPer += tIncreaseAmount;
	}

	int GetAttackSpeedPercent() {
		return this->mAttackSpeedPer;
	}

	void SetLifeSteal(int tAmount) {
		this->mLifeSteal = tAmount;
	}
	void IncreaseLifeSteal(int tIncreaseAmount) {
		this->mLifeSteal += tIncreaseAmount;
	}

	int GetLifeSteal() {
		return this->mLifeSteal;
	}


	void SetExp(float tAmount) {
		this->mExp = tAmount;
	}

	void IncreasedExp(float tIncreaseAmount) {
		this->mExp += tIncreaseAmount;
	}

	float GetExp() {
		return this->mExp;
	}

	void SetLevel(int tAmount) {
		this->mLevel = tAmount;
	}

	void IncreaseLevel() {
		this->mLevel++;
	}

	int GetLevel() {
		return this->mLevel;
	}

	int GetMoney() {
		return this->mMoney;
	}


	void SetBasicMoveSpeed(float tSpeed) {
		this->mBasicMoveSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mBasicMoveSpeed;
	}


	void SetSpeedPercent(int tAmount) {
		this->mSpeedPercent = tAmount;
	}
	void IncreaseSpeedPercent(int tIncreaseAmount) {
		this->mSpeedPercent += tIncreaseAmount;
	}

	int GetSpeedPercent() {
		return this->mSpeedPercent;
	}



	void SetCanCollideEnemy(bool tCanCollideEnemey) {
		this->mCanCollideEnemy = tCanCollideEnemey;
	}

	bool GetCanCollideEnemy() {
		return this->mCanCollideEnemy;
	}

	void SetStartingWeaponID(const std::wstring& tID) {
		this->mStartingWeaponID = tID;
	}

	std::wstring GetStartingWeaponID() {
		return this->mStartingWeaponID;
	}

public:
	void SetBaseTexture(CTexture* tTexture) {
		this->mBaseTexture = tTexture;
	}

	CTexture* GetBaseTexture() {
		return this->mBaseTexture;
	}

	void SetEyesTexture(CTexture* tTexture) {
		this->mEyesTexture = tTexture;
	}

	CTexture* GetEyesTexture() {
		return this->mEyesTexture;
	}

	void SetMouthTexture(CTexture* tTexture) {
		this->mMouthTexture = tTexture;
	}

	CTexture* GetMouthTexture() {
		return this->mMouthTexture;
	}

	void SetHairTexture(CTexture* tTexture) {
		this->mHairTexture = tTexture;
	}

	CTexture* GetHairTexture() {
		return this->mHairTexture;
	}

	void SetClothTexture(CTexture* tTexture) {
		this->mClothTexture = tTexture;
	}

	CTexture* GetClothTexture() {
		return this->mClothTexture;
	}

private:
	eState mState;
	CAnimator* mAnimator;
	CInputMgr* mInputMgr = CInputMgr::GetInst();

	int mHP;
	int mMaxHP;
	int mHPRegeneration;


	int mDodge;
	int mArmor;
	int mBasicMoveSpeed;
	int mSpeedPercent;

	int mRange;
	int mDamagePer;
	int mCriticalChancePer;
	int mMeleeDamage;
	int mRangedDamage;
	int mAttackSpeedPer;

	int mLifeSteal;

	float mExp;
	int mLevel;
	int mMoney;

	float mGracePeriod = 0.2f;
	bool mCanCollideEnemy = true;

	std::wstring mStartingWeaponID = L"";
	std::wstring mStartingCharacterID = L"";

	CTexture* mBaseTexture = nullptr;
	CTexture* mEyesTexture = nullptr;
	CTexture* mMouthTexture = nullptr;
	CTexture* mClothTexture = nullptr;
	CTexture* mHairTexture = nullptr;
};

