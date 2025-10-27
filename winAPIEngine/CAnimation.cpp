#include "CAnimation.h"
#include "CTransform.h"
#include "GameObject.h"
#include "CAnimator.h"
#include "CRenderer.h"

HRESULT CAnimation::Load(CAPIEngine* tEngine, const std::wstring& tPath)
{
    return E_NOTIMPL;
}

void CAnimation::OnUpdate(float tDeltaTime)
{
    if (mbComplete) return;

    mTime += tDeltaTime;

    if (mAnimationSheet[mIndex].duration < mTime) {
        mTime = 0.0f;
        if (mIndex < mAnimationSheet.size() - 1) mIndex++;
        else {
            mbComplete = true;
        }
    }
}

void CAnimation::Render(HDC tHDC) {

    if (mTexture == nullptr) return;

    GameObject* gameObj = mAnimator->GetOwner();
    CTransform* tr = gameObj->GetComponent<CTransform>();
    SVector2D pos = tr->GetPos();
    float rot = tr->GetRot();
    SVector2D scale = tr->GetScale();

    if (mainCamera) {
        pos = mainCamera->CaluatePosition(pos);
    }

    CTexture::eTextureType type = mTexture->GetTextureType();
    SSprite sprite = mAnimationSheet[mIndex];

    if (type == CTexture::eTextureType::Bmp) {

        HDC imgHDC = mTexture->GetDCMem();

        if (mTexture->GetbAlpha()) {
            BLENDFUNCTION func = {};
            func.BlendOp = AC_SRC_OVER;
            func.BlendFlags = 0;
            func.AlphaFormat = AC_SRC_ALPHA;
            func.SourceConstantAlpha = 255;

            AlphaBlend(tHDC,
                pos.mX - ((sprite.size.mX / 2.0f) + sprite.offset.mX) * scale.mX, pos.mY - (sprite.size.mY + sprite.offset.mY) * scale.mY,
                sprite.size.mX * scale.mX, sprite.size.mY * scale.mY,
                imgHDC,
                sprite.leftTop.mX, sprite.leftTop.mY,
                sprite.size.mX, sprite.size.mY,
                func);
        }
        else {
            TransparentBlt(tHDC,
                pos.mX - ((sprite.size.mX / 2.0f) + sprite.offset.mX) * scale.mX, pos.mY - (sprite.size.mY + sprite.offset.mY) * scale.mY,
                sprite.size.mX * scale.mX, sprite.size.mY * scale.mY,
                imgHDC,
                sprite.leftTop.mX, sprite.leftTop.mY,
                sprite.size.mX, sprite.size.mY,
                RGB(255, 0, 255));
        }

    }
    else if (type == CTexture::eTextureType::Png) {
        Gdiplus::ImageAttributes imgAtt = {};

        // 투명화 시킬 픽셀의 색 범위
        imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));
        
        Gdiplus::Graphics graphics(tHDC);
        Gdiplus::GraphicsState state = graphics.Save();

        graphics.TranslateTransform(pos.mX, pos.mY);
        graphics.ScaleTransform(scale.mX, scale.mY);
        graphics.RotateTransform(rot);
        
        float originalWidth = sprite.size.mX;
        float originalHeight = sprite.size.mY;

        graphics.DrawImage(mTexture->GetImage(),
            -originalWidth / 2.0f,
            -originalHeight,      
            originalWidth,
            originalHeight
        );
    }
}

void CAnimation::CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration) {
    mTexture = tSpriteSheet;

    for (size_t i = 0; i < tSpriteLength; i++) {
        SSprite sprite = {};
        sprite.leftTop.mX = tLeftTop.mX + (tSize.mX * i);
        sprite.leftTop.mY = tLeftTop.mY;
        sprite.size = tSize;
        sprite.offset = tOffset;
        sprite.duration = tDuration;

        mAnimationSheet.push_back(sprite);
    }
}

void CAnimation::Reset() {
    this->mTime = 0.0f;
    this->mIndex = 0;
    this->mbComplete = false;
}