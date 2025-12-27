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

	if (GetOwner()->GetLayerType() == eLayerType::BackGround) {
		BitBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
			mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
			mTexture->GetDCMem(),
			0, 0,
			SRCCOPY);

		return;
	}

	// 화면 밖의 오브젝트들을 컬링하기 위함

	float left = pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX;
	float top = pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY;
	float right = left + mTexture->GetWidth() * fScaleX;
	float bottom = top + mTexture->GetHeight() * fScaleY;

	float margin = 0.0f;
	if (rot != 0.0f) margin = (mTexture->GetWidth() * fScaleX + mTexture->GetHeight() * fScaleY) * 0.5f;

	if (right + margin < 0 ||
		left - margin > windowWidth ||
		bottom + margin < 0||
		top - margin > windowHeight)
	{
		return;
	}

	HBITMAP hBm = mTexture->GetHBitmap(mbFlipX);

	if (hBm != nullptr && !mbGdiplusDraw) {
		HDC srcDC = mTexture->GetDCMem();
		HBITMAP oldBm = (HBITMAP)SelectObject(srcDC, hBm);

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
		if (mbFlipX)
		{
			fScaleX *= -1.0f;
		}
		graphics.ScaleTransform(fScaleX, fScaleY);

		float originalWidth = mTexture->GetWidth();
		float originalHeight = mTexture->GetHeight();

		// 행렬에 알파 값에 직접 곱해줌
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
