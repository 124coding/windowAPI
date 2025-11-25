#include "CBulletScript.h"

#include "GameObject.h"
#include "Object.h"

#include "CTilemapRenderer.h"

void CBulletScript::OnCreate()
{
}

void CBulletScript::OnDestroy()
{
}

void CBulletScript::OnUpdate(float tDeltaTime)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	if (tr->GetPos().mX < -CTilemapRenderer::TileSize.mX || tr->GetPos().mX > mapWidth + CTilemapRenderer::TileSize.mX
		|| tr->GetPos().mY < -CTilemapRenderer::TileSize.mY * 2 || tr->GetPos().mY > mapHeight + CTilemapRenderer::TileSize.mY) {
		ObjDestroy(GetOwner());
	}
}

void CBulletScript::OnLateUpdate(float tDeltaTime)
{
}

void CBulletScript::Render(HDC tHDC)
{
}

void CBulletScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	ObjDestroy(GetOwner());
}

void CBulletScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CBulletScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}
