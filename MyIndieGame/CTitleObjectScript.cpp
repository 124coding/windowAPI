#include "CTitleObjectScript.h"

#include "GameObject.h"

#include "CTransform.h"

void CTitleObjectScript::OnCreate()
{
}

void CTitleObjectScript::OnDestroy()
{
}

void CTitleObjectScript::OnUpdate(float tDeltaTime)
{

	mTotalTime += tDeltaTime;

	mRegularVelocity = SVector2D(mSpeed * sin(mTotalTime), 0.0f);

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetVelocity(mRegularVelocity * 30.0f);
}

void CTitleObjectScript::OnLateUpdate(float tDeltaTime)
{
}

void CTitleObjectScript::Render(HDC tHDC)
{
}
