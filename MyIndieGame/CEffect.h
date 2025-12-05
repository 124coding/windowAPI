#pragma once

#include "GameObject.h"

class CEffect : public GameObject
{
public:
	CEffect() {}
	virtual ~CEffect() {}

	CEffect(const CEffect& tEffect);
	CEffect& operator=(const CEffect& tEffect);

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	virtual void Reset(SVector2D tPos) = 0;
protected:
	SVector2D mStartPos;

protected:
	float mLifeTime = 0.0f;
	float mCurTime = 0.0f;
};