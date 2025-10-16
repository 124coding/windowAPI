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

	pos = mainCamera->CaluatePosition(pos);

	if (mTexture->GetTextureType() == CTexture::eTextureType::Bmp) {

		if (mTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;

			func.SourceConstantAlpha = 255;
			AlphaBlend(tHDC, pos.mX, pos.mY,
				mTileSize.mX * GetOwner()->GetSize().mX * scale.mX, mTileSize.mY * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetDCMem(),
				mIndex.mX * mTileSize.mX, mIndex.mY * mTileSize.mY,
				mTileSize.mX, mTileSize.mY, func);
		}
		else {
			TransparentBlt(tHDC, pos.mX, pos.mY,
				mTileSize.mX * GetOwner()->GetSize().mX * scale.mX, mTileSize.mY * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetDCMem(),
				mIndex.mX * mTileSize.mX, mIndex.mY * mTileSize.mY,
				mTileSize.mX, mTileSize.mY,
				RGB(255, 0, 255));
		}
	}
	//else if (mTexture->GetTextureType() == CTexture::eTextureType::Png) {
	//	Gdiplus::ImageAttributes imgAtt = {};

	//	// 투명화 시킬 픽셀의 색 범위
	//	imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

	//	Gdiplus::Graphics graphics(tHDC);

	//	graphics.TranslateTransform(pos.mX, pos.mY);
	//	graphics.RotateTransform(rot);
	//	graphics.TranslateTransform(-pos.mX, -pos.mY);

	//	int a = pos.mX;
	//	int b = pos.mY;
	//	graphics.DrawImage(mTexture->GetImage(), Gdiplus::Rect(
	//		pos.mX, pos.mY,
	//		mTileSize.mX * GetOwner()->GetSize().mX * scale.mX, mTileSize.mY * GetOwner()->GetSize().mY * scale.mY),
	//		mTileSize.mX, mTileSize.mY,
	//		mIndex.mX * mTileSize.mX, mIndex.mY * mTileSize.mY,
	//		Gdiplus::UnitPixel,
	//		nullptr
	//	);
	//}
	// Gdiplus::Graphics graphics(tHDC);
	// graphics.DrawImage(mImage, Gdiplus::Rect(pos.mX, pos.mY, mWidth, mHeight));
	// Rectangle(tHDC, pos.mX, pos.mY, pos.mX + 100, pos.mY + 100);
}