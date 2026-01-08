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
	// 플레이어 상태 머신 (FSM)
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
		mMoney(1000) {}
	virtual ~CPlayerScript() {}

	virtual CComponent* Clone() override {
		return new CPlayerScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// ==========================================
	// Collision Events (피격 판정)
	// ==========================================
	// 적과 충돌 시 데미지 입음, 아이템 충돌 시 획득 처리
	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

private:
	// ==========================================
	// Private Helper Methods (FSM Actions)
	// ==========================================
	void Idle();	// 대기 상태 로직
	void Move();	// 키 입력에 따른 이동 로직
	void Translate(CTransform* tr);		// 실제 좌표 이동 및 화면 밖 제한 처리

	// 피격 및 데미지 공식 적용
	void ButtDamageByEnemy(GameObject* tEnemy);			// 적과 부딪혔을 때
	float DecreaseDamageBecauseArmor(float tDamage);	// 방어력에 따른 데미지 감소 계산
	void Death(float tDeltaTime);	// 사망 처리 및 연출
	void Acquire();					// 아이템 획득 범위 체크 (자석 기능 등)

public:
	// 게임 재시작 시 상태 초기화
	void SetState(bool tPower) {
		if (tPower) {
			mState = eState::Idle;
		}
		else {
			mState = eState::Dead;
		}
	}

	// 캐릭터 선택창에서 고른 데이터 저장
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

	// [HP Management]
	void SetHP(int tHP) {
		this->mHP = tHP;

		// 오버힐 방지
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

	// [Stat Modifier System]
	// 아이템 효과로 인해 스탯 증가량이 %로 변동될 때 이를 처리하는 시스템
	// 예: "경험치 획득량 20% 증가" 아이템을 먹으면 IncreaseExp() 호출 시 Multiplier가 적용됨

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

	// [Stat Modifier Core Logic]
	// 특정 스탯(Key)에 대한 퍼센트 보정치를 저장
	void AddStatModifier(const std::wstring& statName, float percent) {
		mStatGainModifiers[statName] += percent;
	}

	// 보정치를 적용한 배율 반환 (기본 1.0f)
	float GetStatMultiplier(const std::wstring& statName) {
		if (mStatGainModifiers.find(statName) != mStatGainModifiers.end()) {
			return 1.0f + (mStatGainModifiers[statName] / 100.0f);
		}
		return 1.0f;
	}

	// 무적 시간(Grace Period) 관리
	void SetCanCollideEnemy(bool tCanCollideEnemey) {
		this->mCanCollideEnemy = tCanCollideEnemey;
	}

	bool GetCanCollideEnemy() {
		return this->mCanCollideEnemy;
	}

	// 게임 리셋 시 모든 스탯 초기화
	void ResetStats();

public:
	// ==========================================
	// Composite Sprite Rendering (파츠 조합)
	// ==========================================

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
	eState mState;									// FSM 현재 상태
	CAnimator* mAnimator;
	CInputMgr* mInputMgr = CInputMgr::GetInst();	// 입력 관리자

	float mDeadTimeTaken = 1.0f;					// 사망 애니메이션 시간

	// [RPG Stats]
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

	// [Leveling]
	float mExp;
	int mLevel;
	float mNeedLevelUpExp;
	int mCurStageLevelUpCount;
	int mMoney;

	// [Collision]
	float mGracePeriod = 0.2f;		// 피격 후 무적 시간
	bool mCanCollideEnemy = true;

	// [Character Data]
	std::wstring mStartingCharacterID = L"";
	std::wstring mStartWeaponID = L"";

	// [Rendering Parts]
	CTexture* mBaseTexture = nullptr;
	CTexture* mEyesTexture = nullptr;
	CTexture* mMouthTexture = nullptr;
	CTexture* mClothTexture = nullptr;
	CTexture* mHairTexture = nullptr;
};

