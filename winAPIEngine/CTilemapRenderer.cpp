#include "CTilemapRenderer.h"

#include "CTransform.h"

#include "CRenderer.h"

#include "winMacro.h"

#include <assert.h>

SVector2D CTilemapRenderer::TileSize = SVector2D(1.0f, 1.0f);

void CTilemapRenderer::OnCreate()
{
	TileSize = mTileSize * GetOwner()->GetSize();
}

void CTilemapRenderer::OnDestroy()
{
	mTexture = nullptr;
}

void CTilemapRenderer::OnUpdate(float tDeltaTime)
{
}

void CTilemapRenderer::OnLateUpdate(float tDeltaTime)
{
}

void CTilemapRenderer::Render(HDC tHDC)
{
	if (mTexture == nullptr) {
		return;
	}
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	SVector2D pos = tr->GetPos();
	float rot = tr->GetRot();
	SVector2D scale = tr->GetScale();

	float fScaleX = GetOwner()->GetSize().mX * scale.mX;
	float fScaleY = GetOwner()->GetSize().mY * scale.mY;

	pos = mainCamera->CalculatePosition(pos);

	if (mTexture->GetTextureType() == CTexture::eTextureType::Bmp) {

		if (mTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;

			func.SourceConstantAlpha = 255;
			AlphaBlend(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * scale.mX, pos.mY - GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				mTexture->GetDCMem(),
				mIndex.mX * mTileSize.mX, mIndex.mY * mTileSize.mY,
				mTileSize.mX, mTileSize.mY, func);
		}
		else {
			TransparentBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * scale.mX, pos.mY - GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				mTexture->GetDCMem(),
				mIndex.mX * mTileSize.mX, mIndex.mY * mTileSize.mY,
				mTileSize.mX, mTileSize.mY,
				RGB(255, 0, 255));
		}
	}
	else if (mTexture->GetTextureType() == CTexture::eTextureType::Png) {
		Gdiplus::ImageAttributes imgAtt = {};

		// 투명화 시킬 픽셀의 색 범위
		imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

		Gdiplus::Graphics graphics(tHDC);

		graphics.TranslateTransform(pos.mX, pos.mY);
		graphics.RotateTransform(rot);
		graphics.ScaleTransform(fScaleX, fScaleY);

		Gdiplus::RectF destRect(
			-GetOwner()->GetAnchorPoint().mX,
			-GetOwner()->GetAnchorPoint().mY,
			mTileSize.mX,                    
			mTileSize.mY                     
		);

		float srcX = mIndex.mX * mTileSize.mX;
		float srcY = mIndex.mY * mTileSize.mY;
		float srcWidth = mTileSize.mX;
		float srcHeight = mTileSize.mY;

		graphics.DrawImage(mTexture->GetImage(),
			destRect,
			srcX, srcY,
			srcWidth, srcHeight,
			Gdiplus::UnitPixel
		);
	}
}