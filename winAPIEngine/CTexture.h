#pragma once

#include "framework.h"
#include "CResource.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <windows.h>

class CTexture : public CResource
{
public:
	enum class eTextureType {
		Bmp,
		Png,
		None
	};

	CTexture() : CResource(eResourceType::Texture) { 
		memset(&mBitmapInfo, 0, sizeof(BITMAP));
	}
	~CTexture(){}

	HRESULT Load(CAPIEngine* tEngine, const std::wstring& tPath) override;
	void UnLoad();

	HRESULT CreateBackBuffer(HINSTANCE tInst, HDC tDC);

	HDC GetDCMem() {
		return this->mhDCMem;
	}

	BITMAP GetBitmapInfo() {
		return this->mBitmapInfo;
	}

	UINT GetWidth() {
		return this->mWidth;
	}

	UINT GetHeight() {
		return this->mHeight;
	}

	eTextureType GetTextureType() {
		return this->mType;
	}
	
	Gdiplus::Image* GetImage() {
		return mImage;
	}

private:
	eTextureType mType = eTextureType::None;

	HDC mhDCMem = nullptr;
	HBITMAP mhBitmap = nullptr;
	BITMAP mBitmapInfo;
	HBITMAP mhOldBitmap = nullptr;

	Gdiplus::Image* mImage = nullptr;
	UINT mWidth = 0;
	UINT mHeight = 0;
};

