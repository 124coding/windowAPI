#include "CTransform.h"

void CTransform::OnCreate()
{
}

void CTransform::OnDestroy()
{
}

void CTransform::OnUpdate(float tDeltaTime)
{
	this->mPosition = this->mPosition + mVelocity * tDeltaTime;
}

void CTransform::OnLateUpdate(float tDeltaTime)
{
}

void CTransform::Render(HDC tHDC)
{
}
