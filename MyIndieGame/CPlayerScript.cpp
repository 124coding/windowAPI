#include "CPlayerScript.h"
#include "CInputMgr.h"
#include "CTransform.h"
#include "GameObject.h"

void CPlayerScript::OnCreate()
{
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::OnUpdate(float tDeltaTime)
{
	SVector2D CurrentVelocity;

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveLt")) {
		CurrentVelocity.mX += -1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveRt")) {
		CurrentVelocity.mX += 1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveFt")) {
		CurrentVelocity.mY += -1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveBt")) {
		CurrentVelocity.mY += 1.0f;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetVelocity(CurrentVelocity * 300.0f);
}

void CPlayerScript::OnLateUpdate(float tDeltaTime)
{
}

void CPlayerScript::Render(HDC hDC)
{
}
