#include "CBoxCollider2D.h"

#include "GameObject.h"

#include "CRenderer.h"

#include "CTransform.h"

#include "Object.h"

void CBoxCollider2D::OnCreate()
{
	CCollider::OnCreate();
}

void CBoxCollider2D::OnDestroy()
{
	CCollider::OnDestroy();
}

void CBoxCollider2D::OnUpdate(float tDeltaTime)
{
	CCollider::OnUpdate(tDeltaTime);
}

void CBoxCollider2D::OnLateUpdate(float tDeltaTime)
{
	CCollider::OnLateUpdate(tDeltaTime);
}

void CBoxCollider2D::Render(HDC tHDC)
{
	CCollider::Render(tHDC);

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	SVector2D pos = tr->GetPos();
	pos = mainCamera->CalculatePosition(pos);

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, transparentBrush);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, greenPen);

	// 기본 사각형 그리기
	/*SVector2D leftTop;
	leftTop.mX = pos.mX - GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * GetSize().mX + GetOffset().mX;
	leftTop.mY = pos.mY - GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * GetSize().mY + GetOffset().mY;

	SVector2D rightBottom = leftTop + (ObjectSize(GetOwner()) * GetSize());

	Rectangle(tHDC, 
		leftTop.mX, leftTop.mY,
		rightBottom.mX, rightBottom.mY);*/

	// 회전 사각형 그리기
	float fCos = cos(DegToRad(tr->GetRot()));
	float fSin = sin(DegToRad(tr->GetRot()));

	SVector2D vLocalLt;
	vLocalLt.mX = -GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * GetSize().mX + GetOffset().mX;
	vLocalLt.mY = -GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * GetSize().mY + GetOffset().mY;

	SVector2D vScaledSize = ObjectSize(GetOwner()) * GetSize();

	SVector2D vLocalRt = { vLocalLt.mX + vScaledSize.mX, vLocalLt.mY };
	SVector2D vLocalLb = { vLocalLt.mX, vLocalLt.mY + vScaledSize.mY };
	SVector2D vLocalRb = { vLocalLt.mX + vScaledSize.mX, vLocalLt.mY + vScaledSize.mY };

	SVector2D vRotLt, vRotLb, vRotRt, vRotRb;

	vRotLt = SVector2D(vLocalLt.mX * fCos - vLocalLt.mY * fSin, vLocalLt.mX * fSin + vLocalLt.mY * fCos);
	vRotLb = SVector2D(vLocalLb.mX * fCos - vLocalLb.mY * fSin, vLocalLb.mX * fSin + vLocalLb.mY * fCos);
	vRotRt = SVector2D(vLocalRt.mX * fCos - vLocalRt.mY * fSin, vLocalRt.mX * fSin + vLocalRt.mY * fCos);
	vRotRb = SVector2D(vLocalRb.mX * fCos - vLocalRb.mY * fSin, vLocalRb.mX * fSin + vLocalRb.mY * fCos);

	SVector2D vFinalLt = pos + vRotLt;
	SVector2D vFinalLb = pos + vRotLb;
	SVector2D vFinalRt = pos + vRotRt;
	SVector2D vFinalRb = pos + vRotRb;

	MoveToEx(tHDC, (int)vFinalLt.mX, (int)vFinalLt.mY, NULL);
	LineTo(tHDC, (int)vFinalRt.mX, (int)vFinalRt.mY);
	LineTo(tHDC, (int)vFinalRb.mX, (int)vFinalRb.mY);
	LineTo(tHDC, (int)vFinalLb.mX, (int)vFinalLb.mY);
	LineTo(tHDC, (int)vFinalLt.mX, (int)vFinalLt.mY);

	SelectObject(tHDC, oldBrush);
	SelectObject(tHDC, oldPen);

	DeleteObject(greenPen);
}
