#pragma once

#include "CComponent.h"

class CCollider;

class CScript : public CComponent
{
public:
	CScript() : CComponent(eComponentType::Script) {}
	virtual ~CScript() {}

	virtual CComponent* Clone() override {
		return new CScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// ==========================================
	// Collision Callbacks
	// ==========================================
	// CCollisionMgr나 CCollider에서 충돌 감지 시 이 함수들을 호출함
	// 자식 클래스(예: CMonsterScript)에서 이 함수들을 오버라이딩(재정의)하여 로직 구현
	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);

	virtual void Bounce();

	void SetBobbingSpeed(float tBobbingSpeed) {
		this->mBobbingSpeed = tBobbingSpeed;
	}

	// 찌그러짐/늘어남 연출 강도 설정
	void SetSquashMagnitude(float tSquashMagnitude) {
		this->mSquashMagnitude = tSquashMagnitude;
	}
	
protected:
	float mTotalTime = 0.0f;	// 누적 시간 (사인파 이동 등에 사용)

private:
	float mBobbingSpeed;
	float mSquashMagnitude;
};

