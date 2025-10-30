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

private:
};

