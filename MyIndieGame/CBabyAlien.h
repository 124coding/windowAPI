#pragma once

#include "CEnemy.h"

class CBabyAlien : public CEnemy
{
public:
	CBabyAlien() {
	}
	virtual ~CBabyAlien() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;
private:

};

