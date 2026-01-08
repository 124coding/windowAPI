#pragma once

#include "CResource.h"
#include "CTexture.h"
#include "SVector2D.h"

#include <vector>

class CAnimator;

class CAnimation : public CResource
{
public:
	// 개별 프레임(컷) 정보를 담는 구조체
	struct SSprite {
		SVector2D leftTop;	// 스프라이트 시트 내에서의 시작 좌표 (Left-Top UV)
		SVector2D size;		// 잘라낼 크기 (Width, Height)
		SVector2D offset;	// 중심점 보정값 (Pivot 오차 수정용)
		float duration;		// 해당 프레임이 유지되는 시간 (초 단위)

		SSprite() :
			leftTop(SVector2D(0.0f, 0.0f)),
			size(SVector2D(0.0f, 0.0f)),
			offset(SVector2D(0.0f, 0.0f)),
			duration(0.0f) {

		}
	};

	CAnimation() :
		CResource(eResourceType::Animation),
		mAnimator(nullptr),
		mTexture(nullptr),
		mAnimationSheet{},
		mIndex(-1),
		mTime(0.0f),
		mbComplete(false) {
	};

	~CAnimation() {}

	HRESULT Load(const std::wstring& tPath) override;

	void OnUpdate(float tDeltaTime);
	void Render(HDC tHDC);

	void CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration);
	void Reset();

	bool IsComplete() { 
		return this->mbComplete; 
	}

	// 이 애니메이션을 소유한 컴포넌트(Animator) 등록
	void SetAnimator(CAnimator* tAnimator) {
		this->mAnimator = tAnimator;
	}

	// 투명도 조절 (페이드 인/아웃 효과 등에 사용)
	void SetAlphaMultiplier(float tAlpha) {
		this->mAlphaMultiplier = tAlpha;
	}

private:
	CAnimator* mAnimator;
	CTexture* mTexture;						// 애니메이션에 사용되는 원본 이미지 (스프라이트 시트)

	std::vector<SSprite> mAnimationSheet;	// 프레임 정보들의 목록
	int mIndex;								// 현재 재생 중인 프레임 번호
	float mTime;							// 현재 프레임의 누적 진행 시간
	bool mbComplete;						// 애니메이션 종료 여부 (Loop가 아닐 때 확인용)

	float mAlphaMultiplier = 1.0f;

	bool mFlipImage = false;				// 좌우 반전 여부
};

