#pragma once

#include "CScript.h"

class CFloorScript : public CScript
{
public:
	CFloorScript() {}
	~CFloorScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(CCollider* tOther) override;
	void OnCollisionStay(CCollider* tOther) override;
	void OnCollisionExit(CCollider* tOther) override;

private:

};

