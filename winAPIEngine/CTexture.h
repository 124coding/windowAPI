#pragma once

#include "framework.h"
#include "CResource.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <windows.h>

class SVector2D;

class CTexture : public CResource
{
public:
	enum class eTextureType {
		Bmp,
		Png,
		None
	};

	// 빈 텍스처 생성 (주로 빈 캔버스 용도)
	static CTexture* Create(const std::wstring& tName, UINT tWidth, UINT tHeight);
	CTexture* Clone();

	CTexture() : CResource(eResourceType::Texture), mbAlpha(false) { 
		memset(&mBitmapInfo, 0, sizeof(BITMAP));
	}

	// 소멸자: GDI 및 GDI+ 리소스 해제 (메모리 누수 방지 필수)
	~CTexture(){
		if (mhRightBitmap != nullptr)
		{
			DeleteObject(mhRightBitmap); // 좌우 반전된 비트맵 해제
			mhRightBitmap = nullptr;
		}

		if (mhLeftBitmap != nullptr)
		{
			DeleteObject(mhLeftBitmap);
			mhLeftBitmap = nullptr;
		}

		if (mhDCMem != nullptr)
		{
			SelectObject(mhDCMem, mhOldBitmap); // 원래 비트맵으로 복구 후 삭제
			DeleteDC(mhDCMem);
			mhDCMem = nullptr;
		}

		if (mImage != nullptr)
		{
			delete mImage; // GDI+ 이미지 해제
			mImage = nullptr;
		}
	}

	// 파일로부터 이미지 로딩 (CResource 상속)
	HRESULT Load(const std::wstring& tPath) override;
	void UnLoad();

	// 더블 버퍼링용 DC 및 비트맵 생성
	HRESULT CreateBackBuffer(HDC tDC);

	// GDI+ Image 객체를 기반으로 GDI 호환 비트맵(HBITMAP) 생성
	// 렌더링 속도 향상 및 알파 블렌딩 처리를 위해 변환 과정을 거침
	void CreateHBitmapFromGdiPlus(bool tbAlpha);

	// 생성된 비트맵 제거
	void DeleteHBitmap();

	void SetbAlpha(bool tAlpha) {
		this->mbAlpha = tAlpha;
	}

	bool GetbAlpha() {
		return this->mbAlpha;
	}

	// 좌우 반전(Flip) 최적화:
	// 매 프레임 반전 연산을 하지 않고, 로딩 시 미리 만들어둔 반전 비트맵을 반환
	HBITMAP GetHBitmap(bool tFlipX) {
		if (!tFlipX) {
			return this->mhRightBitmap;
		}
		else {
			return this->mhLeftBitmap;
		}
	}

	HDC GetDCMem() {
		return this->mhDCMem;
	}

	BITMAP GetBitmapInfo() {
		return this->mBitmapInfo;
	}

	void SetBaseWidth(UINT tBaseWidth) {
		this->mBaseWidth = tBaseWidth;
	}

	UINT GetBaseWidth() {
		return this->mBaseWidth;
	}

	void SetBaseHeight(UINT tBaseHeight) {
		this->mBaseHeight = tBaseHeight;
	}

	UINT GetBaseHeight() {
		return this->mBaseHeight;
	}

	void SetWidth(UINT tWidth) {
		this->mWidth = tWidth;
	}

	UINT GetWidth() {
		return this->mWidth;
	}

	void SetHeight(UINT tHeight) {
		this->mHeight = tHeight;
	}

	UINT GetHeight() {
		return this->mHeight;
	}

	eTextureType GetTextureType() {
		return this->mType;
	}

	Gdiplus::Image* GetImage() {
		return this->mImage;
	}

public:
	// 특정 밝기 범위(Threshold) 내의 픽셀 색상을 변경 (팔레트 스왑 등)
	static void ApplyOtherColorToWantedAreas(BYTE tBlackThreshold, BYTE tWhiteThreshold, float tR, float tG, float tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage = nullptr);
	
	// 이미지의 모든 픽셀을 단색으로 변경 (피격 시 하얗게 깜빡이는 효과 등)
	static void ApplySolidColor(BYTE tR, BYTE tG, BYTE tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage = nullptr); // 피격 텍스처 전용 함수(모든 픽셀의 색깔을 똑같이 바꿈)

	// 다른 이미지(tImage)의 일부분을 현재 텍스처에 구워넣기(Baking)
	void BakedTex(float tSrcX, float tSrcY, float tWidth, float tHeight, Gdiplus::Image* tImage);

private:
	bool mbAlpha;	// 알파 채널(투명도) 사용 여부
	eTextureType mType;

	HDC mhDCMem = nullptr;				// 텍스처를 담고 있는 메모리 DC
	HBITMAP mhRightBitmap = nullptr;	// 원본 비트맵 핸들
	HBITMAP mhLeftBitmap = nullptr;		// 좌우 반전된 비트맵 핸들 (최적화용)
	BITMAP mBitmapInfo;					// 비트맵 정보 구조체
	HBITMAP mhOldBitmap = nullptr;		// DC 복구용 기존 비트맵 저장

	Gdiplus::Image* mImage = nullptr;	// GDI+ 이미지 객체 (로딩 및 픽셀 조작용)
	UINT mBaseWidth = 0;
	UINT mBaseHeight = 0;
	UINT mWidth = 0;
	UINT mHeight = 0;

};

