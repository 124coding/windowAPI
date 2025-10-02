#include "CTexture.h"
#include "winMacro.h"

bool CTexture::LoadTexture(HINSTANCE tInst, HDC tDC, LPCWSTR tFileName)
{
	mhDCMem = CreateCompatibleDC(tDC);

	mhBitmap = (HBITMAP)LoadImage(tInst, tFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

	return false;
}

void CTexture::UnLoadTexture()
{
	SelectObject(mhDCMem, mhOldBitmap);

	DeleteObject(mhBitmap);

	DeleteDC(mhDCMem);
}

bool CTexture::CreateBackBuffer(HINSTANCE tInst, HDC tDC)
{
	mhDCMem = CreateCompatibleDC(tDC);

	mhBitmap = CreateCompatibleBitmap(tDC, windowWidth, windowHeight);

	GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

	return false;
}
