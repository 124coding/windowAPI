#pragma once
#include <windows.h>

class CTexture
{
public:
	CTexture() { memset(&mBitmapInfo, 0, sizeof(BITMAP)); }
	~CTexture(){}

	bool LoadTexture(HINSTANCE tInst, HDC tDC, LPCWSTR tFileName);
	void UnLoadTexture();

	bool CreateBackBuffer(HINSTANCE tInst, HDC tDC);

	HDC GetDCMem() {
		return this->mhDCMem;
	}

	BITMAP GetBitmapInfo() {
		return this->mBitmapInfo;
	}

private:
	HDC mhDCMem = nullptr;
	HBITMAP mhBitmap = nullptr;
	BITMAP mBitmapInfo;
	HBITMAP mhOldBitmap = nullptr;
};

