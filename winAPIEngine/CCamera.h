#pragma once

#include "CComponent.h"

#include <vector>
#include "winMacro.h"

class CCamera : public CComponent
{
public:
	SVector2D CalculatePosition(SVector2D pos) { return pos - mDistance; }
	SVector2D CalculateTilePosition(SVector2D pos) { return pos + mDistance; }

	CCamera()
		: CComponent(eComponentType::Camera), 
		mDistance(SVector2D(0.0f, 0.0f)),
		mResolution(SVector2D(windowWidth, windowHeight)),
		mLookPosition(SVector2D(0.0f, 0.0f)),
		mTarget(nullptr) {
	}
	~CCamera() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

private:
	// std::vector<GameObject*> mGameObjects;

	SVector2D mDistance;
	SVector2D mResolution;
	SVector2D mLookPosition;

	GameObject* mTarget;
};

