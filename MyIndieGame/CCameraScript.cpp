#include "CCameraScript.h"

#include "GameObject.h"

#include "CTransform.h"

#include "CInputMgr.h"

void CCameraScript::OnCreate()
{
}

void CCameraScript::OnDestroy()
{
}

void CCameraScript::OnUpdate(float tDeltaTime)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

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

void CCameraScript::OnLateUpdate(float tDeltaTime)
{
}

void CCameraScript::Render(HDC tHDC)
{
}
