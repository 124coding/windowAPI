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

	if (mInputMgr->GetKeyPressed("DoMoveLt") && tr->GetPos().mX > windowWidth / 2) {
		CurrentVelocity.mX += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt") && tr->GetPos().mX < mapWidth - windowWidth / 2) {
		CurrentVelocity.mX += 1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt") && tr->GetPos().mY > windowHeight / 2) {
		CurrentVelocity.mY += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt") && tr->GetPos().mY < mapHeight - windowHeight / 2) {
		CurrentVelocity.mY += 1.0f;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	if (tr->GetPos().mX < windowWidth / 2) {
		tr->SetPos(SVector2D(windowWidth / 2, tr->GetPos().mY));
	}
	if (tr->GetPos().mX > mapWidth - windowWidth / 2) {
		tr->SetPos(SVector2D(mapWidth - windowWidth / 2, tr->GetPos().mY));
	}
	if (tr->GetPos().mY < windowHeight / 2) {
		tr->SetPos(SVector2D(tr->GetPos().mX, windowHeight / 2));
	}
	if (tr->GetPos().mY > mapHeight - windowHeight / 2) {
		tr->SetPos(SVector2D(tr->GetPos().mX, mapHeight - windowHeight / 2));
	}

	tr->SetVelocity(CurrentVelocity * 500.0f);
}

void CCameraScript::OnLateUpdate(float tDeltaTime)
{
}

void CCameraScript::Render(HDC tHDC)
{
}
