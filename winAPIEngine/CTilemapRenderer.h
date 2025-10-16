#pragma once

#include "CComponent.h"
#include "CTexture.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <windows.h>


class CTilemapRenderer : public CComponent
{
public:
	CTilemapRenderer()
		: CComponent(eComponentType::SpriteRenderer), mTexture(nullptr), mIndex(SVector2D()), mTileSize(16.0f, 16.0f) {
	}
	~CTilemapRenderer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetTexture(CTexture* tTexture) {
		this->mTexture = tTexture;
	}

private:
	SVector2D mTileSize;

	CTexture* mTexture;
	SVector2D mIndex;
};
