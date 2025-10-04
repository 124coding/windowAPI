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

    if (mainCamera) {
        pos = mainCamera->CaluatePosition(pos);
    }

    BLENDFUNCTION func = {};
    func.BlendOp = AC_SRC_OVER;
    func.BlendFlags = 0;
    func.AlphaFormat = AC_SRC_ALPHA;
    func.SourceConstantAlpha = 255;

    SSprite sprite = mAnimationSheet[mIndex];
    HDC imgHDC = mTexture->GetDCMem();

    AlphaBlend(tHDC,
        pos.mX, pos.mY, 
        sprite.size.mX * 5, sprite.size.mY * 5, 
        imgHDC, 
        sprite.leftTop.mX, sprite.leftTop.mY, 
        sprite.size.mX, sprite.size.mY, 
        func);
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
    mTime = 0.0f;
    mIndex = 0;
    mbComplete = false;
}