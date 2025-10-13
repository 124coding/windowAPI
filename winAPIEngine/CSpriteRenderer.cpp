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

void CSpriteRenderer::Render(HDC tHDC)
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

		if (mTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;

			func.SourceConstantAlpha = 255;
			AlphaBlend(tHDC, pos.mX - ((int)(mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX) / 2) + GetOwner()->GetAnchorPoint().mX, pos.mY - ((int)(mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY)) + GetOwner()->GetAnchorPoint().mY,
				mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX, mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetDCMem(),
				0, 0,
				mTexture->GetWidth(), mTexture->GetHeight(), func);
		}
		else {
			TransparentBlt(tHDC, pos.mX - ((int)(mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX) / 2) + GetOwner()->GetAnchorPoint().mX, pos.mY - ((int)(mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY)) + GetOwner()->GetAnchorPoint().mY,
				mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX, mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetDCMem(),
				0, 0,
				mTexture->GetWidth(), mTexture->GetHeight(),
				RGB(255, 0, 255));
		}
	}
	else if (mTexture->GetTextureType() == CTexture::eTextureType::Png) {
		Gdiplus::ImageAttributes imgAtt = {};

		// 투명화 시킬 픽셀의 색 범위
		imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

		Gdiplus::Graphics graphics(tHDC);

		graphics.TranslateTransform(pos.mX, pos.mY);
		graphics.RotateTransform(rot);
		graphics.TranslateTransform(-pos.mX, -pos.mY);

		int a = pos.mX - ((int)(mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX) / 2) + GetOwner()->GetAnchorPoint().mX;
		int b = pos.mY - ((int)(mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY)) + GetOwner()->GetAnchorPoint().mY;
		graphics.DrawImage(mTexture->GetImage(), Gdiplus::Rect(
			pos.mX - ((int)(mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX) / 2) + GetOwner()->GetAnchorPoint().mX, pos.mY - ((int)(mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY)) + GetOwner()->GetAnchorPoint().mY,
			mTexture->GetWidth() * GetOwner()->GetSize().mX * scale.mX, mTexture->GetHeight() * GetOwner()->GetSize().mY * scale.mY),
			0, 0,
			mTexture->GetWidth(), mTexture->GetHeight(),
			Gdiplus::UnitPixel,
			nullptr
		);
	}
	// Gdiplus::Graphics graphics(tHDC);
	// graphics.DrawImage(mImage, Gdiplus::Rect(pos.mX, pos.mY, mWidth, mHeight));
	// Rectangle(tHDC, pos.mX, pos.mY, pos.mX + 100, pos.mY + 100);
}