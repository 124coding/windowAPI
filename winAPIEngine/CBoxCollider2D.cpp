#include "CBoxCollider2D.h"

#include "GameObject.h"

#include "CRenderer.h"

#include "CTransform.h"

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
	pos = mainCamera->CaluatePosition(pos);

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, transparentBrush);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, greenPen);

	Rectangle(tHDC, 
		pos.mX - GetOwner()->GetAnchorPoint().mX * GetSize().mX + GetOffset().mX, pos.mY - GetOwner()->GetAnchorPoint().mY * GetSize().mY + GetOffset().mY,
		pos.mX * GetSize().mX + GetOffset().mX, pos.mY + GetOffset().mY);

	SelectObject(tHDC, oldBrush);
	SelectObject(tHDC, oldPen);

	DeleteObject(greenPen);
}
