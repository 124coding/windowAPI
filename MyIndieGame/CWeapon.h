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
	float mDelay; // ���� �� ������ ��, ���� �ӵ�
	float mRange;
	float mSpeed; // ���� ���Ⱑ ���ư��� �ӵ� �� ���Ÿ� ������ �Ѿ��� �ӵ�
};

