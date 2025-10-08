#pragma once

#include "CScript.h"

class CAnimator;

class CPlayerScript : public CScript
{
public:
	enum class eState {
		SitDown,
		Walk,
		Sleep,
		Attack
	};

	CPlayerScript() : CScript(), mState(eState::SitDown), mAnimator(nullptr) {}
	~CPlayerScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	void SitDown();
	void Move();

private:
	eState mState;
	CAnimator* mAnimator;
};

