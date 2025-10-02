#pragma once
#include "GameObject.h"

class CPlayer :public GameObject
{
public:
	void OnCreate() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;

private:

};

