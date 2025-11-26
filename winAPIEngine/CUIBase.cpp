#include "CUIBase.h"

void CUIBase::OnCreate() {
	for (CUIBase* childUI : mChilds) {
		childUI->OnCreate();
	}
}

void CUIBase::Active()
{
	mbEnabled = true;
}

void CUIBase::InActive()
{
	mbEnabled = false;
}

void CUIBase::OnDestroy() {
	for (CUIBase* childUI : mChilds) {
		childUI->OnDestroy();
		SAFE_DELETE(childUI);
	}
}

void CUIBase::OnUpdate(float tDeltaTime) {
	GetFinalPos();

	for (CUIBase* childUI : mChilds) {
		if (!childUI->mbEnabled) continue;
		childUI->OnUpdate(tDeltaTime);
	}
}

void CUIBase::OnLateUpdate(float tDeltaTime) {
	for (CUIBase* childUI : mChilds) {
		if (!childUI->mbEnabled) continue;
		childUI->OnLateUpdate(tDeltaTime);
	}
}

void CUIBase::Render(HDC tHDC) {
	for (CUIBase* childUI : mChilds) {
		if (!childUI->mbEnabled) continue;
		childUI->Render(tHDC);
	}
}

void CUIBase::UIClear()
{
	for (CUIBase* childUI : mChilds) {
		childUI->UIClear();
	}
}
