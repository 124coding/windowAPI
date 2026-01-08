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

    // 시간 누적
    mTime += tDeltaTime;

    // 현재 프레임의 유지 시간(duration)을 초과했는지 확인
    if (mAnimationSheet[mIndex].duration < mTime) {
        mTime = 0.0f;

        // 다음 프레임으로 이동
        if (mIndex < mAnimationSheet.size() - 1) mIndex++;
        else {
            // 마지막 프레임이면 완료 처리 (Loop 처리는 Animator에서 담당)
            mbComplete = true;
        }
    }
}

void CAnimation::Render(HDC tHDC) {

    if (mTexture == nullptr) return;

    // 1. 오브젝트의 Transform 정보 가져오기
    GameObject* gameObj = mAnimator->GetOwner();
    

    CTransform* tr = gameObj->GetComponent<CTransform>(eComponentType::Transform);
    SVector2D pos = tr->GetPos();
    float rot = tr->GetRot();
    SVector2D scale = tr->GetScale();

    // 2. 카메라 좌표 변환 (월드 좌표 -> 스크린 좌표)
    if (mainCamera) {
        pos = mainCamera->CalculatePosition(pos);
    }

    CTexture::eTextureType type = mTexture->GetTextureType();
    SSprite sprite = mAnimationSheet[mIndex]; // 현재 그려야 할 프레임 정보

    // 3. 실제 렌더링 크기 계산 (오브젝트 기본 크기 * Transform 스케일 * 스프라이트 자체 크기)
    // 보통 애니메이션은 sprite.size를 기준으로 그리는 경우가 많으므로 확인 필요
    float finalWidthSize = gameObj->GetSize().mX * scale.mX;
    float finalHeightSize = gameObj->GetSize().mY * scale.mY;

    // 4. 그리기 시작 위치 계산 (앵커 포인트 및 오프셋 반영)
    float drawX = pos.mX - (gameObj->GetAnchorPoint().mX * finalWidthSize) + sprite.offset.mX;
    float drawY = pos.mY - (gameObj->GetAnchorPoint().mY * finalHeightSize) + sprite.offset.mY;

    // 5. 화면 밖 컬링 (Frustum Culling 최적화)
    // 그릴 영역이 윈도우 화면을 완전히 벗어났다면 그리지 않음
    if (drawX + sprite.size.mX < 0 || drawX > windowWidth ||
        drawY + sprite.size.mY < 0 || drawY > windowHeight)
    {
        return;
    }

    HDC imgHDC = mTexture->GetDCMem();

    // 6. 렌더링 (AlphaBlend vs TransparentBlt)
    if (mTexture->GetbAlpha()) {
        BLENDFUNCTION func = {};
        func.BlendOp = AC_SRC_OVER;
        func.BlendFlags = 0;
        func.AlphaFormat = AC_SRC_ALPHA;    // 32비트 비트맵의 알파 채널 사용

        // 전체 투명도(AlphaMultiplier) 적용
        func.SourceConstantAlpha = (BYTE)(mAlphaMultiplier * 255.0f);
        AlphaBlend(tHDC,
            drawX, drawY,
            sprite.size.mX * finalWidthSize, sprite.size.mY * finalHeightSize,
            imgHDC,
            sprite.leftTop.mX, sprite.leftTop.mY,
            sprite.size.mX, sprite.size.mY,
            func);
    }
    else {
        // 특정 색상(마젠타)을 투명하게 처리
        TransparentBlt(tHDC,
            drawX, drawY,
            sprite.size.mX * finalWidthSize, sprite.size.mY * finalHeightSize,
            imgHDC,
            sprite.leftTop.mX, sprite.leftTop.mY,
            sprite.size.mX, sprite.size.mY,
            RGB(255, 0, 255));
    }
}

// 스프라이트 시트를 일정한 간격으로 잘라서 애니메이션 프레임 생성
void CAnimation::CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration) {
    mTexture = tSpriteSheet;

    for (size_t i = 0; i < tSpriteLength; i++) {
        SSprite sprite = {};

        // 가로 방향으로 이동하며 좌표 계산
        sprite.leftTop.mX = tLeftTop.mX + (tSize.mX * i);
        sprite.leftTop.mY = tLeftTop.mY;
        sprite.size = tSize;
        sprite.offset = tOffset;
        sprite.duration = tDuration;    // 모든 프레임 동일한 시간

        mAnimationSheet.push_back(sprite);
    }
}

void CAnimation::Reset() {
    this->mTime = 0.0f;
    this->mIndex = 0;
    this->mbComplete = false;
}