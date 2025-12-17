#include "CEffectText.h"

#include "CRenderer.h"
#include "CTransform.h"

CEffectText::CEffectText(const CEffectText& tEffectText) {

}
CEffectText& CEffectText::operator=(const CEffectText& tDamagetTExt) {
    if (this == &tDamagetTExt)
    {
        return *this;
    }

    GameObject::operator=(tDamagetTExt);
}

void CEffectText::OnCreate() {
    CEffect::OnCreate();

    mCurTime = 0.0f;
    mLifeTime = 3.0f;
    SetState(false);
}

void CEffectText::OnDestroy() {
    CEffect::OnDestroy();
}

// 여기 문제인듯??
void CEffectText::OnUpdate(float tDeltaTime) {
    CEffect::OnUpdate(tDeltaTime);

    CTransform* tr = this->GetComponent<CTransform>();

    if (tr->GetPos().mY > mainCamera->CalculatePosition(mStartPos).mY) {
        tr->SetPos(mainCamera->CalculatePosition(mStartPos));
        tr->SetVelocity(SVector2D());
    }
    else {
        tr->SetVelocity(SVector2D(tr->GetVelocity().mX, tr->GetVelocity().mY + mGravity * tDeltaTime));
    }

    if (mCurTime > mWaitTime) {
        float fadeDuration = 0.5f;
        float progress = (mCurTime - mWaitTime) / fadeDuration;

        if (progress < 0.2f)
            tr->SetScale(SVector2D(1.0f + progress, 1.0f + progress));
        else
            tr->SetScale(SVector2D(1.2f - ((progress - 0.2f) / 0.8f * 1.2f), 1.2f - ((progress - 0.2f) / 0.8f * 1.2f)));

        if (progress >= 1.0f) {
            SetState(false);
        }
    }

}

void CEffectText::OnLateUpdate(float tDeltaTime) {
    CEffect::OnLateUpdate(tDeltaTime);
}

void CEffectText::Render(HDC tHDC) {
    CEffect::Render(tHDC);

    CTransform* tr = this->GetComponent<CTransform>();
    tr->SetPos(mainCamera->CalculatePosition(mStartPos));

    Gdiplus::Graphics graphics(tHDC);

    Gdiplus::FontFamily fontFamily(L"Noto Sans KR Medium");
    Gdiplus::Font font(&fontFamily, 30, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush brush(mColor);

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    graphics.TranslateTransform(tr->GetPos().mX, tr ->GetPos().mY);
    graphics.ScaleTransform(tr->GetScale().mX, tr->GetScale().mY);

    graphics.DrawString(mText.c_str(), mText.length(), &font, Gdiplus::PointF(0, 0), &format, &brush);

    graphics.ResetTransform();
}

void CEffectText::Reset(SVector2D tPos)
{
    CTransform* tr = this->GetComponent<CTransform>();
    tr->SetVelocity(SVector2D(0.0f, -200.0f));
    tr->SetPos(mainCamera->CalculatePosition(tPos));
    mStartPos = tPos;

    tr->SetScale(SVector2D(1.0f, 1.0f));
    mCurTime = 0.0f;
    SetState(true);
}

void CEffectText::Reset(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor)
{
    CTransform* tr = this->GetComponent<CTransform>();
    tr->SetVelocity(SVector2D(0.0f, -200.0f));
    tr->SetPos(mainCamera->CalculatePosition(tPos));
    mStartPos = tPos;

    tr->SetScale(SVector2D(1.0f, 1.0f));

    mColor = tColor;

    mText = tText;
    mCurTime = 0.0f;
    SetState(true);
}

