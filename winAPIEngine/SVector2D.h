#pragma once
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

	SVector2D operator-(const SVector2D& t) {
		SVector2D tResult;
		tResult.mX = this->mX - t.mX;
		tResult.mY = this->mY - t.mY;

		return tResult;
	}

	SVector2D operator*(const float& t) {
		SVector2D tResult;
		tResult.mX = this->mX * t;
		tResult.mY = this->mY * t;

		return tResult;
	}

	// ����, ������ ����� ��Į��
	float Dot(const SVector2D& t) {
		return this->mX * t.mX + this->mY * t.mY;
	}

	// ������ ũ�� ���ϱ�
	float Length() {
		// �ڱ� �ڽ��� �����ϰ� ������
		return sqrtf(Dot(*this));
	}

	// ����ȭ. ũ�Ⱑ 1�� �������� ���ϱ�
	SVector2D Normalize() {
		float tLength = Length();

		// 0���� ������ ���� ���п��� ���ǵǾ� ���� �ʴ�.
		if (tLength < FLT_EPSILON) { // ���� ���� ���� ������ ������ (��, 0���� ������ (0�̸�))
			// ������
			return SVector2D(0.0f, 0.0f);
		}

		return (*this) * (1.0f / tLength);
	}
};