#include "CAnimator.h"

void CAnimator::OnCreate()
{
}

void CAnimator::OnDestroy()
{
}

void CAnimator::OnUpdate(float tDeltaTime)
{
	if (mActiveAnimation) {
		mActiveAnimation->OnUpdate(tDeltaTime);

		if (mActiveAnimation->IsComplete() == true && mbLoop == true) {
			mActiveAnimation->Reset();
		}
	}
}

void CAnimator::OnLateUpdate(float tDeltaTime)
{
}

void CAnimator::Render(HDC tHDC)
{
	if (mActiveAnimation) {
		mActiveAnimation->Render(tHDC);
	}
}

void CAnimator::CreateAnimation(const std::wstring& tName, 
	CTexture* tSpriteSheet, 
	SVector2D tLeftTop, 
	SVector2D tSize, SVector2D tOffset, 
	UINT tSpriteLength, float tDuration)
{
	CAnimation* animation = nullptr;
	animation = FindAnimation(tName);
	
	if (animation != nullptr) return;

	animation = new CAnimation();
	animation->CreateAnimation(tName, tSpriteSheet, tLeftTop, tSize, tOffset, tSpriteLength, tDuration);

	animation->SetAnimator(this);

	mAnimations.insert(std::make_pair(tName, animation));
}

CAnimation* CAnimator::FindAnimation(const std::wstring& tName)
{
	auto iter = mAnimations.find(tName);

	if (iter == mAnimations.end()) {
		return nullptr;
	}

	return iter->second;
}

void CAnimator::PlayAnimation(const std::wstring& tName, bool tLoop)
{
	CAnimation* animation = FindAnimation(tName);

	if (animation == nullptr) return;

	mActiveAnimation = animation;
	mActiveAnimation->Reset();
	mbLoop = tLoop;
}
