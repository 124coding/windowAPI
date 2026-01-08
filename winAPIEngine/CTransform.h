#pragma once
#include "CComponent.h"

#include <Windows.h>

class CTransform : public CComponent
{
public:
	CTransform() : CComponent(eComponentType::Transform), mPosition(SVector2D(0.0f, 0.0f)), mVelocity(SVector2D(0.0f, 0.0f)), mScale(SVector2D(1.0f, 1.0f)), mRotation(0.0f) {}
	virtual ~CTransform() {}

	virtual CComponent* Clone() override {
		return new CTransform(*this);
	}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	// 위치(Position) 설정 및 반환
	void SetPos(SVector2D tPosition) {
		this->mPosition.mX = tPosition.mX;
		this->mPosition.mY = tPosition.mY;
	}

	SVector2D GetPos() {
		return this->mPosition;
	}

	// 속도(Velocity) 설정 및 반환
	// 물리 엔진이 없어도 기본적인 이동 구현을 위해 Transform이 속도 정보를 가짐
	void SetVelocity(const SVector2D& tVelocity) {
		this->mVelocity = tVelocity;
	}

	SVector2D GetVelocity() {
		return this->mVelocity;
	}

	// 크기(Scale) 설정 및 반환
	void SetScale(SVector2D tScale) {
		this->mScale = tScale;
	}

	SVector2D GetScale() {
		return this->mScale;
	}

	// 회전(Rotation) 설정 및 반환
	void SetRot(float tRotaiton) {
		this->mRotation = tRotaiton;
	}

	float GetRot() {
		return this->mRotation;
	}

private:
	SVector2D mPosition;	// 월드 좌표
	SVector2D mVelocity;	// 이동 속도 벡터
	SVector2D mScale;		// 확대/축소 비율

	float mRotation;		// 회전 각도
};

