#include "CUIImg.h"

void CUIImg::OnCreate()
{
	CUIBase::OnCreate();
}

void CUIImg::Active()
{
	CUIBase::Active();
}

void CUIImg::InActive()
{
	CUIBase::InActive();
}

void CUIImg::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CUIImg::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CUIImg::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CUIImg::Render(HDC tHDC)
{
	if (mTex == nullptr) return;
	if (mWidth <= 0 || mHeight <= 0) return;

	Gdiplus::Graphics graphics(tHDC);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

	// 원본 크기
	float imgW = mTex->GetWidth();
	float imgH = mTex->GetHeight();

	Gdiplus::RectF drawRect;

    float x, y;

    switch (mImgMode)
    {
    case eImageMode::Stretch:
        // [기본] UI 크기에 딱 맞춰서 늘림
        drawRect = Gdiplus::RectF((float)mFinalPos.mX, (float)mFinalPos.mY, (float)mWidth, (float)mHeight);
        break;

    case eImageMode::OriginalCenter:
        // [아이콘] 원본 크기 그대로 + 정중앙 배치
        x = mFinalPos.mX + (mWidth - imgW) / 2.0f;
        y = mFinalPos.mY + (mHeight - imgH) / 2.0f;
        drawRect = Gdiplus::RectF(x, y, imgW, imgH);
        break;

    case eImageMode::KeepAspect:
        // [초상화] 비율을 유지하면서 박스 안에 꽉 차게
        float scaleX = mWidth / imgW;
        float scaleY = mHeight / imgH;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        float finalW = imgW * scale;
        float finalH = imgH * scale;

        x = mFinalPos.mX + (mWidth - finalW) / 2.0f;
        y = mFinalPos.mY + (mHeight - finalH) / 2.0f;

        drawRect = Gdiplus::RectF(x, y, finalW, finalH);
        break;
    }

    graphics.DrawImage(mTex->GetImage(), drawRect);

	CUIBase::Render(tHDC);
}

void CUIImg::UIClear()
{
	CUIBase::UIClear();
}
