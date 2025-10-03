#pragma once

#include "CComponent.h"

class CScript : public CComponent
{
public:
	CScript() : CComponent(eComponentType::Script) {}
	~CScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;
private:

};

