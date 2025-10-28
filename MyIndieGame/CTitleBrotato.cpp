#include "CTitleBrotato.h"

#include "GameObject.h"

#include "CTransform.h"

void CTitleBrotato::OnCreate()
{
}

void CTitleBrotato::OnDestroy()
{
}

void CTitleBrotato::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	float bobbingFactor = fabs(sin(mTotalTime * mBobbingSpeed));

	float scaleY = 1.0f - (bobbingFactor * mSquashMagnitude);

	float scaleX = 1.0f + (bobbingFactor * mSquashMagnitude);

	tr->SetScale(SVector2D(scaleX, scaleY));
}

void CTitleBrotato::OnLateUpdate(float tDeltaTime)
{
}

void CTitleBrotato::Render(HDC tHDC)
{
}
