#include "CCamera.h"
#include "CTransform.h"
#include "GameObject.h"

void CCamera::OnCreate()
{
}

void CCamera::OnDestroy()
{
}

void CCamera::OnUpdate(float tDeltaTime)
{
	if (mTarget != nullptr) {
		CTransform* tr = mTarget->GetComponent<CTransform>();
		mLookPosition = tr->GetPos();
	}

	CTransform* cameraTr = GetOwner()->GetComponent<CTransform>();
	mLookPosition = cameraTr->GetPos();

	mDistance = mLookPosition - (mResolution / 2.0f);
}

void CCamera::OnLateUpdate(float tDeltaTime)
{
}

void CCamera::Render(HDC tHDC)
{
}
