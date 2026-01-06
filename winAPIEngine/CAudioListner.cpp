#include "CAudioListner.h"

#include "GameObject.h"

#include "CFMOD.h"

#include "CTransform.h"

void CAudioListner::OnCreate()
{
}

void CAudioListner::OnDestroy()
{
}

void CAudioListner::OnUpdate(float tDeltaTime)
{
}

void CAudioListner::OnLateUpdate(float tDeltaTime)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	SVector2D pos = tr->GetPos();
	
	CFMOD::Set2DListenerAttributes(&pos);
}

void CAudioListner::Render(HDC tHDC)
{
}
