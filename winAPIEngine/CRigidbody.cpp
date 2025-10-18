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
	// f(��) = m(����) * a(���ӵ�)
	// a = f / m;

	mAccelation = mForce / mMass;

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	tr->SetVelocity(tr->GetVelocity() + mAccelation * tDeltaTime);

	if (mbGround) {
		// ���� ������
		SVector2D gravity = mGravity;
		gravity = gravity.Normalize();

		float dot = tr->GetVelocity().Dot(gravity);
		tr->SetVelocity(tr->GetVelocity() - gravity * dot);
	}
	else {
		// ���߿� ������
		tr->SetVelocity(tr->GetVelocity() + mGravity * tDeltaTime);
	}

	// �ִ� �ӵ� ����
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

		// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� ū ���
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
