#pragma once
#include "CComponent.h"
#include "CAnimation.h"

#include <map>
#include <functional>

class CAnimator : public CComponent
{
public:
	struct SEvent {
		std::function<void()> mEvent;

		void operator=(std::function<void()> func) {
			this->mEvent = std::move(func);
		}

		void operator()() {
			if (mEvent) mEvent();
		}
	};

	struct SEvents {
		SEvent startEvent;
		SEvent completeEvent;
		SEvent endEvent;
	};

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
	void Render(HDC tHDC) override;

	void CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration);
	CAnimation* FindAnimation(const std::wstring& tName);
	void PlayAnimation(const std::wstring& tName, bool tLoop = true);

	SEvents* FindEvents(const std::wstring tName);
	std::function<void()>& GetStartEvent(const std::wstring& tName);
	std::function<void()>& GetCompleteEvent(const std::wstring& tName);
	std::function<void()>& GetEndEvent(const std::wstring& tName);

	bool IsCompleteAnimation() {
		return this->mActiveAnimation->IsComplete();
	};

private:
	std::map<std::wstring, CAnimation*> mAnimations;
	CAnimation* mActiveAnimation;
	bool mbLoop;

	std::map < std::wstring, SEvents*> mEvents;
};

