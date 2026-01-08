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
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	SVector2D CurrentVelocity;

	// [Input Processing]
	// 입력에 따라 이동 방향 벡터를 설정
	// 동시에 1차적인 경계 검사를 수행하여, 이미 맵 끝에 도달했다면 해당 방향으로의 가속을 차단함
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

	// [Vector Normalization]
	// 대각선 이동 시 속도가 루트 2배(약 1.414배)로 빨라지는 것을 방지하기 위해 정규화 수행
	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}

	// [Hard Clamping]
	// 물리적인 이동이나 프레임 드랍 등으로 인해 카메라가 경계를 살짝 넘어갔을 경우,
	// 강제로 위치를 유효 범위 내로 고정시키는 보정 로직
	// 카메라의 중심점(Pos)이 화면 절반 크기(windowWidth / 2)보다 가장자리로 가지 않도록 제한
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
