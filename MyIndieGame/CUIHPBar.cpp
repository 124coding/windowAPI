#include "CUIHPBar.h"

#include "CUIText.h"
#include "CTexture.h"
#include "CResourceMgr.h"

#include "CPlayerScript.h"

void CUIHPBar::OnCreate()
{
    mHPFill = CResourceMgr::Find<CTexture>(L"UILifeFill");
    mHPFill->CreateHBitmapFromGdiPlus(false);
    mHPFill->SetWidth(this->GetWidth());
    mHPFill->SetHeight(this->GetHeight());

    mHpInfo = new CUIText(SVector2D(), this->GetWidth(), this->GetHeight());
    mHpInfo->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

    this->AddChild(mHpInfo);



    CUIHUD::OnCreate();
}

void CUIHPBar::Active()
{
    CUIHUD::Active();
}

void CUIHPBar::InActive()
{
    CUIHUD::InActive();
}

void CUIHPBar::OnDestroy()
{
    CUIHUD::OnDestroy();
}

void CUIHPBar::OnUpdate(float tDeltaTime)
{
    mHpInfo->SetText(std::to_wstring(mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetHP()) + L" / " + std::to_wstring(mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetMaxHP()));
    CUIHUD::OnUpdate(tDeltaTime);
}

void CUIHPBar::OnLateUpdate(float tDeltaTime)
{
    CUIHUD::OnLateUpdate(tDeltaTime);
}

void CUIHPBar::Render(HDC tHDC)
{
    float ratio = (float)mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetHP() / mPlayer->GetComponent<CPlayerScript>(eComponentType::Script)->GetMaxHP();

    TransparentBlt(tHDC,
        mFinalPos.mX, mFinalPos.mY,
        mHPFill->GetWidth() * ratio, mHPFill->GetHeight(),
        mHPFill->GetDCMem(),
        0, 0,
        mHPFill->GetWidth() * ratio, mHPFill->GetHeight(),
        RGB(255, 0, 255));

    CUIHUD::Render(tHDC);
}

void CUIHPBar::UIClear()
{
    CUIHUD::UIClear();
}
