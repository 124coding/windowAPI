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

private:
	CPlayer* mPlayer = nullptr;

	float mDamage;
	float mDelay; // 공격 간 딜레이 즉, 공격 속도
	float mRange;
	float mSpeed; // 근접 무기가 날아가는 속도 겸 원거리 무기의 총알의 속도
};

