#pragma once

#include "CScript.h"
#include "CInputMgr.h"

#include "CTexture.h"

#include <vector>
#include <map>

class CAnimator;
class CTransform;
class CCollider;
class CWeapon;

class CPlayerScript : public CScript
{
public:
	enum class eState {
		Idle,
		Walk,
		Dead
	};

	CPlayerScript() : 
		CScript(), 
		mState(eState::Idle), 
		mAnimator(nullptr), 
		mHP(10),
		mMaxHP(10),
		mHPRegeneration(0),

		mDodge(0),
		mArmor(0),
		mBasicMoveSpeed(450),
		mSpeedPercent(0),

		mRange(0),
		mDamagePer(0),
		mCriticalChancePer(0),
		mMeleeDamage(0),
		mRangedDamage(0),
		mAttackSpeedPer(0),

		mLifeSteal(0),
		
		mLevel(0),
		mExp(0.0f),
		mNeedLevelUpExp(20.0f),
		mCurStageLevelUpCount(0),
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
	void ButtDamageByEnemy(GameObject* tEnemy);
	float DecreaseDamageBecauseArmor(float tDamage);
	void Death(float tDeltaTime);
	void Acquire();

public:
	void SetState(bool tPower) {
		if (tPower) {
			mState = eState::Idle;
		}
		else {
			mState = eState::Dead;
		}
	}

	void SetStartingCharacterID(std::wstring tID) {
		this->mStartingCharacterID = tID;
	}

	std::wstring GetStartingCharacterID() {
		return this->mStartingCharacterID;
	}

	void SetStartWeaponID(std::wstring tID) {
		this->mStartWeaponID = tID;
	}

	std::wstring GetStartWeaponID() {
		return this->mStartWeaponID;
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
		this->mHPRegeneration += tIncreaseAmount * GetStatMultiplier(L"HPRegen");
	}

	int GetHPGeneration() {
		return this->mHPRegeneration;
	}


	void SetDodge(int tAmount) {
		this->mDodge = tAmount;
	}
	void IncreaseDodge(int tIncreaseAmount) {
		this->mDodge += tIncreaseAmount * GetStatMultiplier(L"Dodge");
	}

	int GetDodge() {
		return this->mDodge;
	}

	void SetDodgeLimit(int tAmount) {
		this->mDodgeLimit = tAmount;
	}



	void SetArmor(int tAmount) {
		this->mArmor = tAmount;
	}
	void IncreaseArmor(int tIncreaseAmount) {
		this->mArmor += tIncreaseAmount * GetStatMultiplier(L"Armor");
	}

	int GetArmor() {
		return this->mArmor;
	}


	void SetRange(float tAmount) {
		this->mRange = tAmount;
	}
	void IncreaseRange(float tIncreaseAmount) {
		this->mRange += tIncreaseAmount * GetStatMultiplier(L"Range");
	}

	float GetRange() {
		return this->mRange;
	}


	void SetDamagePecent(int tAmount) {
		this->mDamagePer = tAmount;
	}
	void IncreaseDamagePercent(int tIncreaseAmount) {
		this->mDamagePer += tIncreaseAmount * GetStatMultiplier(L"DamagePercent");
	}

	int GetDamagePercent() {
		return this->mDamagePer;
	}

	
	void SetCriticalChancePercent(int tAmount) {
		this->mCriticalChancePer = tAmount;
	}
	void IncreaseCriticalChancePercent(int tIncreaseAmount) {
		this->mCriticalChancePer += tIncreaseAmount * GetStatMultiplier(L"CritChancePercent");
	}

	int GetCriticalChancePercent() {
		return this->mCriticalChancePer;
	}


	void SetMeleeDamage(int tAmount) {
		this->mMeleeDamage = tAmount;
	}
	void IncreaseMeleeDamage(int tIncreaseAmount) {
		this->mMeleeDamage += tIncreaseAmount * GetStatMultiplier(L"MeleeDamage");
	}

