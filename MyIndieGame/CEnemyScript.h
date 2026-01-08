#pragma once

#include "CScript.h"

#include "GameObject.h"

class CAnimator;
class CTransform;
class CCollider;

class CEnemyScript : public CScript
{
public:
	// 적의 행동 상태
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


	// 무기에 피격되었을 때 호출되는 함수
	// 데미지 계산, 넉백(Knockback), 피격 이펙트 등을 처리
	virtual void DamageByWeapon(GameObject* tWeapon);

public:
	// 타겟(주로 플레이어) 설정
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	GameObject* GetTarget() {
		return this->mTarget;
	}

	// HP 관리
	void DecreaseHP(int tDecreaseAmount);

	void SetHP(int tHP) {
		this->mHP = tHP;
	}

	int GetHP() {
		return this->mHP;
	}


	// 공격력 및 이동 속도
	void SetDamage(float tDamage) {
		this->mDamage = tDamage;
	}

	float GetDamage() {
		return this->mDamage;
	}

	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	// 렌더링용 기본 텍스처 설정
	void SetBaseTexture(CTexture* tTex) {
		this->mBaseTexture = tTex;
	}

	eState GetState() {
		return this->mState;
	}


private:
	// ==========================================
	// Abstract Methods (순수 가상 함수)
	// ==========================================
	// 구체적인 몬스터(Boss, Normal, SpeedMob 등)마다 이동 방식이 다르므로,
	// 자식 클래스에서 반드시 구현해야 하는 인터페이스 함수들
	virtual void Idle() = 0;
	virtual void Translate(CTransform* tr) = 0;

	// ==========================================
	// Shared Logic (공통 로직)
	// ==========================================
	// 모든 몬스터가 공유하는 생성/사망 연출
	void Spawn(float tDeltaTime);
	void Death(float tDeltaTime);

protected:
	eState mState;	// 현재 상태

private:
	CAnimator* mAnimator;
	GameObject* mTarget;					// 추적 대상

	float mDeadTimeTaken = 1.0f;			// 사망 연출 소요 시간

	// 몬스터 스탯
	int mHP = 0;
	float mDamage = 0;
	float mSpeed = 0.0f;

	// [Visual Effect] 생성 시 깜빡임(Blink) 효과 제어 변수	
	float mBlinkTime = 2.0f;
	float mCurrentBlinkAlpha = 1.0f;
	float mBlinkSpeed = (static_cast<float>(std::rand() % 20) / 10.0f) + 0.5f;
	bool mBlinkFadeOut = true;

	CTexture* mBaseTexture = nullptr;
	float mTextureChangeDelay = 0.0f;
};

