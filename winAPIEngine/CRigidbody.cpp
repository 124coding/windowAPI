#include "CRigidbody.h"

#include "GameObject.h"
#include "CTransform.h"

void CRigidbody::OnCreate()
{
}

void CRigidbody::OnDestroy()
{
}

void CRigidbody::OnUpdate(float tDeltaTime)
{
	// f(힘) = m(질량) * a(가속도)
	// a = f / m;

	mAccelation = mForce / mMass;

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	tr->SetVelocity(tr->GetVelocity() + mAccelation * tDeltaTime);

	if (mbGround) {
		// 땅에 있을때
		SVector2D gravity = mGravity;
		gravity = gravity.Normalize();

		float dot = tr->GetVelocity().Dot(gravity);
		tr->SetVelocity(tr->GetVelocity() - gravity * dot);
	}
	else {
		// 공중에 있을때
		tr->SetVelocity(tr->GetVelocity() + mGravity * tDeltaTime);
	}

	// 최대 속도 제한
	SVector2D gravity = mGravity;
	gravity = gravity.Normalize();

	float dot = gravity.Dot(tr->GetVelocity());
	gravity = gravity * dot;

	SVector2D sideVelocity = tr->GetVelocity() - gravity;
	
	if (mLimitedVelocity.mY < gravity.Length()) {
		gravity = gravity.Normalize();
		gravity = gravity * mLimitedVelocity.mY;
	}

	if (mLimitedVelocity.mX < sideVelocity.Length()) {
		sideVelocity = sideVelocity.Normalize();
		sideVelocity = sideVelocity * mLimitedVelocity.mX;
	}

	tr->SetVelocity(gravity + sideVelocity);

	if (tr->GetVelocity().Length() > 0.0f) {
		SVector2D friction = -(tr->GetVelocity());
		friction = friction.Normalize() * mFriction * mMass * tDeltaTime;

		// 마찰력으로 인한 속도 감소량이 현재 속도보다 큰 경우
		float a = tr->GetVelocity().Length();
		float b = friction.Length();
		if (tr->GetVelocity().Length() <= friction.Length()) {
			tr->SetVelocity(SVector2D());
		}
		else {
			tr->SetVelocity(tr->GetVelocity() + friction);
		}
	}

	mForce = SVector2D();
}

void CRigidbody::OnLateUpdate(float tDeltaTime)
{
}

void CRigidbody::Render(HDC tHDC)
{
}
