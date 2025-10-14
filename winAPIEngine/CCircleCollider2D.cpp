#include "CCircleCollider2D.h"
#include "CTransform.h"
#include "GameObject.h"

void CCircleCollider2D::OnCreate()
{
}

void CCircleCollider2D::OnDestroy()
{
}

void CCircleCollider2D::OnUpdate(float tDeltaTime)
{
}

void CCircleCollider2D::OnLateUpdate(float tDeltaTime)
{
}

void CCircleCollider2D::Render(HDC tHDC)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, transparentBrush);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, greenPen);

	SVector2D rightBottom;
	rightBottom.mX = pos.mX + GetOffset().mX + 100 * GetSize().mX;
	rightBottom.mY = pos.mY + GetOffset().mY + 100 * GetSize().mY;

	Ellipse(tHDC,
		pos.mX + GetOffset().mX, pos.mY + GetOffset().mY,
		rightBottom.mX, rightBottom.mY);

	SelectObject(tHDC, oldBrush);
	SelectObject(tHDC, oldPen);

	DeleteObject(greenPen);
}
