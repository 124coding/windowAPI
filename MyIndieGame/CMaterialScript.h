#pragma once

#include "CScript.h"

class CMaterialScript : public CScript
{
public:
	enum class eState {
		Idle,
		Follow
	};

	CMaterialScript() : CScript() {}
	virtual ~CMaterialScript() {}

	virtual CComponent* Clone() override {
		return new CMaterialScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

public:
	void SetStateFollow() {
		this->mState = eState::Follow;
	}
	void CarriedPlayer();

public:

	void SetMoney(float tMoney) {
		this->mMoney = tMoney;
	}

	void SetExp(float tExp) {
		this->mExp = tExp;
	}

	float GetMoney() {
		return this->mMoney;
	}

	float GetExp() {
		return this->mExp;
	}

private:
	eState mState = eState::Idle;

	float mMoney = 2.0f;
	float mExp = 2.0f;
};

