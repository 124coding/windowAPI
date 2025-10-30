#include "CUIHPBar.h"

#include "CTexture.h"
#include "CResourceMgr.h"

#include "CPlayerScript.h"

void CUIHPBar::OnCreate()
{
}

void CUIHPBar::Active()
{
}

void CUIHPBar::InActive()
{
}

void CUIHPBar::OnDestroy()
{
}

void CUIHPBar::OnUpdate(float tDeltaTime)
{
}

void CUIHPBar::OnLateUpdate(float tDeltaTime)
{
}

void CUIHPBar::Render(HDC tHDC)
{
    HPEN thOldPen = NULL;
    HBRUSH thOldBrush = NULL;

    HPEN thPen = NULL;
    HBRUSH thBrush = NULL;

    thPen = (HPEN)GetStockObject(NULL_PEN);
    thBrush = CreateSolidBrush(RGB(124, 124, 124));

    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);
    thOldPen = (HPEN)SelectObject(tHDC, thPen);

    Rectangle(tHDC, 20, 20, 320, 60);

    SelectObject(tHDC, thOldBrush);
    DeleteObject(thBrush);

    thBrush = CreateSolidBrush(RGB(255, 0, 0));
    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);

    int right = (int)(295 * ((float)mPlayer->GetComponent<CPlayerScript>()->GetHP() / mPlayer->GetComponent<CPlayerScript>()->GetMaxHP())) + 23;

    Rectangle(tHDC, 23, 20, right, 60);

    SelectObject(tHDC, thPen);
    DeleteObject(thPen);

    thPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    thOldPen = (HPEN)SelectObject(tHDC, thPen);

    SelectObject(tHDC, thOldBrush);
    DeleteObject(thBrush);

    thBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);

    Rectangle(tHDC, 20, 20, 320, 60);

    SelectObject(tHDC, thOldBrush);
    SelectObject(tHDC, thPen);

    DeleteObject(thPen);
    DeleteObject(thBrush);

    SetBkMode(tHDC, TRANSPARENT);

    TCHAR szHPText[32];
    _stprintf_s(szHPText, 32, TEXT("%d   /   %d"), mPlayer->GetComponent<CPlayerScript>()->GetHP(), mPlayer->GetComponent<CPlayerScript>()->GetMaxHP());
    TextOut(tHDC, 140, 30, szHPText, _tcslen(szHPText));
}

void CUIHPBar::UIClear()
{
}
