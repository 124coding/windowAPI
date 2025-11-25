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
	virtual ~CSpriteRenderer() {}

	virtual CComponent* Clone() override {
		return new CSpriteRenderer(*this);
	}

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

	/*void SetFlipY(bool tFlipY) {
		this->mbFlipY = tFlipY;
	}*/

	void SetAlphaMultiplier(float tAlpha) {
		this->mAlphaMultiplier = tAlpha;
	}

	void SetGdiplusDraw(bool tDraw) {
		this->mbGdiplusDraw = tDraw;
	}

private:
	CTexture* mTexture;

	bool mbFlipX = false;
	// bool mbFlipY = false;

	bool mbGdiplusDraw = false;
	float mAlphaMultiplier = 1.0f;
};

