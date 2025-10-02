#include "CSpriteRenderer.h"
#include "GameObject.h"
#include "CTransform.h"

#include "winMacro.h"

void CSpriteRenderer::OnCreate()
{
}

void CSpriteRenderer::OnDestroy()
{
	SAFE_DELETE(mImage);
}

void CSpriteRenderer::OnUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::OnLateUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::Render(HDC hDC)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();

	Gdiplus::Graphics graphics(hDC);
	graphics.DrawImage(mImage, Gdiplus::Rect(pos.mX, pos.mY, mWidth, mHeight));
	// Rectangle(hDC, pos.mX, pos.mY, pos.mX + 100, pos.mY + 100);
}

void CSpriteRenderer::ImageLoad(const std::wstring& path)
{
	mImage = Gdiplus::Image::FromFile(path.c_str());
	mWidth = mImage->GetWidth();
	mHeight = mImage->GetHeight();
}
