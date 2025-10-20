#include "CUIButton.h"

#include "CInputMgr.h"

void CUIButton::OnCreate()
{
	SetPos(SVector2D(200.0f, 200.0f));
	SetSize(SVector2D(200.0f, 200.0f));

	mOnClick = std::bind(&CUIButton::ButtonClick, this);
}

void CUIButton::Active()
{
}

void CUIButton::InActive()
{
}

void CUIButton::OnDestroy()
{
}

void CUIButton::OnUpdate(float tDeltaTime)
{
	SVector2D mousePos = CInputMgr::GetMousePosition();

	if (mPos.mX <= mousePos.mX && mousePos.mX <= mPos.mX + mSize.mX &&
		mPos.mY <= mousePos.mY && mousePos.mY <= mPos.mY + mSize.mY) {

		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			mOnClick();
		};
	}
}

void CUIButton::OnLateUpdate(float tDeltaTime)
{
}

void CUIButton::Render(HDC tHDC)
{
	Rectangle(tHDC, (int)mPos.mX, (int)mPos.mY, mPos.mX + mSize.mX, mPos.mY + mSize.mY);
}

void CUIButton::UIClear()
{
}

void CUIButton::ButtonClick()
{
}
