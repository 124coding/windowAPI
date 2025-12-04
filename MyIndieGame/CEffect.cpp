#include "CEffect.h"

CEffect::CEffect(const CEffect& tEffect)
{
}

CEffect& CEffect::operator=(const CEffect& tEffect)
{
	if (this == &tEffect)
	{
		return *this;
	}

	GameObject::operator=(tEffect);
}

void CEffect::OnCreate()
{
	GameObject::OnCreate();
}

void CEffect::OnDestroy()
{
	GameObject::OnDestroy();
}

void CEffect::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);

	mCurTime += tDeltaTime;

	if (mCurTime >= mLifeTime) {
		mCurTime = 0.0f;
		this->SetState(false);
	}
}

void CEffect::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CEffect::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}
