#include "GameEngine.h"
#include "CTexture.h"
#include "winMacro.h"

HRESULT CTexture::Load(CAPIEngine* tEngine, const std::wstring& tPath)
{
	std::wstring ext = tPath.substr(tPath.find_last_of(L".") + 1);

	// bmp
	if (ext == L"bmp") {
		mType = eTextureType::Bmp;
		mhBitmap = (HBITMAP)LoadImageW(nullptr, tPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (mhBitmap == nullptr) return S_FALSE;

		GetObject(mhBitmap, sizeof(BITMAP), &mBitmapInfo);

		mWidth = mBitmapInfo.bmWidth;
		mHeight = mBitmapInfo.bmHeight;

		HDC mainDC = tEngine->GetmhDC();
		mhDCMem = CreateCompatibleDC(mainDC);

		mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);
	}
	else if (ext == L"png") { // png
		mType = eTextureType::Png;
		mImage = Gdiplus::Image::FromFile(tPath.c_str());
		if (mImage == nullptr) {
			return S_FALSE;
		}
		mWidth = mImage->GetWidth();
		mHeight = mImage->GetHeight();
	}

	/*mImage = Gdiplus::Image::FromFile(tPath.c_str());
	mWidth = mImage->GetWidth();
	mHeight = mImage->GetHeight();*/

	return S_OK;
}

void CTexture::UnLoad()
{
	SAFE_DELETE(mImage);

	SelectObject(mhDCMem, mhOldBitmap);

	DeleteObject(mhBitmap);

	DeleteDC(mhDCMem);
}

HRESULT CTexture::CreateBackBuffer(HINSTANCE tInst, HDC tDC)
{
	mhDCMem = CreateCompatibleDC(tDC);

	mhBitmap = CreateCompatibleBitmap(tDC, windowWidth, windowHeight);

	GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

	return S_FALSE;
}
