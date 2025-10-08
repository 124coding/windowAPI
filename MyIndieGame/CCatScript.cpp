#include "CCatScript.h"
#include "CTransform.h"
#include "GameObject.h"
#include "CAnimator.h"

void CCatScript::OnCreate()
{

}

void CCatScript::OnDestroy()
{
}

void CCatScript::OnUpdate(float tDeltaTime)
{

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}
	switch (mState) {
	case eState::SitDown:
		SitDown(tDeltaTime);
		break;
	case eState::Walk:
		Move(tDeltaTime);
		break;
	case eState::Sleep:
		break;
	case eState::LayDown:
		break;
	case eState::Attack:
		break;
	default:
		break;
	}
}

void CCatScript::OnLateUpdate(float tDeltaTime)
{
}

void CCatScript::Render(HDC tHDC)
{
}

void CCatScript::SitDown(float tDeltaTime)
{
	mTime += tDeltaTime;
	if (mTime > 2.0f) {
		mState = eState::Walk;
		int direction = rand() % 4;
		mDirection = (eDirection)direction;
		PlayWalkAnimationByDirection(mDirection);
		mTime = 0.0f;
	}
}

void CCatScript::Move(float tDeltaTime)
{
	mTime += tDeltaTime;

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Translate(tr);

	if (mTime > 2.0f) {
		int isLayDown = rand() % 2;
		if (isLayDown) {
			mState = eState::LayDown;
			mAnimator->PlayAnimation(L"LayDown", false);
			tr->SetVelocity(SVector2D(0.0f, 0.0f));
		}
		else {
			mState = eState::SitDown;
			mAnimator->PlayAnimation(L"SitDown", false);
			tr->SetVelocity(SVector2D(0.0f, 0.0f));
			mTime = 0.0f;
		}
	}
}

void CCatScript::LayDown()
{
}

void CCatScript::PlayWalkAnimationByDirection(eDirection tDirection)
{
	switch (tDirection)
	{
	case eDirection::Left:
		mAnimator->PlayAnimation(L"LeftWalk");
		break;
	case eDirection::Right:
		mAnimator->PlayAnimation(L"RightWalk");
		break;
	case eDirection::Down:
		mAnimator->PlayAnimation(L"DownWalk");
		break;
	case eDirection::Up:
		mAnimator->PlayAnimation(L"UpWalk");
		break;
	default:
		break;
	}
}

void CCatScript::Translate(CTransform* tr)
{
	SVector2D CurrentVelocity;

	switch (mDirection)
	{
	case eDirection::Left:
		CurrentVelocity.mX += -1.0f;
		break;
	case eDirection::Right:
		CurrentVelocity.mX += 1.0f;
		break;
	case eDirection::Down:
		CurrentVelocity.mY += -1.0f;
		break;
	case eDirection::Up:
		CurrentVelocity.mY += 1.0f;
		break;
	default:
		break;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	tr->SetVelocity(CurrentVelocity * 100.0f);
}
