#pragma once

#include "CComponent.h"

class CCollider;

class CScript : public CComponent
{
public:
	CScript() : CComponent(eComponentType::Script) {}
	~CScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);
private:

};

