#pragma once

#include "CComponent.h"
#include "CTexture.h"
#include "GameObject.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <windows.h>


class CTilemapRenderer : public CComponent
{
public:
	CTilemapRenderer()
		: CComponent(eComponentType::TileRenderer), mTexture(nullptr), mIndex(SVector2D()), mTileSize(SVector2D(tileSizeX, tileSizeY)) {

	}
	virtual ~CTilemapRenderer() {}

	virtual CComponent* Clone() override {
		return new CTilemapRenderer(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetTexture(CTexture* tTexture) {
		this->mTexture = tTexture;
	}

	void SetIndex(SVector2D tIndex) {
		this->mIndex = tIndex;
	}

	SVector2D GetIndex() {
		return this->mIndex;
	}

public:
	static SVector2D TileSize;

private:

	SVector2D mTileSize;

	CTexture* mTexture;
	SVector2D mIndex;
};
