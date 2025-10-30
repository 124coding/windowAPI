#include "CPlayerScript.h"

#include "GameObject.h"
#include "CPlayer.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CSpriteRenderer.h"

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
	default:
		break;
	}

	if (!mCanCollideEnemy) {
		mGracePeriod -= tDeltaTime;
		if (mGracePeriod <= 0) {
			mCanCollideEnemy = true;
			mGracePeriod = 1.0f;
		}
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
		mAnimator->PlayAnimation(L"Idle", false);
	}
}

void CPlayerScript::Translate(CTransform* tr)
{
	SVector2D currentVelocity = SVector2D();

	if (mInputMgr->GetKeyPressed("DoMoveLt")) {
		currentVelocity.mX += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt")) {
		currentVelocity.mX += 1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt")) {
		currentVelocity.mY += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt")) {
		currentVelocity.mY += 1.0f;
	}

	if (currentVelocity.Length() > 0.0f) {
		currentVelocity = currentVelocity.Normalize();
	}

	tr->SetVelocity(currentVelocity * GetSpeed());

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}
}

//void CPlayerScript::GiveWater()
//{
//	if (mAnimator->IsCompleteAnimation()) {
//		mState = eState::Idle;
//		mAnimator->PlayAnimation(L"Idle", false);
//	}
//}

void CPlayerScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {

}

void CPlayerScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

}

void CPlayerScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}

void CPlayerScript::IncreaseHP(int tIncreaseAmount) {
	mHP = mHP + tIncreaseAmount;

	if (mHP > mMaxHP) {
		mHP = mMaxHP;
	}
}

void CPlayerScript::DecreaseHP(int tDecreaseAmount) {
	mHP = mHP - tDecreaseAmount;

	if (mHP < 0) {
		mHP = 0;
	}
}

void CPlayerScript::IncreaseMaxHP(int tIncreaseAmount)
{
	mMaxHP = mMaxHP + tIncreaseAmount;
}

void CPlayerScript::DecreaseMaxHP(int tDecreaseAmount)
{
	mMaxHP = mMaxHP - tDecreaseAmount;

	if (mMaxHP < 0) {
		mMaxHP = 0;
	}
}

