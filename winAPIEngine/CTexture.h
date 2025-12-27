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

	static CTexture* Create(const std::wstring& tName, UINT tWidth, UINT tHeight);
	CTexture* Clone();

	CTexture() : CResource(eResourceType::Texture), mbAlpha(false) { 
		memset(&mBitmapInfo, 0, sizeof(BITMAP));
	}
	~CTexture(){
		if (mhRightBitmap != nullptr)
		{
			DeleteObject(mhRightBitmap);
			mhRightBitmap = nullptr;
		}

		if (mhLeftBitmap != nullptr)
		{
			DeleteObject(mhLeftBitmap);
			mhLeftBitmap = nullptr;
		}

		if (mhDCMem != nullptr)
		{
			SelectObject(mhDCMem, mhOldBitmap);
			DeleteDC(mhDCMem);
			mhDCMem = nullptr;
		}

		if (mImage != nullptr)
		{
			delete mImage;
			mImage = nullptr;
		}
	}

	HRESULT Load(const std::wstring& tPath) override;
	void UnLoad();

	HRESULT CreateBackBuffer(HDC tDC);
	void CreateHBitmapFromGdiPlus(bool tbAlpha);
	void DeleteHBitmap();

	void SetbAlpha(bool tAlpha) {
		this->mbAlpha = tAlpha;
	}

	bool GetbAlpha() {
		return this->mbAlpha;
	}

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
	static void ApplyOtherColorToWantedAreas(BYTE tBlackThreshold, BYTE tWhiteThreshold, float tR, float tG, float tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage = nullptr);
	static void ApplySolidColor(BYTE tR, BYTE tG, BYTE tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage = nullptr); // 피격 텍스처 전용 함수(모든 픽셀의 색깔을 똑같이 바꿈)

	void BakedTex(float tSrcX, float tSrcY, float tWidth, float tHeight, Gdiplus::Image* tImage);

private:
	bool mbAlpha;
	eTextureType mType;

	HDC mhDCMem = nullptr;
	HBITMAP mhRightBitmap = nullptr;
	HBITMAP mhLeftBitmap = nullptr;
	BITMAP mBitmapInfo;
	HBITMAP mhOldBitmap = nullptr;

	Gdiplus::Image* mImage = nullptr;
	UINT mBaseWidth = 0;
	UINT mBaseHeight = 0;
	UINT mWidth = 0;
	UINT mHeight = 0;

};

