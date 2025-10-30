#pragma once

#include "CComponent.h"

class CAutoSpriteFlipper : public CComponent
{
public:
	CAutoSpriteFlipper() :
		CComponent(eComponentType::AutoSpriteFlipper){
	}

	virtual ~CAutoSpriteFlipper() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	bool mFlipImage = false;
};

