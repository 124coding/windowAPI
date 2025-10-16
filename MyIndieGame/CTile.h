#pragma once
#include "GameObject.h"

class CTile :public GameObject
{
public:
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetPosition(SVector2D tPos);

private:

};
