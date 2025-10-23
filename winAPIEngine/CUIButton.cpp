#include "CUIButton.h"

#include "CTexture.h"

#include "CInputMgr.h"

#include <string>

void CUIButton::OnCreate()
{
	mOnClick = std::bind(&CUIButton::ButtonClick, this);
}

void CUIButton::Active()
{
}

void CUIButton::InActive()
{
}

void CUIButton::OnDestroy()
{
}

void CUIButton::OnUpdate(float tDeltaTime)
{
	SVector2D mousePos = CInputMgr::GetMousePosition();

	if (mPos.mX <= mousePos.mX && mousePos.mX <= mPos.mX + mSize.mX &&
		mPos.mY <= mousePos.mY && mousePos.mY <= mPos.mY + mSize.mY) {

		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			mOnClick();
		};
	}
}

void CUIButton::OnLateUpdate(float tDeltaTime)
{
}

void CUIButton::Render(HDC tHDC)
{
	SVector2D mousePos = CInputMgr::GetMousePosition();

	if (mPos.mX <= mousePos.mX && mousePos.mX <= mPos.mX + mSize.mX &&
		mPos.mY <= mousePos.mY && mousePos.mY <= mPos.mY + mSize.mY) {

		if (mMouseInTexture->GetTextureType() == CTexture::eTextureType::Bmp) {

			if (mMouseInTexture->GetbAlpha()) {
				BLENDFUNCTION func = {};
				func.BlendOp = AC_SRC_OVER;
				func.BlendFlags = 0;
				func.AlphaFormat = AC_SRC_ALPHA;

				func.SourceConstantAlpha = 255;
				AlphaBlend(tHDC
					, mPos.mX, mPos.mY
					, mPos.mX + mSize.mX
					, mPos.mY + mSize.mY
					, mMouseInTexture->GetDCMem()
					, 0, 0
					, mMouseOutTexture->GetWidth()
					, mMouseOutTexture->GetHeight(), func);
			}
			else {
				TransparentBlt(tHDC
					, mPos.mX, mPos.mY
					, mPos.mX + mSize.mX
					, mPos.mY + mSize.mY
					, mMouseInTexture->GetDCMem()
					, 0, 0
					, mMouseOutTexture->GetWidth()
					, mMouseOutTexture->GetHeight()
					, RGB(255, 0, 255));
			}
		}
		else if (mMouseInTexture->GetTextureType() == CTexture::eTextureType::Png) {
			Gdiplus::ImageAttributes imgAtt = {};

			// 투명화 시킬 픽셀의 색 범위
			imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

			Gdiplus::Graphics graphics(tHDC);

			graphics.TranslateTransform(mPos.mX, mPos.mY);
			graphics.RotateTransform(0.0f);
			graphics.TranslateTransform(-mPos.mX, -mPos.mY);

			graphics.DrawImage(mMouseInTexture->GetImage(), Gdiplus::Rect(
				mPos.mX, mPos.mY,
				mSize.mX, mSize.mY),
				0, 0,
				mMouseOutTexture->GetWidth(),
				mMouseOutTexture->GetHeight(),
				Gdiplus::UnitPixel,
				nullptr
			);
		}
	}
	else {
		if (mMouseOutTexture->GetTextureType() == CTexture::eTextureType::Bmp) {

			if (mMouseOutTexture->GetbAlpha()) {
				BLENDFUNCTION func = {};
				func.BlendOp = AC_SRC_OVER;
				func.BlendFlags = 0;
				func.AlphaFormat = AC_SRC_ALPHA;

				func.SourceConstantAlpha = 255;
				AlphaBlend(tHDC
					, mPos.mX, mPos.mY
					, mPos.mX + mSize.mX
					, mPos.mY + mSize.mY
					, mMouseOutTexture->GetDCMem()
					, 0, 0
					, mMouseOutTexture->GetWidth()
					, mMouseOutTexture->GetHeight(), func);
			}
			else {
				TransparentBlt(tHDC
					, mPos.mX, mPos.mY
					, mPos.mX + mSize.mX
					, mPos.mY + mSize.mY
					, mMouseOutTexture->GetDCMem()
					, 0, 0
					, mMouseOutTexture->GetWidth()
					, mMouseOutTexture->GetHeight()
					, RGB(255, 0, 255));
			}
		}
		else if (mMouseOutTexture->GetTextureType() == CTexture::eTextureType::Png) {
			Gdiplus::ImageAttributes imgAtt = {};

			// 투명화 시킬 픽셀의 색 범위
			imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

			Gdiplus::Graphics graphics(tHDC);

			graphics.TranslateTransform(mPos.mX, mPos.mY);
			graphics.RotateTransform(0.0f);
			graphics.TranslateTransform(-mPos.mX, -mPos.mY);

			graphics.DrawImage(mMouseOutTexture->GetImage(), Gdiplus::Rect(
				mPos.mX, mPos.mY,
				mSize.mX, mSize.mY),
				0, 0,
				mMouseOutTexture->GetWidth(),
				mMouseOutTexture->GetHeight(),
				Gdiplus::UnitPixel,
				nullptr
			);
		}
	}
}

void CUIButton::UIClear()
{
}

void CUIButton::ButtonClick()
{
}
