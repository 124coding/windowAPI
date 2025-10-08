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
	case eState::SitDown:
		SitDown();
		break;
	case eState::Walk:
		Move();
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

void CPlayerScript::SitDown()
{
	CInputMgr* inputMgr = CInputMgr::GetInst();

	if (inputMgr->GetKeyPressed("DoMoveLt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"LeftWalk");
	}

	if (inputMgr->GetKeyPressed("DoMoveRt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"RightWalk");
	}

	if (inputMgr->GetKeyPressed("DoMoveFt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"DownWalk");
	}

	if (inputMgr->GetKeyPressed("DoMoveBt")) {
		mState = eState::Walk;
		mAnimator->PlayAnimation(L"UpWalk");
	}
}

void CPlayerScript::Move()
{
	SVector2D CurrentVelocity;
	CInputMgr* inputMgr = CInputMgr::GetInst();

	if (inputMgr->GetKeyPressed("DoMoveLt")) {
		CurrentVelocity.mX += -1.0f;
	}

	if (inputMgr->GetKeyPressed("DoMoveRt")) {
		CurrentVelocity.mX += 1.0f;
	}

	if (inputMgr->GetKeyPressed("DoMoveFt")) {
		CurrentVelocity.mY += -1.0f;
	}

	if (inputMgr->GetKeyPressed("DoMoveBt")) {
		CurrentVelocity.mY += 1.0f;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetVelocity(CurrentVelocity * 300.0f);

	if ((inputMgr->GetKeyUp("DoMoveLt") || inputMgr->GetKeyNone("DoMoveLt")) &&
		(inputMgr->GetKeyUp("DoMoveRt") || inputMgr->GetKeyNone("DoMoveRt")) &&
		(inputMgr->GetKeyUp("DoMoveFt") || inputMgr->GetKeyNone("DoMoveFt")) &&
		(inputMgr->GetKeyUp("DoMoveBt") || inputMgr->GetKeyNone("DoMoveBt"))) {
		mState = eState::SitDown;
		mAnimator->PlayAnimation(L"SitDown", false);
	}
}
