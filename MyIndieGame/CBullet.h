#pragma once

#include "CWeapon.h"

class CBulletScript;

class CBullet : public CWeapon
{
public:
	CBullet() {
		AddComponent<CBulletScript>();
	}
	virtual ~CBullet() {}

	CBullet(const CBullet& tObj);
	CBullet& operator=(const CBullet& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

