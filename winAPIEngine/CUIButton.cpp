#include "CUIButton.h"

#include "CTexture.h"

#include "CInputMgr.h"

#include <string>

void CUIButton::OnCreate()
{
	CUIBase::OnCreate();
}

void CUIButton::Active()
{
	CUIBase::Active();
}

void CUIButton::InActive()
{
	CUIBase::InActive();
}

void CUIButton::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CUIButton::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);

	SVector2D mousePos = CInputMgr::GetMousePosition();

	if (mFinalPos.mX <= mousePos.mX && mousePos.mX <= mFinalPos.mX + mWidth &&
		mFinalPos.mY <= mousePos.mY && mousePos.mY <= mFinalPos.mY + mHeight) {

		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			mOnClick();
		};
	}
}

void CUIButton::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CUIButton::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);

	Gdiplus::Graphics graphics(tHDC);

	graphics.TranslateTransform(mFinalPos.mX, mFinalPos.mY);

	SVector2D mousePos = CInputMgr::GetMousePosition();

	if (mFinalPos.mX <= mousePos.mX && mousePos.mX <= mFinalPos.mX + mWidth &&
		mFinalPos.mY <= mousePos.mY && mousePos.mY <= mFinalPos.mY + mHeight) {

		graphics.DrawImage(mMouseInTexture->GetImage(),
			0.0f,
			0.0f,
			mWidth,
			mHeight
		);
	}
	else {
		/*TransparentBlt(tHDC
			, mFinalPos.mX, mFinalPos.mY
			, mFinalPos.mX + mWidth
			, mFinalPos.mY + mHeight
			, mMouseOutTexture->GetDCMem()
			, 0, 0
			, mWidth, mHeight
			, RGB(255, 0, 255));*/

		graphics.DrawImage(mMouseOutTexture->GetImage(),
			0.0f,
			0.0f,
			mWidth,
			mHeight
		);
	}
}

void CUIButton::UIClear()
{
	CUIBase::UIClear();
}

CUIButton* CUIButton::CreateInvertButton(
	const std::wstring& tName
	, const std::wstring& tText
	, const std::wstring& tImagePath
	, float tX, float tY
	, float tPaddingX, float tPaddingY
	, Gdiplus::Color tNormalBgColor, Gdiplus::Color tNormalFgColor
	, Gdiplus::Color tHoverBgColor, Gdiplus::Color tHoverFgColor
	, float tFontSize
	, float tBold) {

	bool isImageMode = !tImagePath.empty();

	UINT width = 0;
	UINT height = 0;

	if (isImageMode) {
		// 이미지를 임시로 로드해서 크기를 알아냅니다.
		Gdiplus::Image tempImg(tImagePath.c_str());
		if (tempImg.GetLastStatus() != Gdiplus::Ok) {
			return nullptr;
		}
		else {
			width = tempImg.GetWidth() + (UINT)tPaddingX;
			height = tempImg.GetHeight() + (UINT)tPaddingY;
		}
	}
	else {
		Gdiplus::FontFamily fontFamily(L"Noto Sans KR Medium");
		Gdiplus::Font font(&fontFamily, tFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Gdiplus::Bitmap tempBmp(1, 1);
		Gdiplus::Graphics tempG(&tempBmp);
		tempG.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		Gdiplus::RectF boundRect;
		tempG.MeasureString(tText.c_str(), -1, &font, Gdiplus::PointF(0, 0), &boundRect);

		width = (UINT)(boundRect.Width + tPaddingX);
		height = (UINT)(boundRect.Height + tPaddingY);
	}

	if (width == 0 || height == 0) return nullptr;

	std::wstring keyOut = tName + L"_Normal";
	std::wstring keyIn = tName + L"_Hover";

	CTexture* texNormal = CTexture::Create(keyOut, width, height);
	CTexture* texHover = CTexture::Create(keyIn, width, height);

	auto DrawFunc = [&](CTexture* tTargetTex, Gdiplus::Color tBgColor, Gdiplus::Color tFgColor) {
		if (tTargetTex == nullptr) return;

		Gdiplus::Graphics g(tTargetTex->GetImage());
		g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		g.Clear(Gdiplus::Color(0, 0, 0, 0));

		Gdiplus::SolidBrush bgBrush(tBgColor);
		Gdiplus::GraphicsPath bgPath;
		Gdiplus::Rect rect(0, 0, width, height);
		CTexture::AddRoundedRectToPath(&bgPath, rect, height / 5);
		g.FillPath(&bgBrush, &bgPath);

		if (isImageMode) {

			Gdiplus::Image img(tImagePath.c_str());

			int imgX = (width - img.GetWidth()) / 2;
			int imgY = (height - img.GetHeight()) / 2;

			// 이미지 틴트(Tint) 처리: fgColor로 이미지 색상을 덮어씌움
			// (원본 색을 유지하려면 fgColor에 흰색을 넣기.)
			Gdiplus::ImageAttributes imgAttr;
			if (tFgColor.GetValue() != Gdiplus::Color::White) {
				// 색상 행렬을 이용해 이미지를 단색으로 만듭니다.
				float r = tFgColor.GetR() / 255.0f;
				float g = tFgColor.GetG() / 255.0f;
				float b = tFgColor.GetB() / 255.0f;
				Gdiplus::ColorMatrix colorMatrix = {
					r, 0, 0, 0, 0,
					0, g, 0, 0, 0,
					0, 0, b, 0, 0,
					0, 0, 0, 1, 0,
					0, 0, 0, 0, 1
				};
				imgAttr.SetColorMatrix(&colorMatrix);
			}

			g.DrawImage(&img, Gdiplus::Rect(imgX, imgY, img.GetWidth(), img.GetHeight()),
				0, 0, img.GetWidth(), img.GetHeight(), Gdiplus::UnitPixel, &imgAttr);

		}
		else {
			Gdiplus::SolidBrush textBrush(tFgColor);
			Gdiplus::Pen textPen(tFgColor, tBold);
			Gdiplus::FontFamily fontFamily(L"Noto Sans KR Medium");
			Gdiplus::Font font(&fontFamily, tFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

			Gdiplus::GraphicsPath textPath;
			Gdiplus::StringFormat format;
			format.SetAlignment(Gdiplus::StringAlignmentCenter);
			format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			Gdiplus::RectF textRect(0.0f, 0.0f, (float)width, (float)height);

			textPath.AddString(tText.c_str(), -1, &fontFamily, Gdiplus::FontStyleBold,
				tFontSize, textRect, &format);
			g.FillPath(&textBrush, &textPath);
			g.DrawPath(&textPen, &textPath);
		}
		};

	// 평상시 (Normal)
	DrawFunc(texNormal, tNormalBgColor, tNormalFgColor);
	// 마우스 오버시 (Hover)
	DrawFunc(texHover, tHoverBgColor, tHoverFgColor);

	CUIButton* btn = new CUIButton();
	btn->SetName(tName);
	// CUIButton 내부에서 평상시를 MouseOut, 호버를 MouseIn으로 쓴다고 가정
	btn->SetMouseOutTexture(keyOut);
	btn->SetMouseInTexture(keyIn);
	btn->SetWidth(width);
	btn->SetHeight(height);
	btn->SetPos(SVector2D(tX, tY));

	return btn;
}
