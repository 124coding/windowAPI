#pragma once
#include "GameObject.h"

#include "CPlayerScript.h"

#include <vector>

class CWeapon;

class CPlayer :public GameObject
{
public:
	CPlayer() : 
		mHP(5), 
		mMaxHP(10), 
		mLevel(1), 
		mExp(0.0f),
		mMoney(0),
		mSpeed(100.0f) {
		AddComponent<CPlayerScript>();
	}

	virtual ~CPlayer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

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

	CTexture* mEyesTexture = nullptr;
	CTexture* mMouthTexture = nullptr;
	CTexture* mBodyTexture = nullptr;
};

