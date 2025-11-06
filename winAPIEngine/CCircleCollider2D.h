#pragma once

#include "CCollider.h"

class CCircleCollider2D : public CCollider
{
public:
	CCircleCollider2D() : CCollider(eColliderType::Circle2D), mRadius(1.0f) {}
	virtual ~CCircleCollider2D() {}

	virtual CComponent* Clone() override {
		return new CCircleCollider2D(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	float mRadius;
};

