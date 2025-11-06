#pragma once
#include "GameObject.h"
#include "CCatScript.h"


class CCat : public GameObject
{
public:
	CCat() {}
	virtual ~CCat() {}

	CCat(const CCat& tObj);
	CCat& operator=(const CCat& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

