#include "CAutoSpriteFlipper.h"

#include "GameObject.h"

#include "CTransform.h"
#include "CSpriteRenderer.h"

void CAutoSpriteFlipper::OnCreate()
{
}

void CAutoSpriteFlipper::OnDestroy()
{
}

void CAutoSpriteFlipper::OnUpdate(float tDeltaTime)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();

	if (tr->GetVelocity().mX < 0 && !mFlipImage) {
		sr->GetTexture()->GetImage()->RotateFlip(Gdiplus::RotateNoneFlipX);
		mFlipImage = true;
	}
	else if (tr->GetVelocity().mX > 0 && mFlipImage) {
		sr->GetTexture()->GetImage()->RotateFlip(Gdiplus::RotateNoneFlipX);
		mFlipImage = false;
	}
}

void CAutoSpriteFlipper::OnLateUpdate(float tDeltaTime)
{
}

void CAutoSpriteFlipper::Render(HDC tHDC)
{
}
