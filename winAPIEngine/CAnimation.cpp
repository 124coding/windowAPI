#include "CAnimation.h"
#include "CTransform.h"
#include "GameObject.h"
#include "CAnimator.h"
#include "CRenderer.h"

HRESULT CAnimation::Load(const std::wstring& tPath)
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
        pos = mainCamera->CalculatePosition(pos);
    }

    CTexture::eTextureType type = mTexture->GetTextureType();
    SSprite sprite = mAnimationSheet[mIndex];

    float finalWidth = sprite.size.mX * scale.mX;
    float finalHeight = sprite.size.mY * scale.mY;

    float drawX = pos.mX - (gameObj->GetAnchorPoint().mX * finalWidth) + sprite.offset.mX;
    float drawY = pos.mY - (gameObj->GetAnchorPoint().mY * finalHeight) + sprite.offset.mY;

    if (drawX + finalWidth < 0 || drawX > windowWidth ||
        drawY + finalHeight < 0 || drawY > windowHeight)
    {
        return;
    }

    HDC imgHDC = mTexture->GetDCMem();

    if (mTexture->GetbAlpha()) {
        BLENDFUNCTION func = {};
        func.BlendOp = AC_SRC_OVER;
        func.BlendFlags = 0;
        func.AlphaFormat = AC_SRC_ALPHA;

        func.SourceConstantAlpha = (BYTE)(mAlphaMultiplier * 255.0f);
        AlphaBlend(tHDC,
            drawX, drawY,
            sprite.size.mX * scale.mX, sprite.size.mY * scale.mY,
            imgHDC,
            sprite.leftTop.mX, sprite.leftTop.mY,
            sprite.size.mX, sprite.size.mY,
            func);
    }
    else {
        TransparentBlt(tHDC,
            drawX, drawY,
            sprite.size.mX * scale.mX, sprite.size.mY * scale.mY,
            imgHDC,
            sprite.leftTop.mX, sprite.leftTop.mY,
            sprite.size.mX, sprite.size.mY,
            RGB(255, 0, 255));
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