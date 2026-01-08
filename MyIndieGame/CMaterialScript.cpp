#include "CMaterialScript.h"

#include "Object.h"

#include "CPlayScene.h"

#include "CResourceMgr.h"

#include "GameObject.h"
#include "CPlayer.h"

#include "CSpriteRenderer.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CCircleCollider2D.h"

void CMaterialScript::OnCreate()
{
	// 동적으로 렌더러와 충돌체 컴포넌트 추가
	CSpriteRenderer* sr = GetOwner()->AddComponent<CSpriteRenderer>();
    CCircleCollider2D* cl = GetOwner()->AddComponent<CCircleCollider2D>();
    cl->SetSize(SVector2D(0.5f, 0.5f));

	// [Visual Variety]
	// 11종의 자원 이미지를 랜덤으로 선택하여 시각적 다양성 부여
	int rand = std::rand() % 11;

	sr->SetTexture(CResourceMgr::Find<CTexture>(L"Material_" + std::to_wstring(rand)));

	// 텍스트의 중심을 앵커 포인트로 설정 (회전이나 위치 계산의 기준점)
    GetOwner()->SetAnchorPoint(sr->GetTexture()->GetWidth() / 2.0f, sr->GetTexture()->GetHeight() / 2.0f);
}

void CMaterialScript::OnDestroy()
{
}

void CMaterialScript::OnUpdate(float tDeltaTime)
{
	// [FSM State Machine]
	// Idle: 제자리에 멈춰 있음 (기본 상태)
	// Follow: 플레이어의 자석 범위에 들어오면(PlayerScript에서 설정), 플레이어를 추적함
	switch (mState) {
	case eState::Idle:
		break;
	case eState::Follow:
		CarriedPlayer();		// 추적 로직 실행
		break;
	default:
		break;
	}
}

void CMaterialScript::OnLateUpdate(float tDeltaTime)
{
}

void CMaterialScript::Render(HDC tHDC)
{
}

// 자원은 충돌 시 로직을 직접 처리하지 않고, 플레이어(CPlayerScript) 측에서 처리함 (Observer or Event pattern)
void CMaterialScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
}

void CMaterialScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CMaterialScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}

// 플레이어를 향해 가속 이동하는 벡터 연산
void CMaterialScript::CarriedPlayer()
{
    CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
    CTransform* targetTr = CPlayScene::GetPlayer()->GetComponent<CTransform>(eComponentType::Transform);
    CCollider* plCollider = CPlayScene::GetPlayer()->GetComponent<CCollider>(eComponentType::Collider);

    SVector2D currentVelocity = SVector2D();

	// 1. 방향 벡터 계산 (Destination - Origin)
	// 플레이어의 중심 위치(Pos + Offset)를 목표로 설정
    currentVelocity = targetTr->GetPos() + plCollider->GetOffset() - tr->GetPos();

	// 2. 벡터 정규화 (Normalization)
	// 거리에 상관없이 일정한 방향(Direction)만 추출
    if (currentVelocity.Length() > 0.0f) {
        currentVelocity = currentVelocity.Normalize();
    }

	// 3. 속도 적용 (Scalar Multiplication)
	// 800.0f의 빠른 속도로 플레이어에게 날아가도록 설정 (자석 느낌)
    tr->SetVelocity(currentVelocity * 800.0f);
}
