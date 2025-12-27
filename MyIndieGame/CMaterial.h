#pragma once
#include "GameObject.h"

#include "CMaterialScript.h"

class CMaterial :public GameObject
{
public:
	CMaterial() {
		AddComponent<CMaterialScript>();
	}

	virtual ~CMaterial() {}

	CMaterial(const CMaterial& tObj);
	CMaterial& operator=(const CMaterial& tObj);

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

