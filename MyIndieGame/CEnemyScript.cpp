#include "CEnemyScript.h"

#include "CPlayer.h"
#include "CEnemy.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CSpriteRenderer.h"

#include "Object.h"

void CEnemyScript::OnCreate()
{
	SetBobbingSpeed(1.0f);
	SetSquashMagnitude(0.2f);
}

void CEnemyScript::OnDestroy()
{
}

void CEnemyScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

	CTransform* plTr = mTarget->GetComponent<CTransform>();
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	SetDistanceToPlayer((plTr->GetPos() - tr->GetPos()).Length());

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}

	switch (mState) {
	case eState::Walk:
		Bounce();
		break;
	case eState::Attack:
		break;
	default:
		break;
	}
}

void CEnemyScript::OnLateUpdate(float tDeltaTime)
{
}

void CEnemyScript::Render(HDC tHDC)
{
}

void CEnemyScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	if (tOther->GetOwner()->GetLayerType() == eLayerType::Player) {
		CPlayer* player = dynamic_cast<CPlayer*>(tOther->GetOwner());
		ButtDamageToPlayer(player);
	}
}

void CEnemyScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{

	if (tOther->GetOwner()->GetLayerType() == eLayerType::Player) {
		CPlayer* player = dynamic_cast<CPlayer*>(tOther->GetOwner());
		ButtDamageToPlayer(player);
	}
}

void CEnemyScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{

}

void CEnemyScript::DecreaseHP(int tDecreaseAmount) {
	mHP = mHP - tDecreaseAmount;

	if (mHP < 0) {
		mHP = 0;
	}
}

void CEnemyScript::ButtDamageToPlayer(GameObject* tPlayer)
{
	if (tPlayer->GetComponent<CPlayerScript>()->GetCanCollideEnemy()) {
		tPlayer->GetComponent<CPlayerScript>()->DecreaseHP(mButtDamage);
		tPlayer->GetComponent<CPlayerScript>()->SetCanCollideEnemy(false);
	}
}