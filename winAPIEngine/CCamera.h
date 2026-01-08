#pragma once

#include "CComponent.h"

#include <vector>
#include "winMacro.h"

class CCamera : public CComponent
{
public:

	// ==========================================
	// Coordinate Conversion (좌표 변환)
	// ==========================================
	
	// [월드 좌표 -> 화면 좌표]
	SVector2D CalculatePosition(SVector2D pos) { return pos - mDistance; }

	// [화면 좌표 -> 월드 좌표]
	SVector2D CalculateTilePosition(SVector2D pos) { return pos + mDistance; }

	CCamera()
		: CComponent(eComponentType::Camera), 
		mDistance(SVector2D(0.0f, 0.0f)),
		mResolution(SVector2D(windowWidth, windowHeight)),
		mLookPosition(SVector2D(0.0f, 0.0f)),
		mTarget(nullptr) {
	}
	virtual ~CCamera() {}

	virtual CComponent* Clone() override {
		return new CCamera(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// 카메라가 따라다닐 대상을 설정 (주로 플레이어)
	void SetTarget(GameObject* tTarget) {
		this->mTarget = tTarget;
	}

	SVector2D GetPosition() {
		return this->mLookPosition;
	}

private:
	// 카메라의 좌상단(Left-Top) 기준 월드 좌표
	// 렌더링 시 모든 오브젝트 위치에서 이 값을 빼줌
	SVector2D mDistance;

	// 화면 해상도 (중앙 정렬 계산용)
	SVector2D mResolution;

	// 카메라가 바라보는 지점 (보통 Target의 중심)
	SVector2D mLookPosition;

	// 추적 대상
	GameObject* mTarget;
};

