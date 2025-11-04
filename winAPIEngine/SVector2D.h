#pragma once

#include "winMacro.h"

#include <math.h>
#include <cfloat>

struct SVector2D {
public:
	float mX = 0.0f;
	float mY = 0.0f;

	SVector2D() :mX(0.0f), mY(0.0f) {}
	SVector2D(float tX, float tY) :mX(tX), mY(tY) {  }

	SVector2D(const SVector2D& t) {
		this->mX = t.mX;
		this->mY = t.mY;
	}

	void operator=(const SVector2D& t) {
		this->mX = t.mX;
		this->mY = t.mY;
	}

	SVector2D operator+(const SVector2D& t) {
		SVector2D tResult;
		tResult.mX = this->mX + t.mX;
		tResult.mY = this->mY + t.mY;

		return tResult;
	}

	SVector2D operator-() {
		return SVector2D(-this->mX, -this->mY);
	}

	SVector2D operator-(const SVector2D& t) {
		SVector2D tResult;
		tResult.mX = this->mX - t.mX;
		tResult.mY = this->mY - t.mY;

		return tResult;
	}

	SVector2D operator*(const SVector2D& t) {
		SVector2D tResult;
		tResult.mX = this->mX * t.mX;
		tResult.mY = this->mY * t.mY;

		return tResult;
	}

	SVector2D operator*(const float& t) {
		SVector2D tResult;
		tResult.mX = this->mX * t;
		tResult.mY = this->mY * t;

		return tResult;
	}

	SVector2D operator/(const float& t) {
		SVector2D tResult;
		tResult.mX = this->mX * (1 / t);
		tResult.mY = this->mY * (1 / t);

		return tResult;
	}

	SVector2D operator/(const SVector2D& t) {
		SVector2D tResult;
		tResult.mX = this->mX / t.mX;
		tResult.mY = this->mY / t.mY;

		return tResult;
	}

	// 내적, 내적의 결과는 스칼라
	float Dot(const SVector2D& t) {
		return this->mX * t.mX + this->mY * t.mY;
	}

	// 벡터의 크기 구하기
	float LengthSq() {
		return Dot(*this);
	}

	// 벡터의 크기 구하기
	float Length() {
		// 자기 자신을 내적하고 제곱근
		return sqrtf(Dot(*this));
	}

	// 정규화. 크기가 1인 단위벡터 구하기
	SVector2D Normalize() {
		float tLength = Length();

		// 0으로 나누는 것은 수학에서 정의되어 있지 않다.
		if (tLength < FLT_EPSILON) { // 아주 극히 작은 수보다 작으면 (즉, 0보다 작으면 (0이면))
			// 영벡터
			return SVector2D(0.0f, 0.0f);
		}

		return (*this) * (1.0f / tLength);

	}

	// 높이 벡터
	SVector2D GetHeightVector(float tRotate) {
		SVector2D tResult;
		tResult.mX = this->mY * cosf(DegToRad(tRotate - 90.0f)) / 2;
		tResult.mY = this->mY * sinf(DegToRad(tRotate - 90.0f)) / 2;

		return tResult;
	}

	// 너비 벡터
	SVector2D GetWidthVector(float tRotate) {
		SVector2D tResult;
		tResult.mX = this->mX * cosf(DegToRad(tRotate)) / 2;
		tResult.mY = this->mX * sinf(DegToRad(tRotate)) / 2;

		return tResult;
	}

	// 거리 벡터
	SVector2D GetDistanceVector(const SVector2D& t) {
		SVector2D tResult;

		tResult.mX = t.mX - this->mX;
		tResult.mY = t.mY - this->mY;

		return tResult;
	}

	//// 회전
	//SVector2D Rotate(float tDegree) {
	//	float radian = (tDegree / 180.0f) * PI;

	//	this->Normalize();
	//	float x = cosf(radian) * this->mX - sinf(radian) * this->mY;
	//	float y = sinf(radian) * this->mX + cosf(radian) * this->mY;

	//	return SVector2D(x, y);
	//}
};