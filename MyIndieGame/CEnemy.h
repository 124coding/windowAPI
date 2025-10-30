#pragma once
#include "GameObject.h"

class CEnemy : public GameObject
{
public:
	CEnemy() {
	}
	virtual ~CEnemy() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

public:
	void DecreaseHP(int tDecreaseAmount);

	void SetHP(int tHP) {
		this->mHP = tHP;
	}

	int GetHP() {
		return this->mHP;
	}

	void SetButtDamage(int tButtDamage) {
		this->mButtDamage = tButtDamage;
	}

	int GetButtDamage() {
		return this->mButtDamage;
	}

	void SetSpeed(float tSpeed) {
		this->mSpeed = tSpeed;
	}

	float GetSpeed() {
		return this->mSpeed;
	}

	void SetDistanceToPlayer(float tDistance) {
		this->mDistanceToPlayer = tDistance;
	}

	float GetDistanceToPlayer() {
		return this->mDistanceToPlayer;
	}

private:
	int mHP = 0;

	int mButtDamage = 0;

	float mSpeed = 0.0f;

	float mDistanceToPlayer = 9999.9f;
};

