#pragma once

#include "CResource.h"
#include "CTexture.h"
#include "SVector2D.h"

#include <vector>

class CAnimator;

class CAnimation : public CResource
{
public:
	struct SSprite {
		SVector2D leftTop;
		SVector2D size;
		SVector2D offset;
		float duration;

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

	HRESULT Load(CAPIEngine* tEngine, const std::wstring& tPath) override;

	void OnUpdate(float tDeltaTime);
	void Render(HDC tHDC);

	void CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration);
	void Reset();

	bool IsComplete() { 
		return this->mbComplete; 
	}
	void SetAnimator(CAnimator* tAnimator) {
		this->mAnimator = tAnimator;
	}

private:
	CAnimator* mAnimator;
	CTexture* mTexture;

	std::vector<SSprite> mAnimationSheet;
	int mIndex;
	float mTime;
	bool mbComplete;
};

