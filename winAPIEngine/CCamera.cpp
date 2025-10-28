#include "CCamera.h"
#include "GameObject.h"

#include "CCollider.h"
#include "CTransform.h"

void CCamera::OnCreate()
{
}

void CCamera::OnDestroy()
{
}

void CCamera::OnUpdate(float tDeltaTime)
{
	if (mTarget != nullptr) {
		CTransform* targetTr = mTarget->GetComponent<CTransform>();
		CCollider* targetCollider = mTarget->GetComponent<CCollider>();

		mLookPosition = targetTr->GetPos() + targetCollider->GetOffset();
	}
	else {
		CTransform* cameraTr = GetOwner()->GetComponent<CTransform>();
		mLookPosition = cameraTr->GetPos();
	}

	mDistance = mLookPosition - (mResolution / 2.0f);
}

void CCamera::OnLateUpdate(float tDeltaTime)
{
}

void CCamera::Render(HDC tHDC)
{
}
