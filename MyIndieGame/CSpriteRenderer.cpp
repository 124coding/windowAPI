#include "CSpriteRenderer.h"
#include "GameObject.h"
#include "CTransform.h"

#include "winMacro.h"

#include <assert.h>

void CSpriteRenderer::OnCreate()
{
}

void CSpriteRenderer::OnDestroy()
{

}

void CSpriteRenderer::OnUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::OnLateUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::Render(HDC hDC)
{
	if (mTexture == nullptr) {
		assert(false);
	}
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();

	if (mTexture->GetTextureType() == CTexture::eTextureType::Bmp) {
		TransparentBlt(hDC, pos.mX, pos.mY, 
			mTexture->GetWidth(), mTexture->GetHeight(), 
			mTexture->GetDCMem(), 
			0, 0, 
			mTexture->GetWidth(), mTexture->GetHeight(), 
			RGB(255, 0, 255));
	}
	else if (mTexture->GetTextureType() == CTexture::eTextureType::Png) {
		Gdiplus::Graphics graphics(hDC);
		graphics.DrawImage(mTexture->GetImage(), Gdiplus::Rect(pos.mX, pos.mY, mTexture->GetWidth() * mSize.mX, mTexture->GetHeight() * mSize.mY));
	}
	// Gdiplus::Graphics graphics(hDC);
	// graphics.DrawImage(mImage, Gdiplus::Rect(pos.mX, pos.mY, mWidth, mHeight));
	// Rectangle(hDC, pos.mX, pos.mY, pos.mX + 100, pos.mY + 100);
}