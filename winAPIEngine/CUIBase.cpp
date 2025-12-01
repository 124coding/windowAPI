#include "CUIBase.h"

#include "CInputMgr.h"

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
	if (!this->mbEnabled) return;

	GetFinalPos();

	for (CUIBase* childUI : mChilds) {
		childUI->OnUpdate(tDeltaTime);
	}
}

void CUIBase::OnLateUpdate(float tDeltaTime) {
	if (!this->mbEnabled) return;

	for (CUIBase* childUI : mChilds) {
		childUI->OnLateUpdate(tDeltaTime);
	}
}

void CUIBase::Render(HDC tHDC) {
	if (!this->mbEnabled) return;
	if (mWidth <= 0 || mHeight <= 0) return;

	Gdiplus::Graphics graphics(tHDC);
	Gdiplus::SolidBrush brush(mBackColor);

	if (mCornerRadius > 0) {
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		Gdiplus::GraphicsPath path;
		Gdiplus::Rect rect((int)mFinalPos.mX, (int)mFinalPos.mY, (int)mWidth, (int)mHeight);

		AddRoundedRectToPath(&path, rect, mCornerRadius);

		graphics.FillPath(&brush, &path);
	}
	else {
		graphics.FillRectangle(&brush,
			(int)mFinalPos.mX, (int)mFinalPos.mY,
			(int)mWidth, (int)mHeight);
	}

	int savedDC = 0;

	if (mbUseClipping) {
		savedDC = SaveDC(tHDC);
		IntersectClipRect(tHDC, (int)mFinalPos.mX, (int)mFinalPos.mY,
			(int)(mFinalPos.mX + mWidth), (int)(mFinalPos.mY + mHeight));
	}

	for (CUIBase* childUI : mChilds) {

		if (mbUseClipping) {
			if (IsOutsideRect(childUI)) {
				continue;
			}
		}

		childUI->Render(tHDC);
	}

	if (mbUseClipping) {
		RestoreDC(tHDC, savedDC);
	}
}

void CUIBase::UIClear()
{
	for (CUIBase* childUI : mChilds) {
		childUI->UIClear();
		SAFE_DELETE(childUI);
	}
}

void CUIBase::AddRoundedRectToPath(Gdiplus::GraphicsPath* path, Gdiplus::Rect rect, int cornerRadius) {
	int diameter = cornerRadius * 2;
	SVector2D size(diameter, diameter);
	Gdiplus::Rect cornerBox = Gdiplus::Rect(rect.X, rect.Y, size.mX, size.mY);

	path->StartFigure();

	// 좌상단 모서리
	path->AddArc(cornerBox, 180, 90);

	// 우상단 모서리
	cornerBox.X = rect.GetRight() - diameter;
	path->AddArc(cornerBox, 270, 90);

	// 우하단 모서리
	cornerBox.Y = rect.GetBottom() - diameter;
	path->AddArc(cornerBox, 0, 90);

	// 좌하단 모서리

	cornerBox.X = rect.X;
	path->AddArc(cornerBox, 90, 90);
	path->CloseFigure();
}

bool CUIBase::IsOutsideRect(CUIBase* tUIBase)
{
	RECT rcMyself = {
		(LONG)mFinalPos.mX, (LONG)mFinalPos.mY,
		(LONG)mFinalPos.mX + mWidth, (LONG)mFinalPos.mY + mHeight
	};

	RECT rcOther = {
		(LONG)tUIBase->GetFinalPos().mX, (LONG)tUIBase->GetFinalPos().mY,
		(LONG)(tUIBase->GetFinalPos().mX + tUIBase->GetWidth()), (LONG)(tUIBase->GetFinalPos().mY + tUIBase->GetHeight())
	};

	RECT rcTemp;
	if (IntersectRect(&rcTemp, &rcMyself, &rcOther) == FALSE) {
		return true;
	}
	return false;
}

CUIBase* CUIBase::FindTargetUI(SVector2D mousePos) {
	if (!GetEnabled() || mIgnoreMouse) return nullptr;

	// 자식들 먼저 검사 (보통 자식이 부모 위에 그려지므로)
	for (auto iter = mChilds.rbegin(); iter != mChilds.rend(); ++iter) {
		CUIBase* child = *iter;

		// 자식에게 재귀적으로 물어봅니다.
		CUIBase* target = child->FindTargetUI(mousePos);

		if (target != nullptr) {
			return target;
		}
	}

	if (mFinalPos.mX <= mousePos.mX && mousePos.mX <= mFinalPos.mX + mWidth &&
		mFinalPos.mY <= mousePos.mY && mousePos.mY <= mFinalPos.mY + mHeight)
	{
		return this;
	}

	return nullptr;
}
