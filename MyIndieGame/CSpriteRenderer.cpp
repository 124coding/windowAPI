#include "CSpriteRenderer.h"
#include "GameObject.h"

#include "CTransform.h"

#include "CRenderer.h"

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
		return;
	}
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();
	float rot = tr->GetRot();
	SVector2D scale = tr->GetScale();

	pos = mainCamera->CaluatePosition(pos);

	if (mTexture->GetTextureType() == CTexture::eTextureType::Bmp) {
		TransparentBlt(hDC, pos.mX, pos.mY, 
			mTexture->GetWidth() * mSize.mX * scale.mX, mTexture->GetHeight() * mSize.mY * scale.mY, 
			mTexture->GetDCMem(), 
			0, 0, 
			mTexture->GetWidth(), mTexture->GetHeight(), 
			RGB(255, 0, 255));
	}
	else if (mTexture->GetTextureType() == CTexture::eTextureType::Png) {
		Gdiplus::ImageAttributes imgAtt = {};

		// 투명화 시킬 픽셀의 색 범위
		imgAtt.SetColorKey(Gdiplus::Color(100, 100, 100), Gdiplus::Color(255, 255, 255));

		Gdiplus::Graphics graphics(hDC);

		graphics.TranslateTransform(pos.mX, pos.mY);
		graphics.RotateTransform(rot);
		graphics.TranslateTransform(-pos.mX, -pos.mY);

		graphics.DrawImage(mTexture->GetImage(), Gdiplus::Rect(
			pos.mX, pos.mY, 
			mTexture->GetWidth() * mSize.mX * scale.mX, mTexture->GetHeight() * mSize.mY * scale.mY),
			0, 0,
			mTexture->GetWidth(), mTexture->GetHeight(),
			Gdiplus::UnitPixel,
			nullptr
		);
	}
	// Gdiplus::Graphics graphics(hDC);
	// graphics.DrawImage(mImage, Gdiplus::Rect(pos.mX, pos.mY, mWidth, mHeight));
	// Rectangle(hDC, pos.mX, pos.mY, pos.mX + 100, pos.mY + 100);
}