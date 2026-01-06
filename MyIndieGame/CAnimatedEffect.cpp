#include "CAnimatedEffect.h"

#include "CRenderer.h"

void CAnimatedEffect::OnCreate()
{
	CEffect::OnCreate();
}

void CAnimatedEffect::OnDestroy()
{
	CEffect::OnDestroy();
}

void CAnimatedEffect::OnUpdate(float tDeltaTime)
{
	CEffect::OnUpdate(tDeltaTime);
}

void CAnimatedEffect::OnLateUpdate(float tDeltaTime)
{
	CEffect::OnLateUpdate(tDeltaTime);
}

void CAnimatedEffect::Render(HDC tHDC)
{
	CEffect::Render(tHDC);
}

void CAnimatedEffect::Reset(SVector2D tPos)
{
	mCurTime = 0.0f;
	CTransform* tr = this->GetComponent<CTransform>(eComponentType::Transform);
	tr->SetPos(mainCamera->CalculatePosition(tPos));
	mStartPos = tPos;

	tr->SetScale(SVector2D(1.0f, 1.0f));

	SetState(true);
}
