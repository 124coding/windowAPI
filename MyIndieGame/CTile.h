#pragma once
#include "GameObject.h"

class CTile :public GameObject
{
public:
	CTile() {}
	virtual ~CTile() {}

	CTile(const CTile& tObj);
	CTile& operator=(const CTile& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetIndexPosition(SVector2D tPos);

private:

};
