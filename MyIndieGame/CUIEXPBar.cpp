#include "CUIEXPBar.h"

#include "CPlayer.h"

void CUIEXPBar::OnCreate()
{
}

void CUIEXPBar::Active()
{
}

void CUIEXPBar::InActive()
{
}

void CUIEXPBar::OnDestroy()
{
}

void CUIEXPBar::OnUpdate(float tDeltaTime)
{
}

void CUIEXPBar::OnLateUpdate(float tDeltaTime)
{
}

void CUIEXPBar::Render(HDC tHDC)
{
    HPEN thOldPen = NULL;
    HBRUSH thOldBrush = NULL;

    HPEN thPen = NULL;
    HBRUSH thBrush = NULL;

    thPen = (HPEN)GetStockObject(NULL_PEN);
    thBrush = CreateSolidBrush(RGB(124, 124, 124));

    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);
    thOldPen = (HPEN)SelectObject(tHDC, thPen);

    Rectangle(tHDC, 20, 70, 320, 110);

    SelectObject(tHDC, thOldBrush);
    DeleteObject(thBrush);

    thBrush = CreateSolidBrush(RGB(0, 255, 0));
    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);

    Rectangle(tHDC, 23, 70, 23 + 295 /* * (mPlayer->GetHP() / mPlayer->GetMaxHP())*/, 110); // 여기 각 경험치에 맞게 바꿔야 함

    SelectObject(tHDC, thPen);
    DeleteObject(thPen);

    thPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    thOldPen = (HPEN)SelectObject(tHDC, thPen);

    SelectObject(tHDC, thOldBrush);
    DeleteObject(thBrush);

    thBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);

    Rectangle(tHDC, 20, 70, 320, 110);

    SelectObject(tHDC, thOldBrush);
    DeleteObject(thBrush);

    DeleteObject(thPen);
    DeleteObject(thBrush);

    SetBkMode(tHDC, TRANSPARENT);

    TCHAR szHPText[32];
    _stprintf_s(szHPText, 32, TEXT("Lv: %d"), mPlayer->GetLevel());
    TextOut(tHDC, 265, 82, szHPText, _tcslen(szHPText));
}

void CUIEXPBar::UIClear()
{
}
