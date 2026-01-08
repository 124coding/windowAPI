#include "CTransform.h"

void CTransform::OnCreate()
{
}

void CTransform::OnDestroy()
{
}

void CTransform::OnUpdate(float tDeltaTime)
{
	// 현재 위치 = 이전 위치 + 속도 * 시간 간격
	this->mPosition = this->mPosition + mVelocity * tDeltaTime;
}

void CTransform::OnLateUpdate(float tDeltaTime)
{
}

void CTransform::Render(HDC tHDC)
{
}
