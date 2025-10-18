#pragma once
#include "GameObject.h"

#include "CFloorScript.h"

class CFloor : public GameObject
{
public:
	CFloor() {
		AddComponent<CFloorScript>();
	}
	virtual ~CFloor() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

