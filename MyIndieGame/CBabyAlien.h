#pragma once

#include "CEnemy.h"

#include "CBabyAlienScript.h"

class CBabyAlien : public CEnemy
{
public:
	CBabyAlien() {
		this->AddComponent<CBabyAlienScript>();
	}
	virtual ~CBabyAlien() {}

	CBabyAlien(const CBabyAlien& tObj);
	CBabyAlien& operator=(const CBabyAlien& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;
private:

};

