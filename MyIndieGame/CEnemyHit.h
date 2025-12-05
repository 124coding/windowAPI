#pragma once

#include "CAnimatedEffect.h"

class CEnemyHit : public CAnimatedEffect
{
public:
	CEnemyHit() {}
	virtual ~CEnemyHit() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	virtual void Reset(SVector2D tPos) override;
};

