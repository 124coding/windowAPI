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
    // 비활성화 상태거나 텍스처/크기 정보가 없으면 그리지 않음
    if (!this->GetEnabled()) return;

	if (mTex == nullptr) return;
	if (mWidth <= 0 || mHeight <= 0) return;

	Gdiplus::Graphics graphics(tHDC);

    // 이미지 확대/축소 시 품질 설정 (HighQualityBicubic: 부드럽게 처리)
    // 도트 게임(픽셀 아트)이라면 NearestNeighbor를 사용하는 것이 좋음
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
        // [기본] Stretch: 비율 무시하고 UI 영역(mWidth, mHeight)에 꽉 차게 늘림
        drawRect = Gdiplus::RectF((float)mFinalPos.mX, (float)mFinalPos.mY, (float)mWidth, (float)mHeight);
        break;

    case eImageMode::OriginalCenter:
        // [아이콘] Center: 리사이징 없이 원본 크기 그대로 출력하되, 위치만 UI 정중앙으로 보정
        // (UI 영역 - 이미지 크기) / 2 만큼 이동
        x = mFinalPos.mX + (mWidth - imgW) / 2.0f;
        y = mFinalPos.mY + (mHeight - imgH) / 2.0f;
        drawRect = Gdiplus::RectF(x, y, imgW, imgH);
        break;

    case eImageMode::KeepAspect:
        // [초상화/썸네일] Fit: 원본 종횡비(Aspect Ratio)를 유지하면서 UI 영역 안에 쏙 들어오게 조정

        // 가로/세로 중 더 많이 축소(혹은 덜 확대)해야 하는 쪽의 비율을 선택
        float scaleX = mWidth / imgW;
        float scaleY = mHeight / imgH;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        float finalW = imgW * scale;
        float finalH = imgH * scale;

        // 크기가 줄어들어 생긴 여백만큼 중앙 정렬
        x = mFinalPos.mX + (mWidth - finalW) / 2.0f;
        y = mFinalPos.mY + (mHeight - finalH) / 2.0f;

        drawRect = Gdiplus::RectF(x, y, finalW, finalH);
        break;
    }

    // 계산된 영역(drawRect)에 이미지 그리기
    graphics.DrawImage(mTex->GetImage(), drawRect);

    // 자식 UI 렌더링 (CUIBase::Render 호출)
    CUIBase::Render(tHDC);
}

void CUIImg::UIClear()
{
	CUIBase::UIClear();
}
