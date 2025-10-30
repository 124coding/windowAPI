#include "CTitleBrotato.h"

#include "GameObject.h"

#include "CTransform.h"

void CTitleBrotato::OnCreate()
{
	SetBobbingSpeed(1.0f);
	SetSquashMagnitude(0.1f);
}

void CTitleBrotato::OnDestroy()
{
}

void CTitleBrotato::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

	Bounce();
}

void CTitleBrotato::OnLateUpdate(float tDeltaTime)
{
}

void CTitleBrotato::Render(HDC tHDC)
{
}
