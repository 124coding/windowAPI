#include "CUIButton.h"

#include "CTexture.h"

#include "CInputMgr.h"
#include "CResourceMgr.h"

#include <string>

void CUIButton::OnCreate()
{
	CUIBase::OnCreate();
}

void CUIButton::Active()
{
	CUIBase::Active();
}

void CUIButton::InActive()
{
	CUIBase::InActive();
}

void CUIButton::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CUIButton::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
	
	if (mIsHover) {
		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			mOnClick();
		};
	}
}

void CUIButton::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CUIButton::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CUIButton::UIClear()
{
	CUIBase::UIClear();
}
