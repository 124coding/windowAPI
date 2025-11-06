#pragma once
#include "GameObject.h"

class CEnemy : public GameObject
{
public:
	CEnemy() {
	}
	virtual ~CEnemy() {}

	CEnemy(const CEnemy& tObj);
	CEnemy& operator=(const CEnemy& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

