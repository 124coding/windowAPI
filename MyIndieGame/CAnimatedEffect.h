#pragma once

#include "CEffect.h"

#include "CAnimator.h"

class CAnimatedEffect : public CEffect
{
public:
	CAnimatedEffect() {
		AddComponent<CAnimator>();
	}
	virtual ~CAnimatedEffect() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	virtual void Reset(SVector2D tPos);

protected:
};

