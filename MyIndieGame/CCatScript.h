#pragma once
#include "CScript.h"
#include "CInputMgr.h"

class CAnimator;
class CTransform;

class CCatScript : public CScript
{
public:
	enum class eState {
		SitDown,
		Walk,
		Sleep,
		LayDown,
		Attack
	};

	enum class eDirection {
		Left,
		Right,
		Down,
		Up,
		End
	};

	CCatScript() : CScript(), mState(eState::SitDown), mAnimator(nullptr), mDirection(eDirection::End), mTime(0.0f), mDeathTime(0.0f), mRadian(0.0f) {}
	~CCatScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(CCollider* tOther) override;
	void OnCollisionStay(CCollider* tOther) override;
	void OnCollisionExit(CCollider* tOther) override;

private:
	void SitDown(float tDeltaTime);
	void Move(float tDeltaTime);
	void LayDown();
	void PlayWalkAnimationByDirection(eDirection tDirection);
	void Translate(CTransform* tr);

private:
	eState mState;
	eDirection mDirection;

	CAnimator* mAnimator;

	CInputMgr* mInputMgr = CInputMgr::GetInst();

	float mTime;
	float mDeathTime;
	float mRadian;
};