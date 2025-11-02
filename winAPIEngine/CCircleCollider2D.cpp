#include "CCircleCollider2D.h"

#include "GameObject.h"

#include "CRenderer.h"
#include "CTransform.h"

#include "Object.h"

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
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	SVector2D pos = tr->GetPos();

	pos = mainCamera->CaluatePosition(pos);

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, transparentBrush);

	HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, greenPen);

	SVector2D leftTop;
	leftTop.mX = pos.mX - GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * tr->GetScale().mX * GetSize().mX + GetOffset().mX;
	leftTop.mY = pos.mY - GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * tr->GetScale().mY * GetSize().mY + GetOffset().mY;

	SVector2D rightBottom = leftTop + (ObjectSize(GetOwner()) * GetSize());

	Ellipse(tHDC,
		leftTop.mX, leftTop.mY,
		rightBottom.mX, rightBottom.mY);

	SelectObject(tHDC, oldBrush);
	SelectObject(tHDC, oldPen);

	DeleteObject(greenPen);
}
