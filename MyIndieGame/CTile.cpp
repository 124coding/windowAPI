#include "CTile.h"

#include "CTilemapRenderer.h"

void CTile::OnCreate()
{
	GameObject::OnCreate();
}

void CTile::OnDestroy() {
	GameObject::OnDestroy();
}

void CTile::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CTile::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CTile::Render(HDC tHDC)
{
	GameObject::Render(tHDC);
}

void CTile::SetPosition(SVector2D tPos)
{
	CTransform* tr = GetComponent<CTransform>();

	tPos.mX = tPos.mX * CTilemapRenderer::TileSize.mX;
	tPos.mY = tPos.mY * CTilemapRenderer::TileSize.mY;

	tr->SetPos(tPos);
}
