#include "CEnemyScript.h"

#include "CTransform.h"
#include "CAnimator.h"

#include "Object.h"

void CEnemyScript::OnCreate()
{

}

void CEnemyScript::OnDestroy()
{
}

void CEnemyScript::OnUpdate(float tDeltaTime)
{
	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}
	switch (mState) {
	case eState::Walk:
		Move(tDeltaTime);
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

void CEnemyScript::OnCollisionEnter(CCollider* tOther)
{

}

void CEnemyScript::OnCollisionStay(CCollider* tOther)
{

}

void CEnemyScript::OnCollisionExit(CCollider* tOther)
{

}