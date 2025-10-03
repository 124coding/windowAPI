#pragma once

#include "CScript.h"

class CPlayerScript : public CScript
{
public:
	CPlayerScript() : CScript() {}
	~CPlayerScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;
private:

};

