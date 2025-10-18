#include "CBoxCollider2D.h"
#include "CTransform.h"
#include "GameObject.h"

void CBoxCollider2D::OnCreate()
{
}

void CBoxCollider2D::OnDestroy()
{
}

void CBoxCollider2D::OnUpdate(float tDeltaTime)
{
}

void CBoxCollider2D::OnLateUpdate(float tDeltaTime)
{
}

void CBoxCollider2D::Render(HDC tHDC)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, transparentBrush);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, greenPen);

	Rectangle(tHDC, 
		pos.mX + GetOffset().mX * GetSize().mX, pos.mY + GetOffset().mY * GetSize().mY,
		pos.mX + (GetOffset().mX + 100) * GetSize().mX, pos.mY + (GetOffset().mY + 100) * GetSize().mY);

	SelectObject(tHDC, oldBrush);
	SelectObject(tHDC, oldPen);

	DeleteObject(greenPen);
}
