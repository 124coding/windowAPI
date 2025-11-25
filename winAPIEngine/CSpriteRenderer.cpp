#include "CSpriteRenderer.h"
#include "GameObject.h"

#include "CSceneMgr.h"

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

	float fScaleX = GetOwner()->GetSize().mX * scale.mX;
	float fScaleY = GetOwner()->GetSize().mY * scale.mY;

	/*if (mbFlipY) {
		fScaleY *= -1.0f;
	}*/

	pos = mainCamera->CalculatePosition(pos);

	HBITMAP hBm = mTexture->GetHBitmap(mbFlipX);

	if (hBm != nullptr) {
		HDC srcDC = mTexture->GetDCMem();
		HBITMAP oldBm = (HBITMAP)SelectObject(srcDC, hBm);

		if (GetOwner()->GetLayerType() == eLayerType::BackGround) {
			BitBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				srcDC,
				0, 0,
				SRCCOPY);
			SelectObject(srcDC, oldBm);

			return;
		}

		if (mTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;

			func.SourceConstantAlpha = (BYTE)(mAlphaMultiplier * 255.0f);
			AlphaBlend(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				srcDC,
				0, 0,
				mTexture->GetBaseWidth(), mTexture->GetBaseHeight(), func);
		}
		else {
			TransparentBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				srcDC,
				0, 0,
				mTexture->GetBaseWidth(), mTexture->GetBaseHeight(),
				RGB(255, 0, 255));
		}

		SelectObject(srcDC, oldBm);
	}
	else {
		Gdiplus::ImageAttributes imgAtt = {};

		// 투명화 시킬 픽셀의 색 범위
		imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

		Gdiplus::Graphics graphics(tHDC);

		graphics.TranslateTransform(pos.mX, pos.mY);
		graphics.RotateTransform(rot);
		graphics.ScaleTransform(fScaleX, fScaleY);

		float originalWidth = mTexture->GetWidth();
		float originalHeight = mTexture->GetHeight();

		if (mAlphaMultiplier < 1.0f) {
			Gdiplus::ColorMatrix colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			};

			colorMatrix.m[3][3] = mAlphaMultiplier;

			imgAtt.SetColorMatrix(&colorMatrix);

			graphics.DrawImage(mTexture->GetImage(),
				Gdiplus::Rect(-GetOwner()->GetAnchorPoint().mX, -GetOwner()->GetAnchorPoint().mY,
					originalWidth, originalHeight),
				0, 0,
				originalWidth, originalHeight,
				Gdiplus::UnitPixel,
				&imgAtt
			);
		}
		else {
			graphics.DrawImage(mTexture->GetImage(),
				-GetOwner()->GetAnchorPoint().mX, 
				-GetOwner()->GetAnchorPoint().mY,
				originalWidth, 
				originalHeight
			);
		}
	}
}
