#pragma once

#include "GameObject.h"

class CWeapon : public GameObject
{
public:
	CWeapon() {}
	virtual ~CWeapon() {}

	CWeapon(const CWeapon& tObj);
	CWeapon& operator=(const CWeapon& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