	int GetMeleeDamage() {
		return this->mMeleeDamage;
	}


	void SetRangedDamage(int tAmount) {
		this->mRangedDamage = tAmount;
	}
	void IncreaseRangedDamage(int tIncreaseAmount) {
		this->mRangedDamage += tIncreaseAmount * GetStatMultiplier(L"RangedDamage");
	}

	int GetRangedDamage() {
		return this->mRangedDamage;
	}


	void SetAttackSpeedPercent(int tAmount) {
		this->mAttackSpeedPer = tAmount;
	}
	void IncreaseAttackSpeedPercent(int tIncreaseAmount) {
		this->mAttackSpeedPer += tIncreaseAmount * GetStatMultiplier(L"AttackSpeed");
	}

	int GetAttackSpeedPercent() {
		return this->mAttackSpeedPer;
	}

	void SetLifeSteal(int tAmount) {
		this->mLifeSteal = tAmount;
	}
	void IncreaseLifeSteal(int tIncreaseAmount) {
		this->mLifeSteal += tIncreaseAmount * GetStatMultiplier(L"LifeSteal");
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

	float GetNeedLevelUpExp() {
		return this->mNeedLevelUpExp;
	}

	void ResetCurStage() {
		this->mCurStageLevelUpCount = 0;
	}

	void MinusCurStage() {
		this->mCurStageLevelUpCount--;
	}

	int GetCurStageLevelUpCount() {
		return this->mCurStageLevelUpCount;
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

	void ChangeMoney(int tChange) {
		this->mMoney += tChange;
	}

	int GetMoney() {
		return this->mMoney;
	}


	void SetBasicMoveSpeed(int tSpeed) {
		this->mBasicMoveSpeed = tSpeed;
	}

	int GetSpeed() {
		return this->mBasicMoveSpeed;
	}


	void SetSpeedPercent(int tAmount) {
		this->mSpeedPercent = tAmount;
	}
	void IncreaseSpeedPercent(int tIncreaseAmount) {
		this->mSpeedPercent += tIncreaseAmount * GetStatMultiplier(L"SpeedPercent");
	}

	int GetSpeedPercent() {
		return this->mSpeedPercent;
	}

	void AddStatModifier(const std::wstring& statName, float percent) {
		mStatGainModifiers[statName] += percent;
	}

	float GetStatMultiplier(const std::wstring& statName) {
		if (mStatGainModifiers.find(statName) != mStatGainModifiers.end()) {
			return 1.0f + (mStatGainModifiers[statName] / 100.0f);
		}
		return 1.0f;
	}

	void SetCanCollideEnemy(bool tCanCollideEnemey) {
		this->mCanCollideEnemy = tCanCollideEnemey;
	}

	bool GetCanCollideEnemy() {
		return this->mCanCollideEnemy;
	}

	void ResetStats();

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

	float mDeadTimeTaken = 1.0f;

	int mHP;
	int mMaxHP;
	int mHPRegeneration;
	float mHPRegenTime = 0.0f;

	int mDodge;
	int mDodgeLimit = 60;
	int mArmor;
	int mBasicMoveSpeed;
	int mSpeedPercent;

	float mRange;
	int mDamagePer;
	int mCriticalChancePer;
	int mMeleeDamage;
	int mRangedDamage;
	int mAttackSpeedPer;

	int mLifeSteal;

	std::map<std::wstring, float> mStatGainModifiers;

	float mExp;
	int mLevel;
	float mNeedLevelUpExp;
	int mCurStageLevelUpCount;
	int mMoney;

	float mGracePeriod = 0.2f;
	bool mCanCollideEnemy = true;

	std::wstring mStartingCharacterID = L"";
	std::wstring mStartWeaponID = L"";

	CTexture* mBaseTexture = nullptr;
	CTexture* mEyesTexture = nullptr;
	CTexture* mMouthTexture = nullptr;
	CTexture* mClothTexture = nullptr;
	CTexture* mHairTexture = nullptr;
};

