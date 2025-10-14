#pragma once

#include "CCollider.h"

class CBoxCollider2D : public CCollider
{
public:
	CBoxCollider2D() : CCollider(), mSize(SVector2D()) {}
	virtual ~CBoxCollider2D() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	SVector2D mSize;
};

