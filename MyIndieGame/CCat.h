#pragma once
#include "GameObject.h"

class CCat : public GameObject
{
public:
	CCat() {}
	~CCat() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

