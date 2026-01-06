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
	CSpriteRenderer* sr = GetOwner()->AddComponent<CSpriteRenderer>();
    CCircleCollider2D* cl = GetOwner()->AddComponent<CCircleCollider2D>();
    cl->SetSize(SVector2D(0.5f, 0.5f));

	int rand = std::rand() % 11;

	sr->SetTexture(CResourceMgr::Find<CTexture>(L"Material_" + std::to_wstring(rand)));
    GetOwner()->SetAnchorPoint(sr->GetTexture()->GetWidth() / 2.0f, sr->GetTexture()->GetHeight() / 2.0f);
}

void CMaterialScript::OnDestroy()
{
}

void CMaterialScript::OnUpdate(float tDeltaTime)
{
	switch (mState) {
	case eState::Idle:
		break;
	case eState::Follow:
		CarriedPlayer();
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

void CMaterialScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
}

void CMaterialScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CMaterialScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}

void CMaterialScript::CarriedPlayer()
{
    CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
    CTransform* targetTr = CPlayScene::GetPlayer()->GetComponent<CTransform>(eComponentType::Transform);
    CCollider* plCollider = CPlayScene::GetPlayer()->GetComponent<CCollider>(eComponentType::Collider);

    SVector2D currentVelocity = SVector2D();

    currentVelocity = targetTr->GetPos() + plCollider->GetOffset() - tr->GetPos();

    if (currentVelocity.Length() > 0.0f) {
        currentVelocity = currentVelocity.Normalize();
    }

    tr->SetVelocity(currentVelocity * 800.0f);
}
