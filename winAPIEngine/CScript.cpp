#include "CScript.h"

#include "GameObject.h"

#include "CTransform.h"

void CScript::OnCreate()
{
}

void CScript::OnDestroy()
{
}

void CScript::OnUpdate(float tDeltaTime)
{
}

void CScript::OnLateUpdate(float tDeltaTime)
{
}

void CScript::Render(HDC tHDC)
{
}

void CScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {

}

void CScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

}

void CScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}

void CScript::Bounce() {
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	float bobbingFactor = fabs(sin(mTotalTime * mBobbingSpeed));

	float scaleY = 1.0f - (bobbingFactor * mSquashMagnitude);

	float scaleX = 1.0f + (bobbingFactor * mSquashMagnitude);

	tr->SetScale(SVector2D(scaleX, scaleY));
}
