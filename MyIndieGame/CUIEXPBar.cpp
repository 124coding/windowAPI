#include "CUIEXPBar.h"

#include "CResourceMgr.h"

#include "CUIText.h"
#include "CPlayerScript.h"

void CUIEXPBar::OnCreate()
{
    mExpFill = CResourceMgr::Find<CTexture>(L"UIEXPFill");
    mExpFill->CreateHBitmapFromGdiPlus(false);
    mExpFill->SetWidth(this->GetWidth());
    mExpFill->SetHeight(this->GetHeight());

    mLevelInfo = new CUIText(SVector2D(), this->GetWidth() - 20.0f, this->GetHeight());
    mLevelInfo->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentCenter);

    this->AddChild(mLevelInfo);



    CUIHUD::OnCreate();
}

void CUIEXPBar::Active()
{
    CUIHUD::Active();
}

void CUIEXPBar::InActive()
{
    CUIHUD::InActive();
}

void CUIEXPBar::OnDestroy()
{
    CUIHUD::OnDestroy();
}

void CUIEXPBar::OnUpdate(float tDeltaTime)
{
    mLevelInfo->SetText(L"Lv." + std::to_wstring(mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetLevel()));
    CUIHUD::OnUpdate(tDeltaTime);
}

void CUIEXPBar::OnLateUpdate(float tDeltaTime)
{

    CUIHUD::OnLateUpdate(tDeltaTime);
}

void CUIEXPBar::Render(HDC tHDC)
{
    float ratio = (float)mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetExp() / mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetNeedLevelUpExp();

    TransparentBlt(tHDC,
        mFinalPos.mX, mFinalPos.mY,
        mExpFill->GetWidth() * ratio, mExpFill->GetHeight(),
        mExpFill->GetDCMem(),
        0, 0,
        mExpFill->GetWidth() * ratio, mExpFill->GetHeight(),
        RGB(255, 0, 255));

    CUIHUD::Render(tHDC);
}

void CUIEXPBar::UIClear()
{
    CUIHUD::UIClear();
}
