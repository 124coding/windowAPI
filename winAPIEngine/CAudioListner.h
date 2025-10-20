#pragma once

#include "CComponent.h"

class CAudioListner : public CComponent
{
public:
	CAudioListner() : CComponent(eComponentType::AudioListner) {}
	virtual ~CAudioListner() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:

};

