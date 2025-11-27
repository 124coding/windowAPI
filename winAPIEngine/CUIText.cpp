#include "CUIText.h"

void CUIText::OnCreate()
{
	CUIBase::OnCreate();
}

void CUIText::Active()
{
	CUIBase::Active();
}

void CUIText::InActive()
{
	CUIBase::InActive();
}

void CUIText::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CUIText::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CUIText::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CUIText::Render(HDC tHDC)
{
	if (mText == L"") return;

	Gdiplus::Graphics graphics(tHDC);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::FontFamily fontFamily(mFont.c_str());
	int style = mbBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;

	Gdiplus::StringFormat format;
	format.SetAlignment(mAlignH);
	format.SetLineAlignment(mAlignV);

    // 일반 그리기
    if (mStrokeWidth <= 0.0f && mOutlineWidth <= 0.0f)
    {
        Gdiplus::Font font(&fontFamily, mFontSize, style, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(mFontColor);
        Gdiplus::RectF layoutRect((float)mFinalPos.mX, (float)mFinalPos.mY, (float)mWidth, (float)mHeight);
        graphics.DrawString(mText.c_str(), -1, &font, layoutRect, &format, &brush);
    }
    else
    {
        Gdiplus::GraphicsPath path(Gdiplus::FillModeWinding);
        Gdiplus::RectF layoutRect((float)mFinalPos.mX, (float)mFinalPos.mY, (float)mWidth, (float)mHeight);

        path.AddString(
            mText.c_str(), -1, &fontFamily, style,
            mFontSize, layoutRect, &format
        );

        // 윤곽선 (가장 뒤)
        if (mOutlineWidth > 0.0f)
        {
            float totalWidth = mStrokeWidth + (mOutlineWidth * 2.0f);

            Gdiplus::Pen outlinePen(mOutlineColor, totalWidth);
            outlinePen.SetLineJoin(Gdiplus::LineJoinRound);

            graphics.DrawPath(&outlinePen, &path);
        }

        // 글자 두께 (중간)
        if (mStrokeWidth > 0.0f)
        {
            Gdiplus::Pen bodyPen(mFontColor, mStrokeWidth);
            bodyPen.SetLineJoin(Gdiplus::LineJoinRound);

            graphics.DrawPath(&bodyPen, &path);
        }

        // 글자 채우기 (가장 앞) - 빈틈 메우기
        Gdiplus::SolidBrush brush(mFontColor);
        graphics.FillPath(&brush, &path);
    }

	CUIBase::Render(tHDC);
}

void CUIText::UIClear()
{
	CUIBase::UIClear();
}
