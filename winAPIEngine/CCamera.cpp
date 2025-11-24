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
		CTransform* targetTr = mTarget->GetComponent<CTransform>();
		CCollider* targetCollider = mTarget->GetComponent<CCollider>();

		mLookPosition = targetTr->GetPos() + targetCollider->GetOffset();

		if (mLookPosition.mX < windowWidth / 2 - CTilemapRenderer::TileSize.mX) {
			mLookPosition.mX = windowWidth / 2 - CTilemapRenderer::TileSize.mX + 1;
		}

		if (mLookPosition.mX > mapWidth - windowWidth / 2 + CTilemapRenderer::TileSize.mX) {
			mLookPosition.mX = mapWidth - windowWidth / 2 + CTilemapRenderer::TileSize.mX - 1;
		}

		if (mLookPosition.mY < windowHeight / 2 - CTilemapRenderer::TileSize.mY * 2) {
			mLookPosition.mY = windowHeight / 2 - CTilemapRenderer::TileSize.mY * 2 + 1;
		}

		if (mLookPosition.mY > mapHeight - windowHeight / 2 + CTilemapRenderer::TileSize.mY) {
			mLookPosition.mY = mapHeight - windowHeight / 2 + CTilemapRenderer::TileSize.mY - 1;
		}
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
