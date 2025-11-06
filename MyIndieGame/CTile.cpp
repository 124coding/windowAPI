#include "CTile.h"

#include "CTilemapRenderer.h"

CTile::CTile(const CTile& tTile) : GameObject(tTile)
{
}

CTile& CTile::operator=(const CTile& tTile) {
	if (this == &tTile)
	{
		return *this;
	}

	GameObject::operator=(tTile);
}

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

void CTile::SetIndexPosition(SVector2D tPos)
{
	CTransform* tr = GetComponent<CTransform>();

	tPos.mX = tPos.mX * CTilemapRenderer::TileSize.mX;
	tPos.mY = tPos.mY * CTilemapRenderer::TileSize.mY;

	tr->SetPos(tPos);
}
