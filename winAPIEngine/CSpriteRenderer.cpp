#include "CSpriteRenderer.h"
#include "GameObject.h"

#include "CSceneMgr.h"

#include "CTransform.h"

#include "CRenderer.h"

#include "winMacro.h"

#include <assert.h>

void CSpriteRenderer::OnCreate()
{
}

void CSpriteRenderer::OnDestroy()
{

}

void CSpriteRenderer::OnUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::OnLateUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::Render(HDC tHDC)
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

	if (mbFlipX) {
		fScaleX *= -1.0f;
	}

	if (mbFlipY) {
		fScaleY *= -1.0f;
	}

	pos = mainCamera->CaluatePosition(pos);

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
				0, 0,
				mTexture->GetWidth(), mTexture->GetHeight(), func);
		}
		else {
			TransparentBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * GetOwner()->GetSize().mX * scale.mX, pos.mY - GetOwner()->GetAnchorPoint().mY * GetOwner()->GetSize().mY * scale.mY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				mTexture->GetDCMem(),
				0, 0,
				mTexture->GetWidth(), mTexture->GetHeight(),
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

		float originalWidth = mTexture->GetWidth();
		float originalHeight = mTexture->GetHeight();

		graphics.DrawImage(mTexture->GetImage(),
			-GetOwner()->GetAnchorPoint().mX,
			-GetOwner()->GetAnchorPoint().mY,
			originalWidth,
			originalHeight
		);
	}
}