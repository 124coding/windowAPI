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
	// 1. 가속도 계산 (뉴턴의 제2법칙: F = ma -> a = F / m)
	// 가벼운 물체일수록 같은 힘을 받아도 더 빠르게 가속됨
	mAccelation = mForce / mMass;

	// 2. 속도 갱신 (v = v0 + at)
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	tr->SetVelocity(tr->GetVelocity() + mAccelation * tDeltaTime);

	// 3. 중력 및 지면 처리
	if (mbGround) {
		// 땅에 있을때
		SVector2D gravity = mGravity;
		gravity = gravity.Normalize();

		// 내 속도 중 중력 방향 성분(내적)을 구함
		float dot = tr->GetVelocity().Dot(gravity);

		// 중력 방향 성분만 뺌 (수평 속도는 유지하고 수직 속도만 0으로 만듦)
		tr->SetVelocity(tr->GetVelocity() - gravity * dot);
	}
	else {
		// [공중에 있을 때]
		// 중력 가속도 적용 (계속 아래로 가속)
		tr->SetVelocity(tr->GetVelocity() + mGravity * tDeltaTime);
	}

	// 4. 최대 속도 제한 (Terminal Velocity)
	// 낙하 속도와 이동 속도를 분리해서 제한해야 함 (안 그러면 빨리 떨어질 때 좌우 이동이 안 될 수 있음)
	SVector2D gravity = mGravity;
	gravity = gravity.Normalize();

	// 현재 속도를 '중력 방향(수직)'과 '그 외 방향(수평)'으로 분리
	float dot = gravity.Dot(tr->GetVelocity());

	gravity = gravity * dot;	// 수직 속도 벡터
	SVector2D sideVelocity = tr->GetVelocity() - gravity;	// 수평 속도 벡터
	
	// 수직 속도 제한 (낙하 속도 제한)
	if (mLimitedVelocity.mY < gravity.Length()) {
		gravity = gravity.Normalize();
		gravity = gravity * mLimitedVelocity.mY;
	}

	// 수평 속도 제한 (이동 속도 제한)
	if (mLimitedVelocity.mX < sideVelocity.Length()) {
		sideVelocity = sideVelocity.Normalize();
		sideVelocity = sideVelocity * mLimitedVelocity.mX;
	}

	// 분리했던 속도를 다시 합침
	tr->SetVelocity(gravity + sideVelocity);

	// 5. 마찰력 적용 (Friction)
	// 움직이고 있을 때만 적용
	if (tr->GetVelocity().Length() > 0.0f) {

		// 마찰력은 이동 방향의 반대 방향으로 작용
		SVector2D friction = -(tr->GetVelocity());
		friction = friction.Normalize() * mFriction * mMass * tDeltaTime;

		// 마찰력으로 인한 속도 감소량이 현재 속도보다 큰 경우 -> 즉시 정지
		float a = tr->GetVelocity().Length();
		float b = friction.Length();
		if (tr->GetVelocity().Length() <= friction.Length()) {
			tr->SetVelocity(SVector2D());
		}
		else {
			// 속도가 충분히 빠르면 마찰력만큼 감속
			tr->SetVelocity(tr->GetVelocity() + friction);
		}
	}

	// 누적된 힘 초기화 (힘은 매 프레임 다시 가해져야 함 - Impulse 방식이 아닌 Force 방식)
	mForce = SVector2D();
}

void CRigidbody::OnLateUpdate(float tDeltaTime)
{
}

void CRigidbody::Render(HDC tHDC)
{
}
