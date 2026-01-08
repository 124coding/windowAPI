#pragma once
#include "CComponent.h"
#include "CAnimation.h"

#include <map>
#include <functional>

class CAnimator : public CComponent
{
public:
	// std::function을 래핑하여 함수 객체처럼 사용
	// 특정 애니메이션 시점에 실행될 콜백 함수를 저장함
	struct SEvent {
		std::function<void()> mEvent;

		// 함수 대입 연산자 오버로딩 ( event = func; )
		void operator=(std::function<void()> func) {
			this->mEvent = std::move(func);
		}

		// 함수 호출 연산자 오버로딩 ( event(); )
		void operator()() {
			if (mEvent) mEvent();
		}
	};

	// 애니메이션 하나당 가질 수 있는 3가지 주요 이벤트
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

	virtual CComponent* Clone() override {
		return new CAnimator(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// 1. 아틀라스(Sprite Sheet) 방식 생성
	// 큰 이미지 한 장에서 특정 영역(LeftTop, Size)을 잘라내어 프레임으로 만듦
	// tSpriteLength: 프레임 개수, tDuration: 각 프레임당 시간
	void CreateAnimation(const std::wstring& tName, CTexture* tSpriteSheet, SVector2D tLeftTop, SVector2D tSize, SVector2D tOffset, UINT tSpriteLength, float tDuration);

	// 2. 폴더 방식 생성
	// 폴더 내의 이미지 파일들을 순서대로 로딩하여 프레임으로 만듦
	void CreateAnimationByFolder(const std::wstring& tName, const std::wstring& tPath, SVector2D tOffset, float tDuration);

	// 이름으로 애니메이션 찾기
	CAnimation* FindAnimation(const std::wstring& tName);

	// 애니메이션 재생 (상태 전환)
	// 이미 재생 중인 애니메이션이면 다시 재생하지 않음(Re-entry 방지)
	void PlayAnimation(const std::wstring& tName, bool tLoop = true);

	// ==========================================
	// Event Binding (이벤트 연결)
	// ==========================================
	SEvents* FindEvents(const std::wstring tName);
	std::function<void()>& GetStartEvent(const std::wstring& tName);
	std::function<void()>& GetCompleteEvent(const std::wstring& tName);
	std::function<void()>& GetEndEvent(const std::wstring& tName);

	// 현재 애니메이션이 끝났는지 확인 (FSM 상태 전이 조건으로 주로 사용)
	bool IsCompleteAnimation() {
		return this->mActiveAnimation->IsComplete();
	};

	CAnimation* GetActiveAnimation() {
		return this->mActiveAnimation;
	}

private:
	std::map<std::wstring, CAnimation*> mAnimations;		// 보유한 모든 애니메이션 목록
	CAnimation* mActiveAnimation;							// 현재 재생 중인 애니메이션
	bool mbLoop;											// 반복 재생 여부

	std::map < std::wstring, SEvents*> mEvents;				// 애니메이션별 이벤트 목록
};

