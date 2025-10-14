#include "GameEngine.h"
#include "CTexture.h"
#include "winMacro.h"
#include "CResourceMgr.h"

CTexture* CTexture::Create(CAPIEngine* tEngine, const std::wstring& tName, UINT tWidth, UINT tHeight)
{
	CTexture* image = CResourceMgr::Find<CTexture>(tName);

	if (image) return image;

	image = new CTexture();
	image->SetName(tName);
	image->SetWidth(tWidth);
	image->SetHeight(tHeight);

	HDC hDC = tEngine->GetmhDC();
	HWND hWND = tEngine->GetmhWnd();
	
	image->mhBitmap = CreateCompatibleBitmap(hDC, tWidth, tHeight);
	image->mhDCMem = CreateCompatibleDC(hDC);

	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, transparentBrush);

	Rectangle(hDC,
		-1, -1, image->GetWidth() + 1, image->GetHeight() + 1);

	SelectObject(hDC, oldBrush);

	image->mhOldBitmap = (HBITMAP)SelectObject(image->mhDCMem, image->mhBitmap);

	CResourceMgr::Insert(tName, image);

	return image;
}

HRESULT CTexture::Load(CAPIEngine* tEngine, const std::wstring& tPath)
{
	std::wstring ext = L"";
	if (tPath != L"") ext = tPath.substr(tPath.find_last_of(L".") + 1);

	// bmp
	if (ext == L"bmp") {
		mType = eTextureType::Bmp;
		mhBitmap = (HBITMAP)LoadImageW(nullptr, tPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (mhBitmap == nullptr) return S_FALSE;

		GetObject(mhBitmap, sizeof(BITMAP), &mBitmapInfo);

		mWidth = mBitmapInfo.bmWidth;
		mHeight = mBitmapInfo.bmHeight;
		if (mBitmapInfo.bmBitsPixel == 32) {
			mbAlpha = true;
		}
		else {
			mbAlpha = false;
		}

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
