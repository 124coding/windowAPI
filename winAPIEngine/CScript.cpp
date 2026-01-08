#include "CScript.h"

#include "GameObject.h"

#include "CTransform.h"

void CScript::OnCreate()
{
}

void CScript::OnDestroy()
{
}

void CScript::OnUpdate(float tDeltaTime)
{
}

void CScript::OnLateUpdate(float tDeltaTime)
{
}

void CScript::Render(HDC tHDC)
{
}

void CScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {

}

void CScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

}

void CScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}

void CScript::Bounce() {
	// 오브젝트의 Transform 컴포넌트 가져오기
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 1. 사인파(Sin Wave)를 이용한 주기적인 움직임 계산
	// sin 값은 -1 ~ 1 사이를 오가는데, fabs(절댓값)를 취해 0 ~ 1 사이로 만듦
	// mBobbingSpeed: 움직이는 속도 조절
	float bobbingFactor = fabs(sin(mTotalTime * mBobbingSpeed));

	// 2. 스쿼시 앤 스트레치 (Squash and Stretch)
	// Y축(높이)이 줄어들면(Squash) -> 1.0f - factor
	float scaleY = 1.0f - (bobbingFactor * mSquashMagnitude);

	// X축(너비)은 늘어나야 함(Stretch) -> 1.0f + factor
	// 즉, 찌그러질 때 옆으로 퍼지는 효과를 수식으로 표현한 것
	float scaleX = 1.0f + (bobbingFactor * mSquashMagnitude);

	// 계산된 스케일 적용 (젤리처럼 통통 튀는 느낌을 줌)
	tr->SetScale(SVector2D(scaleX, scaleY));
}
