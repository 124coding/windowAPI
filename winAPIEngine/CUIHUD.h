#pragma once

#include "CUIBase.h"

#include "GameObject.h"

class CUIHUD : public CUIBase
{
public:
	CUIHUD(SVector2D tPos,
		float tWidth, float tHeight) 
		: CUIBase(tPos,
			tWidth, tHeight, eUIType::None) {}
	virtual ~CUIHUD() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

private:
};