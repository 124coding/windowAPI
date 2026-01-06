#include "CCamera.h"
#include "GameObject.h"

#include "CCollider.h"
#include "CTransform.h"

#include "CTilemapRenderer.h"

void CCamera::OnCreate()
{
}

void CCamera::OnDestroy()
{
}

void CCamera::OnUpdate(float tDeltaTime)
{
	if (mTarget != nullptr) {
		CTransform* targetTr = mTarget->GetComponent<CTransform>(eComponentType::Transform);
		CCollider* targetCollider = mTarget->GetComponent<CCollider>(eComponentType::Collider);

		mLookPosition = targetTr->GetPos() + targetCollider->GetOffset();

		if (mLookPosition.mX < windowWidth / 2 - tileSizeX) {
			mLookPosition.mX = windowWidth / 2 - tileSizeX + 1;
		}

		if (mLookPosition.mX > mapWidth - windowWidth / 2 + tileSizeX) {
			mLookPosition.mX = mapWidth - windowWidth / 2 + tileSizeX - 1;
		}

		if (mLookPosition.mY < windowHeight / 2 - tileSizeY * 2) {
			mLookPosition.mY = windowHeight / 2 - tileSizeY * 2 + 1;
		}

		if (mLookPosition.mY > mapHeight - windowHeight / 2 + tileSizeY) {
			mLookPosition.mY = mapHeight - windowHeight / 2 + tileSizeY - 1;
		}
	}
	else {
		CTransform* cameraTr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
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
