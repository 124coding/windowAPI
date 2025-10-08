#pragma once
#include "CComponent.h"
#include "CTexture.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <windows.h>


class CSpriteRenderer : public CComponent
{
public:
	CSpriteRenderer() 
		: CComponent(eComponentType::SpriteRenderer), mTexture(nullptr), mSize(SVector2D(1.0f, 1.0f)) {}
	~CSpriteRenderer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetTexture(CTexture* tTexture) {
		this->mTexture = tTexture;
	}

	void SetSize(SVector2D tSize) {
		this->mSize = tSize;
	}

private:
	CTexture* mTexture;
	SVector2D mSize;
};

