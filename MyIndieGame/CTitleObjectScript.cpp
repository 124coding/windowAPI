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

	mRadian += tDeltaTime;

	mRegularVelocity = SVector2D(mSwing * cosf(mRadian), 0.0f);

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	tr->SetVelocity(mRegularVelocity * 30.0f);
}

void CTitleObjectScript::OnLateUpdate(float tDeltaTime)
{
}

void CTitleObjectScript::Render(HDC tHDC)
{
}
