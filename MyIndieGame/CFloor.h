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

	CFloor(const CFloor& tObj);
	CFloor& operator=(const CFloor& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

