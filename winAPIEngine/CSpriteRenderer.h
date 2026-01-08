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

	// 렌더링할 텍스처(이미지) 리소스 설정
	void SetTexture(CTexture* tTexture) {
		this->mTexture = tTexture;
	}

	CTexture* GetTexture() {
		return this->mTexture;
	}

	// 이미지 좌우 반전 설정 (true: 반전, false: 원본)
	// 캐릭터가 왼쪽/오른쪽을 볼 때 텍스처를 뒤집는 용도로 사용
	void SetFlipX(bool tFlipX) {
		this->mbFlipX = tFlipX;
	}

	/*void SetFlipY(bool tFlipY) {
		this->mbFlipY = tFlipY;
	}*/

	// 전체 투명도 조절 (0.0f ~ 1.0f)
	// 페이드 인/아웃 효과나 유령 같은 반투명 오브젝트 표현 시 사용
	void SetAlphaMultiplier(float tAlpha) {
		this->mAlphaMultiplier = tAlpha;
	}

	// GDI+ 사용 여부 설정
	// true: 회전/확대축소 품질이 좋지만 느림 (Gdiplus)
	// false: 빠르지만 회전 시 품질이 떨어지거나 지원 안 함 (GDI: BitBlt/AlphaBlend/TransparentBlt)
	void SetGdiplusDraw(bool tDraw) {
		this->mbGdiplusDraw = tDraw;
	}

private:
	CTexture* mTexture;				// 출력할 이미지 리소스

	bool mbFlipX = false;			// 좌우 반전 플래그
	// bool mbFlipY = false;

	bool mbGdiplusDraw = false;		// 렌더링 방식 플래그
	float mAlphaMultiplier = 1.0f;	// 알파값 곱연산 인자
};

