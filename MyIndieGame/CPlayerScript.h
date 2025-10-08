#pragma once

#include "CScript.h"
#include "CInputMgr.h"

class CAnimator;
class CTransform;

class CPlayerScript : public CScript
{
public:
	enum class eState {
		Idle,
		Walk,
		GiveWater,
		Sleep,
		Attack
	};

	CPlayerScript() : CScript(), mState(eState::Idle), mAnimator(nullptr) {}
	~CPlayerScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	void Idle();
	void Move();
	void Translate(CTransform* tr);
	void GiveWater();

private:
	eState mState;
	CAnimator* mAnimator;
	CInputMgr* mInputMgr = CInputMgr::GetInst();

	void (*StartEvent)();
	void (*CompleteEvent)();
	void (*EndEvent)();
};

