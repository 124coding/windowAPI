#pragma once
#include "CComponent.h"
#include "CAnimation.h"

#include <map>

class CAnimator : public CComponent
{
public:
	CAnimator() : 
		CComponent(eComponentType::Animator),
		mAnimations{},
		mActiveAnimation(nullptr),
		mbLoop(false) {
	}
	virtual ~CAnimator() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;

	void CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration);
	CAnimation* FindAnimation(const std::wstring& tName);
	void PlayAnimation(const std::wstring& tName, bool tLoop = true);

private:
	std::map<std::wstring, CAnimation*> mAnimations;
	CAnimation* mActiveAnimation;
	bool mbLoop;
};

