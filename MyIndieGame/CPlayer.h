#pragma once
#include "GameObject.h"

#include "CPlayerScript.h"

class CPlayer :public GameObject
{
public:
	CPlayer() {
		AddComponent<CPlayerScript>();
	}

	virtual ~CPlayer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:

};

