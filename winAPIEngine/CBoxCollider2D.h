#pragma once

#include "CCollider.h"

class CBoxCollider2D : public CCollider
{
public:
	CBoxCollider2D() : CCollider(eColliderType::Rect2D) {}
	virtual ~CBoxCollider2D() {}

	virtual CComponent* Clone() override {
		return new CBoxCollider2D(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
};

