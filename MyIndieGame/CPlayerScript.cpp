#include "CPlayerScript.h"
#include "CInputMgr.h"
#include "CTransform.h"
#include "GameObject.h"
#include "CAnimator.h"

void CPlayerScript::OnCreate()
{
	
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::OnUpdate(float tDeltaTime)
{

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}
	switch (mState) {
	case eState::Idle:
		Idle();
		break;
	case eState::Walk:
		Move();
		break;
	case eState::GiveWater:
		GiveWater();
		break;
	case eState::Sleep:
		break;
	case eState::Attack:
		break;
	default:
		break;
	}

}

void CPlayerScript::OnLateUpdate(float tDeltaTime)
{
}

void CPlayerScript::Render(HDC tHDC)
{
}

void CPlayerScript::Idle()
{
	if (mInputMgr->GetKeyPressed("DoMoveLt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"LeftWalk");
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"RightWalk");
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"DownWalk");
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"UpWalk");
	}

	if (mInputMgr->GetKeyDown("MouseLeftClick")) {
		SVector2D mousePos = CInputMgr::GetMousePosition();

		mState = eState::GiveWater;
		mAnimator->PlayAnimation(L"FrontGiveWater", false);
	}
}

void CPlayerScript::Move()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	Translate(tr);

	if ((mInputMgr->GetKeyUp("DoMoveLt") || mInputMgr->GetKeyNone("DoMoveLt")) &&
		(mInputMgr->GetKeyUp("DoMoveRt") || mInputMgr->GetKeyNone("DoMoveRt")) &&
		(mInputMgr->GetKeyUp("DoMoveFt") || mInputMgr->GetKeyNone("DoMoveFt")) &&
		(mInputMgr->GetKeyUp("DoMoveBt") || mInputMgr->GetKeyNone("DoMoveBt"))) {
		mState = eState::Idle;
		tr->SetVelocity(SVector2D(0.0f, 0.0f));
		mAnimator->PlayAnimation(L"SitDown", false);
	}
}

void CPlayerScript::Translate(CTransform* tr)
{
	SVector2D CurrentVelocity;

	if (mInputMgr->GetKeyPressed("DoMoveLt")) {
		CurrentVelocity.mX += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt")) {
		CurrentVelocity.mX += 1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt")) {
		CurrentVelocity.mY += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt")) {
		CurrentVelocity.mY += 1.0f;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	tr->SetVelocity(CurrentVelocity * 100.0f);
}

void CPlayerScript::GiveWater()
{
	if (mAnimator->IsCompleteAnimation()) {
		mState = eState::Idle;
		mAnimator->PlayAnimation(L"Idle", false);
	}
}

