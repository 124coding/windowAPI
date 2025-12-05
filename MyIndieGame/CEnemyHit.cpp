#include "CEnemyHit.h"

#include "CAnimator.h"

void CEnemyHit::OnCreate()
{
	CAnimatedEffect::OnCreate();

	mLifeTime = 1.0f;

	CAnimator* anim = this->GetComponent<CAnimator>();
	anim->CreateAnimationByFolder(L"EnemyHit", L"../resources/Enemy/EnemyHit", SVector2D(), mLifeTime / 3);

	SetState(false);
}

void CEnemyHit::OnDestroy()
{
	CAnimatedEffect::OnDestroy();
}

void CEnemyHit::OnUpdate(float tDeltaTime)
{
	CAnimatedEffect::OnUpdate(tDeltaTime);
}

void CEnemyHit::OnLateUpdate(float tDeltaTime)
{
	CAnimatedEffect::OnLateUpdate(tDeltaTime);
}

void CEnemyHit::Render(HDC tHDC)
{
	CAnimatedEffect::Render(tHDC);
	this->GetComponent<CAnimator>()->PlayAnimation(L"EnemyHit", false);

}

void CEnemyHit::Reset(SVector2D tPos)
{
	CAnimatedEffect::Reset(tPos);
	mLifeTime = 1.0f;
}
