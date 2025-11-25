#include "GameEngine.h"
#include "CTexture.h"
#include "winMacro.h"
#include "CResourceMgr.h"
#include "SVector2D.h"

#include "CRenderer.h"

CTexture* CTexture::Create(const std::wstring& tName, UINT tWidth, UINT tHeight)
{
	CTexture* image = CResourceMgr::Find<CTexture>(tName);

	if (image) return image;

	image = new CTexture();
	image->SetName(tName);
	image->SetWidth(tWidth);
	image->SetHeight(tHeight);

	HDC hDC = mainEngine->GetmhDC();
	HWND hWND = mainEngine->GetmhWnd();
	
	image->mhBitmap = nullptr;
	image->mhDCMem = nullptr;
	image->mhOldBitmap = nullptr;

	image->mType = eTextureType::Png;
	image->mImage = new Gdiplus::Bitmap(tWidth, tHeight, PixelFormat32bppARGB);

	CResourceMgr::Insert(tName, image);

	return image;
}

HRESULT CTexture::Load(const std::wstring& tPath)
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

		HDC mainDC = mainEngine->GetmhDC();
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

HRESULT CTexture::CreateBackBuffer(HDC tDC)
{
	mhDCMem = CreateCompatibleDC(tDC);

	mhBitmap = CreateCompatibleBitmap(tDC, windowWidth, windowHeight);

	GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

	return S_FALSE;
}

void CTexture::CreateHBitmapFromGdiPlus() {
	if (mImage == nullptr) return;

	if (mhDCMem == nullptr) {
		HDC mainDC = mainEngine->GetmhDC();
		mhDCMem = CreateCompatibleDC(mainDC);
	}

	mbAlpha = true;

	if (mhOldBitmap != nullptr) {
		SelectObject(mhDCMem, mhOldBitmap);
	}

	if (mhBitmap != nullptr) {
		DeleteObject(mhBitmap);
		mhBitmap = nullptr;
	}

	Gdiplus::Bitmap* bitmap = (Gdiplus::Bitmap*)mImage;
	bitmap->GetHBITMAP(Gdiplus::Color(255, 0, 255), &mhBitmap);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);
}

void CTexture::ApplyOtherColorToWantedAreas(BYTE tBlackThreshold, BYTE tWhiteThreshold, float tR, float tG, float tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage)
{
	if (tImage == nullptr) {
		return;
	}

	int width = tImage->GetWidth();
	int height = tImage->GetHeight();

	// Basic 이미지 복사
	if (tBasicImage != nullptr) {
		Gdiplus::Graphics graphics(tImage);
		graphics.Clear(Gdiplus::Color(0, 0, 0, 0));
		graphics.DrawImage(tBasicImage, 0, 0, width, height);
	}
	
	Gdiplus::Bitmap* targetBitmap = (Gdiplus::Bitmap*)tImage;
	Gdiplus::Rect rect(0, 0, width, height);
	Gdiplus::BitmapData bitmapData;

	// 쓰기 모드로 잠금
	if (Gdiplus::Ok != targetBitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData))
	{
		return;
	}

	BYTE* pixels = (BYTE*)bitmapData.Scan0;
	int stride = bitmapData.Stride;

	for (int i = 0; i < height; i++) {
		BYTE* row = pixels + (i * stride);

		for (int j = 0; j < width; j++) {
			// BGRA 순서
			BYTE b = row[j * 4 + 0];
			BYTE g = row[j * 4 + 1];
			BYTE r = row[j * 4 + 2];
			BYTE a = row[j * 4 + 3];

			if (a == 0) continue; // 투명하면 패스

			BYTE brigtness = (BYTE)((r + g + b) / 3);

			if (brigtness > tBlackThreshold && brigtness < tWhiteThreshold) {
				float ratio = brigtness / 255.0f;

				// 명암 비율 곱해서 적용
				row[j * 4 + 0] = (BYTE)(tB * ratio);
				row[j * 4 + 1] = (BYTE)(tG * ratio);
				row[j * 4 + 2] = (BYTE)(tR * ratio);
			}
		}
	}

	targetBitmap->UnlockBits(&bitmapData);
}

void CTexture::ApplySolidColor(BYTE tR, BYTE tG, BYTE tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage) {
	if (tImage == nullptr) return;

	int width = tImage->GetWidth();
	int height = tImage->GetHeight();

	if (tBasicImage != nullptr)
	{
		Gdiplus::Graphics graphics(tImage);
		graphics.Clear(Gdiplus::Color(0, 0, 0, 0));
		graphics.DrawImage(tBasicImage, 0, 0, width, height);
	}

	Gdiplus::Bitmap* targetBitmap = (Gdiplus::Bitmap*)tImage;
	Gdiplus::Rect rect(0, 0, width, height);
	Gdiplus::BitmapData bitmapData;

	// 쓰기 모드로 잠금
	if (Gdiplus::Ok != targetBitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData))
	{
		return;
	}

	BYTE* pixels = (BYTE*)bitmapData.Scan0;
	int stride = bitmapData.Stride;

	for (int i = 0; i < height; i++) {
		BYTE* row = pixels + (i * stride);

		for (int j = 0; j < width; j++) {
			BYTE a = row[j * 4 + 3];

			if (a > 80) {
				row[j * 4 + 0] = tB;
				row[j * 4 + 1] = tG;
				row[j * 4 + 2] = tR;
				row[j * 4 + 3] = 255;
			}
		}
	}

	targetBitmap->UnlockBits(&bitmapData);
}