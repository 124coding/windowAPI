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

// [Logic Update] 물리 연산 및 애니메이션 상태 진행
void CEffectText::OnUpdate(float tDeltaTime) {
    CEffect::OnUpdate(tDeltaTime);

    CTransform* tr = this->GetComponent<CTransform>(eComponentType::Transform);

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

        // 1. 처음 20% 구간: 크기가 1.0 -> 1.2로 커짐 (Pop!)
        if (progress < 0.2f)
            tr->SetScale(SVector2D(1.0f + progress, 1.0f + progress));
        // 2. 나머지 구간: 크기가 1.2 -> 0.0으로 작아짐 (Fade Out 느낌)
        else
            tr->SetScale(SVector2D(1.2f - ((progress - 0.2f) / 0.8f * 1.2f), 1.2f - ((progress - 0.2f) / 0.8f * 1.2f)));

        // 애니메이션 종료 시 객체 비활성화 (반환)
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

    CTransform* tr = this->GetComponent<CTransform>(eComponentType::Transform);

    Gdiplus::Graphics graphics(tHDC);

    Gdiplus::FontFamily fontFamily(L"Noto Sans KR Medium");
    Gdiplus::Font font(&fontFamily, 30, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush brush(mColor);

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    // 텍스트의 위치 이동 및 크기 변화(Scale Animation) 적용
    graphics.TranslateTransform(tr->GetPos().mX, tr ->GetPos().mY);
    graphics.ScaleTransform(tr->GetScale().mX, tr->GetScale().mY);

    // 텍스트 그리기 (좌표는 0,0이지만 위에서 TranslateTransform을 했으므로 해당 위치에 그려짐)
    graphics.DrawString(mText.c_str(), mText.length(), &font, Gdiplus::PointF(0, 0), &format, &brush);

    graphics.ResetTransform();
}

// [Object Pooling] 초기화 (기본형)
void CEffectText::Reset(SVector2D tPos)
{
    CTransform* tr = this->GetComponent<CTransform>(eComponentType::Transform);
    // 위로 솟구치는 초기 속도 부여 (Jump Force)
    tr->SetVelocity(SVector2D(0.0f, -200.0f));

    // 월드 좌표(tPos)를 화면 좌표로 변환하여 저장
    tr->SetPos(mainCamera->CalculatePosition(tPos));
    mStartPos = tPos;

    tr->SetScale(SVector2D(1.0f, 1.0f));
    mCurTime = 0.0f;
    SetState(true);
}

// [Object Pooling] 초기화 (텍스트, 색상 지정형)
void CEffectText::Reset(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor)
{
    CTransform* tr = this->GetComponent<CTransform>(eComponentType::Transform);
    tr->SetVelocity(SVector2D(0.0f, -200.0f));

    tr->SetPos(mainCamera->CalculatePosition(tPos));
    mStartPos = tPos;

    tr->SetScale(SVector2D(1.0f, 1.0f));

    mColor = tColor;

    mText = tText;
    mCurTime = 0.0f;
    SetState(true);
}

