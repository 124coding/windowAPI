#pragma once

#include "CEffect.h"

class Animator;

class CAnimatedEffect : public CEffect
{
public:
	CAnimatedEffect() {}
	virtual ~CAnimatedEffect() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

private:

};

