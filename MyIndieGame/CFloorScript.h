#pragma once

#include "CScript.h"

class CFloorScript : public CScript
{
public:
	CFloorScript() {}
	virtual ~CFloorScript() {}

	virtual CComponent* Clone() override {
		return new CFloorScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

private:

};

