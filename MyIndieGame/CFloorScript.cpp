#include "CFloorScript.h"

#include "GameObject.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRigidbody.h"

void CFloorScript::OnCreate()
{
	CScript::OnCreate();
}

void CFloorScript::OnDestroy()
{
	CScript::OnDestroy();
}

void CFloorScript::OnUpdate(float tDeltaTime)
{
	CScript::OnUpdate(tDeltaTime);
}

void CFloorScript::OnLateUpdate(float tDeltaTime)
{
	CScript::OnLateUpdate(tDeltaTime);
}

void CFloorScript::Render(HDC tHDC)
{
	CScript::Render(tHDC);
}

void CFloorScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {
	CRigidbody* plRb = tOther->GetOwner()->GetComponent<CRigidbody>(eComponentType::Rigidbody);
	CTransform* plTr = tOther->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	CCollider* plCol = tOther;

	CRigidbody* floorRb = this->GetOwner()->GetComponent<CRigidbody>(eComponentType::Rigidbody);
	CTransform* floorTr = this->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	CCollider* floorCol = this->GetOwner()->GetComponent<CCollider>(eComponentType::Collider);


	float len = fabs(plTr->GetPos().mY - floorTr->GetPos().mY);
	float scale = fabs(plCol->GetSize().mY * 100.0f / 2.0f - floorCol->GetSize().mY * 100.0f / 2.0f);

	if (len < scale) {
		SVector2D plPos = plTr->GetPos();
		plPos.mY -= scale - len - 1.0f;

		plTr->SetPos(plPos);
	}

    plRb->SetGround(true);
}

void CFloorScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

}

void CFloorScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}