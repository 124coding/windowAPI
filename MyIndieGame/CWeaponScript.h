#pragma once

#include "CScript.h"

#include <vector>

class GameObject;
class CEnemy;

class CWeaponScript : public CScript
{
public:
	// 데미지 정보를 담는 구조체
	// 단순 float 데미지만 리턴하면, 피격 시 이게 치명타인지 알 수 없어 UI 표현이 제한됨
	// 따라서 치명타 여부(isCritical)를 함께 묶어서 전달
	struct SDamageInfo {
		float damage;
		bool isCritical;
	};

	// 무기의 행동 상태 (Finite State Machine)
	// Idle: 적을 탐색하거나 대기 중
	// Attack: 공격 수행 (찌르기, 휘두르기, 발사 등)
	// Back: 공격 후 원래 위치로 복귀 (근접 무기 등)
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

	// ==========================================
	// Collision Events
	// ==========================================
	// 적과 충돌 시 데미지를 주는 로직이 들어갈 곳
	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

	// ==========================================
	// AI / Targeting Logic
	// ==========================================

	// 가장 가까운 적을 바라보도록 회전각(Rotation) 설정
	void SetRotForClosedEnemyWatch(std::vector<GameObject*> tEnemies);

	// 플레이어(mTarget) 주변을 공전하거나 위치를 고정하는 로직
	void CalculatePosNextToTarget();

public:
	// 무기의 주인(주로 플레이어) 설정
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	GameObject* GetTarget() {
		return this->mTarget;
	}


	void SetDamage(float tDamage) {
		this->mDamage = tDamage;
	}

	virtual float GetDamage() {
		return this->mDamage;
	}

	// 최종 데미지 계산 (치명타 확률 적용 포함)
	// 외부(Enemy)에서는 이 함수를 호출하여 최종 피격 데미지를 받아감
	virtual SDamageInfo GetFinalDamage() {
		return ApplyDamageModifiers(this->mDamage);
	}

	// 공격 속도 (낮을수록 빠름)
	void SetDelay(float tDelay) {
		this->mDelay = tDelay;
	}

	float GetDelay() {
		return this->mDelay;
	}

	// 사거리 (근접: 공격 범위, 원거리: 인식 범위)
	void SetRange(float tRange) {
		this->mRange = tRange;
	}

	float GetRange() {
		return this->mRange;
	}

	// 치명타 배율 (예: 1.5x, 2.0x)
	void SetCriticalDamage(float tCritDamage) {
		this->mCriticalDamage = tCritDamage;
	}

	float GetCriticalDamage() {
		return this->mCriticalDamage;
	}

	// 치명타 확률 (0.0 ~ 100.0)
	void SetCriticalChance(float tChance) {
		this->mCriticalChance = tChance;
	}

	float GetCriticalChance() {
		return this->mCriticalChance;
	}

	// 생명력 흡수율
	void SetLifeSteal(float tAmount) {
		this->mLifeSteal = tAmount;
	}

	float GetLifeSteal() {
		return this->mLifeSteal;
	}

	// 상점 가격
	void SetBasePrice(float tPrice) {
		this->mBasePrice = tPrice;
	}

	float GetBasePrice() {
		return this->mBasePrice;
	}

	// 투사체 속도 or 근접 공격 속도
	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	// 무기 등급 (Tier 1 ~ 4)
	void SetTier(int tTier) {
		this->mTier = tTier;
	}

	// 무기 합성 시 호출
	void PlusTier() {
		this->mTier++;
	}

	int GetTier() {
		return this->mTier;
	}

	// 플레이어 중심으로부터의 거리 오프셋
	void SetOffset(SVector2D tOffset) {
		this->mOffset = tOffset;
	}

	SVector2D GetOffset() {
		return this->mOffset;
	}

	// 현재 타겟팅 중인 적의 위치
	SVector2D GetClosedEnemyPos() {
		return this->mClosedEnemyPos;
	}
	
	// 내부 로직: 기본 데미지에 치명타 확률 계산을 적용하여 SDamageInfo 반환
	SDamageInfo ApplyDamageModifiers(float _baseDamage);

protected:
	eState mState;		// 현재 무기 상태 (Idle, Attack, Back)

private:
	GameObject* mTarget = nullptr;		// 무기를 들고 있는 주체 (플레이어)

	// 무기 고유 스탯들
	float mDamage;
	float mDelay; // 공격 간 딜레이 즉, 공격 속도
	float mRange;
	float mCriticalDamage;
	float mCriticalChance;
	float mLifeSteal;
	float mBasePrice;
	float mSpeed; // 근접 무기가 날아가는 속도 겸 원거리 무기의 총알의 속도
	int mTier = 1;

	SVector2D mOffset;		// 플레이어로부터 떨어진 거리

	// 가장 가까운 적의 위치 캐싱 (매 프레임 계산 최적화용 또는 타겟팅용)
	SVector2D mClosedEnemyPos = SVector2D(9999.0f, 9999.0f);
};

