#include "CEnemyScript.h"

#include "CPlayer.h"
#include "CEnemy.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"

#include "Object.h"

void CEnemyScript::OnCreate()
{

}

void CEnemyScript::OnDestroy()
{
}

void CEnemyScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

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

void CEnemyScript::ButtDamageToPlayer(CPlayer* tPlayer)
{
	if (tPlayer->GetCanCollideEnemy()) {
		tPlayer->DecreaseHP(dynamic_cast<CEnemy*>(this->GetOwner())->GetButtDamage());
		tPlayer->SetCanCollideEnemy(false);
	}
}

void CEnemyScript::Bounce() {
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	float bobbingFactor = fabs(sin(mTotalTime * mBobbingSpeed));

	float scaleY = 1.0f - (bobbingFactor * mSquashMagnitude);

	float scaleX = 1.0f + (bobbingFactor * mSquashMagnitude);

	tr->SetScale(SVector2D(scaleX, scaleY));
}
