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

	CTexture() : CResource(eResourceType::Texture), mbAlpha(false) { 
		memset(&mBitmapInfo, 0, sizeof(BITMAP));
	}
	~CTexture(){}

	HRESULT Load(const std::wstring& tPath) override;
	void UnLoad();

	HRESULT CreateBackBuffer(HDC tDC);

	bool GetbAlpha() {
		return this->mbAlpha;
	}

	HDC GetDCMem() {
		return this->mhDCMem;
	}

	BITMAP GetBitmapInfo() {
		return this->mBitmapInfo;
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

private:
	bool mbAlpha;
	eTextureType mType;

	HDC mhDCMem = nullptr;
	HBITMAP mhBitmap = nullptr;
	BITMAP mBitmapInfo;
	HBITMAP mhOldBitmap = nullptr;

	Gdiplus::Image* mImage = nullptr;
	UINT mWidth = 0;
	UINT mHeight = 0;
};

