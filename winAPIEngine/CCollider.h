#pragma once

#include "CComponent.h"

class CCollider : public CComponent
{
public:
	CCollider() : CComponent(eComponentType::Collider) {}
	virtual ~CCollider() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetOffset(SVector2D tOffset) {
		this->mOffset = tOffset;
	}

	SVector2D GetOffset() {
		return this->mOffset;
	}

private:
	SVector2D mOffset;
};

