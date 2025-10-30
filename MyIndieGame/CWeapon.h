#pragma once

#include "GameObject.h"

class CPlayer;

class CWeapon : public GameObject
{
public:
	CWeapon() {}
	virtual ~CWeapon() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetPlayer(CPlayer* tPlayer) {
		this->mPlayer = tPlayer;
	}

	CPlayer* GetPlayer() {
		return this->mPlayer;
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

private:
	CPlayer* mPlayer = nullptr;

	float mDamage;
	float mDelay; // ���� �� ������ ��, ���� �ӵ�
	float mRange;
	float mSpeed; // ���� ���Ⱑ ���ư��� �ӵ� �� ���Ÿ� ������ �Ѿ��� �ӵ�
	int mTier = 1;
};

