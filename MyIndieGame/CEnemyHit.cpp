#include "CEnemyHit.h"

#include "CAnimator.h"

void CEnemyHit::OnCreate()
{
	CAnimatedEffect::OnCreate();

	mLifeTime = 0.1f;

	CAnimator* anim = this->GetComponent<CAnimator>();
	anim->CreateAnimationByFolder(L"EnemyHit", L"../resources/Enemy/EnemyHit", SVector2D(), mLifeTime / 3);

	SetAnchorPoint(128.0f, 128.0f);

	SetState(false);
}

void CEnemyHit::OnDestroy()
{
	CAnimatedEffect::OnDestroy();
}

void CEnemyHit::OnUpdate(float tDeltaTime)
{
	CAnimatedEffect::OnUpdate(tDeltaTime);
	if (mOwnerObj != nullptr && mOwnerObj->IsActive()) {
		CTransform* tr = this->GetComponent<CTransform>();
		tr->SetPos(mOwnerObj->GetComponent<CTransform>()->GetPos());
	}
}

void CEnemyHit::OnLateUpdate(float tDeltaTime)
{
	CAnimatedEffect::OnLateUpdate(tDeltaTime);
}

void CEnemyHit::Render(HDC tHDC)
{
	CAnimatedEffect::Render(tHDC);

}

void CEnemyHit::Reset(SVector2D tPos)
{
	CAnimatedEffect::Reset(tPos);

	mLifeTime = 0.1f;

	this->GetComponent<CAnimator>()->PlayAnimation(L"EnemyHit", false);
}
