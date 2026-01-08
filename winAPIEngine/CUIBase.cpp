#include "CUIBase.h"

#include "CInputMgr.h"

void CUIBase::OnCreate() {
	// 자식 UI들도 재귀적으로 초기화

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
	// 자식 UI들의 메모리 해제 및 정리
	for (CUIBase* childUI : mChilds) {
		childUI->OnDestroy();
		SAFE_DELETE(childUI);
	}
}

void CUIBase::OnUpdate(float tDeltaTime) {
	if (!this->mbEnabled) return;

	// 매 프레임 월드 상의 최종 좌표를 갱신해둬야 렌더링이나 충돌 처리가 정확함
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

	// GDI+ 그래픽 객체 생성
	Gdiplus::Graphics graphics(tHDC);
	Gdiplus::SolidBrush brush(mBackColor);

	// 1. 배경 그리기 (둥근 모서리 vs 일반 사각형)
	if (mCornerRadius > 0) {
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);	// 계단 현상 제거

		Gdiplus::GraphicsPath path;
		Gdiplus::Rect rect((int)mFinalPos.mX, (int)mFinalPos.mY, (int)mWidth, (int)mHeight);

		AddRoundedRectToPath(&path, rect, mCornerRadius);	// 경로 생성

		graphics.FillPath(&brush, &path);
	}
	else {
		graphics.FillRectangle(&brush,
			(int)mFinalPos.mX, (int)mFinalPos.mY,
			(int)mWidth, (int)mHeight);
	}

	int savedDC = 0;

	// 2. 클리핑(Clipping) 처리: 자식 UI가 부모 영역을 벗어나면 잘라내기
	if (mbUseClipping) {
		savedDC = SaveDC(tHDC);	// 현재 DC 상태 저장 (클리핑 영역 적용 전)

		// 그리기 영역을 부모의 사각형으로 제한
		IntersectClipRect(tHDC, (int)mFinalPos.mX, (int)mFinalPos.mY,
			(int)(mFinalPos.mX + mWidth), (int)(mFinalPos.mY + mHeight));
	}

	// 3. 자식 UI 렌더링
	for (CUIBase* childUI : mChilds) {

		// 최적화: 클리핑 사용 시, 화면(부모 영역) 밖에 있는 자식은 아예 그리기 시도조차 안 함
		if (mbUseClipping) {
			if (IsOutsideRect(childUI)) {
				continue;
			}
		}

		childUI->Render(tHDC);
	}

	// 클리핑 해제: 저장해둔 DC 상태로 복구 (다음 UI 그리기에 영향 안 주도록)
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

// 둥근 사각형 경로 생성 (4개의 모서리에 호(Arc)를 추가하는 방식)
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

// AABB 충돌 검사를 이용하여 UI가 부모 영역과 겹치는지 확인
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
	// 두 사각형의 교집합이 없으면(FALSE) 밖에 있는 것
	if (IntersectRect(&rcTemp, &rcMyself, &rcOther) == FALSE) {
		return true;
	}
	return false;
}

// 마우스 좌표에 해당하는 UI 찾기 (재귀 호출)
CUIBase* CUIBase::FindTargetUI(SVector2D mousePos) {
	if (!GetEnabled() || mIgnoreMouse) return nullptr;

	// 역순 순회
	// 자식들 먼저 검사 (보통 자식이 부모 위에 그려지므로)
	for (auto iter = mChilds.rbegin(); iter != mChilds.rend(); ++iter) {
		CUIBase* child = *iter;

		// 자식에게 재귀적으로 물어봄 (자식의 자식 확인)
		CUIBase* target = child->FindTargetUI(mousePos);

		if (target != nullptr) {
			return target;
		}
	}

	// 자식 중에 타겟이 없다면, 나 자신이 범위 안에 있는지 확인
	if (mFinalPos.mX <= mousePos.mX && mousePos.mX <= mFinalPos.mX + mWidth &&
		mFinalPos.mY <= mousePos.mY && mousePos.mY <= mFinalPos.mY + mHeight)
	{
		return this;
	}

	return nullptr;
}

// 부모 변경 (좌표계 변환 포함)
CUIBase* CUIBase::Reparent(CUIBase* tNewParent, bool keepWorldPosition, SVector2D tWantPos)
{
	if (!tNewParent || tNewParent == mParent) return nullptr;

	SVector2D currentWorldPos = GetFinalPos();

	if (mParent) {
		mParent->RemoveChild(this); // 부모의 mChildren 벡터에서 제거
	}

	mParent = tNewParent;
	mParent->AddChild(this);

	// 좌표 재계산
	if (keepWorldPosition) {
		SVector2D parentWorldPos = tNewParent->GetFinalPos();
		mPos = currentWorldPos - parentWorldPos;
	}
	else {
		// 그냥 새 부모 기준 (0,0)이나 지정된 위치로 가고 싶을 때
		mPos = tWantPos;
	}

	GetFinalPos();

	return this;
}
