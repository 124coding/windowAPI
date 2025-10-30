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
		: CComponent(eComponentType::SpriteRenderer), mTexture(nullptr) {
	}
	~CSpriteRenderer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void SetTexture(CTexture* tTexture) {
		this->mTexture = tTexture;
	}

	CTexture* GetTexture() {
		return this->mTexture;
	}

	void SetFlipX(bool tFlipX) {
		this->mbFlipX = tFlipX;
	}

	void SetFlipY(bool tFlipY) {
		this->mbFlipY = tFlipY;
	}

private:
	CTexture* mTexture;

	bool mbFlipX = false;
	bool mbFlipY = false;
};

