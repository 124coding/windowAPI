#include "GameEngine.h"
#include "CTexture.h"
#include "winMacro.h"
#include "CResourceMgr.h"
#include "SVector2D.h"

#include "CRenderer.h"

CTexture* CTexture::Create(const std::wstring& tName, UINT tWidth, UINT tHeight)
{
	// 리소스 매니저에서 이미 로드된 텍스처인지 확인
	CTexture* image = CResourceMgr::Find<CTexture>(tName);

	if (image) return image;

	// 없다면 새로 생성
	image = new CTexture();
	image->SetName(tName);
	image->SetWidth(tWidth);
	image->SetHeight(tHeight);
	image->SetBaseWidth(tWidth);
	image->SetBaseHeight(tHeight);

	HDC mainDC = mainEngine->GetmhDC();
	
	image->mhRightBitmap = nullptr;
	image->mhLeftBitmap = nullptr;
	image->mhDCMem = CreateCompatibleDC(mainDC);	// 메인 DC와 호환되는 메모리 DC 생성
	image->mhOldBitmap = nullptr;

	image->mType = eTextureType::Png;	// 기본적으로 투명도 지원을 위해 PNG 포맷 가정
	image->mImage = new Gdiplus::Bitmap(tWidth, tHeight, PixelFormat32bppARGB);

	CResourceMgr::Insert(tName, image);

	return image;
}

CTexture* CTexture::Clone() {
	CTexture* pCloneTex = new CTexture();
	HDC mainDC = mainEngine->GetmhDC();

	pCloneTex->mType = this->mType;
	pCloneTex->mbAlpha = this->mbAlpha;
	pCloneTex->mBaseWidth = this->mBaseWidth;
	pCloneTex->mBaseHeight = this->mBaseHeight;
	pCloneTex->mWidth = this->mWidth;
	pCloneTex->mHeight = this->mHeight;
	pCloneTex->mhDCMem = CreateCompatibleDC(mainDC);

	if (this->mImage != nullptr)
	{
		pCloneTex->mImage = this->mImage->Clone();
	}

	return pCloneTex;
}

HRESULT CTexture::Load(const std::wstring& tPath)
{
	std::wstring ext = L"";
	if (tPath != L"") ext = tPath.substr(tPath.find_last_of(L".") + 1);

	// 1. BMP 로딩 (GDI 방식)
	if (ext == L"bmp") {
		mType = eTextureType::Bmp;
		mhRightBitmap = (HBITMAP)LoadImageW(nullptr, tPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (mhRightBitmap == nullptr) return S_FALSE;

		GetObject(mhRightBitmap, sizeof(BITMAP), &mBitmapInfo);

		mWidth = mBitmapInfo.bmWidth;
		mHeight = mBitmapInfo.bmHeight;

		// 32비트면 알파 채널이 있다고 가정
		if (mBitmapInfo.bmBitsPixel == 32) {
			mbAlpha = true;
		}
		else {
			mbAlpha = false;
		}

		HDC mainDC = mainEngine->GetmhDC();
		mhDCMem = CreateCompatibleDC(mainDC);

		mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhRightBitmap);
	}
	// 2. PNG 로딩 (GDI+ 방식)
	else if (ext == L"png") {
		mType = eTextureType::Png;
		mImage = Gdiplus::Image::FromFile(tPath.c_str());
		if (mImage == nullptr) {
			return S_FALSE;
		}
		mWidth = mImage->GetWidth();
		mHeight = mImage->GetHeight();
		mBaseWidth = mImage->GetWidth();
		mBaseHeight = mImage->GetHeight();
	}

	return S_OK;
}

void CTexture::UnLoad()
{
	SAFE_DELETE(mImage);

	SelectObject(mhDCMem, mhOldBitmap);

	DeleteObject(mhRightBitmap);
	DeleteObject(mhLeftBitmap);

	DeleteDC(mhDCMem);
}

HRESULT CTexture::CreateBackBuffer(HDC tDC)
{
	// 더블 버퍼링을 위한 백버퍼 생성
	mhDCMem = CreateCompatibleDC(tDC);

	mhRightBitmap = CreateCompatibleBitmap(tDC, windowWidth, windowHeight);

	GetObject(mhRightBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhRightBitmap);

	return S_OK;
}

// GDI+ 이미지(mImage)를 GDI 비트맵(HBITMAP)으로 변환
// 목적: 매 프레임 GDI+로 그리는 것보다, 메모리 DC에 비트맵을 박아두고 BitBlt 하는 것이 훨씬 빠름
void CTexture::CreateHBitmapFromGdiPlus(bool tbAlpha) {
	if (mImage == nullptr) return;

	if (mhDCMem == nullptr) {
		HDC mainDC = mainEngine->GetmhDC();
		mhDCMem = CreateCompatibleDC(mainDC);
	}

	mbAlpha = tbAlpha;

	// 기존 비트맵 정리
	if (mhOldBitmap != nullptr) {
		SelectObject(mhDCMem, mhOldBitmap);
	}

	if (mhRightBitmap != nullptr) {
		DeleteObject(mhRightBitmap);
		mhRightBitmap = nullptr;
	}

	if (mhLeftBitmap != nullptr) {
		DeleteObject(mhLeftBitmap);
		mhLeftBitmap = nullptr;
	}

	// 1. 알파 채널 사용 시 (AlphaBlend)
	if (tbAlpha) {
		Gdiplus::Bitmap* bitmap = (Gdiplus::Bitmap*)mImage;

		// 원본(우측 방향) 비트맵 생성
		bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &mhRightBitmap);

		// 좌측 방향(Flip) 비트맵 미리 생성 (매 프레임 반전 연산 비용 절약)
		bitmap->RotateFlip(Gdiplus::RotateNoneFlipX);
		bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &mhLeftBitmap);

		// 다시 원상복구
		bitmap->RotateFlip(Gdiplus::RotateNoneFlipX);
	}
	// 2. 컬러키(Color Key) 사용 시 (TransparentBlt)
	else {
		int width = mImage->GetWidth();
		int height = mImage->GetHeight();

		// 임시 비트맵 생성
		Gdiplus::Bitmap tempBitmap(width, height, PixelFormat32bppARGB);
		Gdiplus::Bitmap* src = (Gdiplus::Bitmap*)mImage;
		Gdiplus::BitmapData srcData, destData;
		Gdiplus::Rect rect(0, 0, width, height);

		// 메모리 직접 접근을 위해 LockBits 호출 (GetPixel보다 훨씬 빠름)
		src->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &srcData);
		tempBitmap.LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &destData);

		BYTE* srcBits = (BYTE*)srcData.Scan0;
		BYTE* destBits = (BYTE*)destData.Scan0;

		const int stride = srcData.Stride;

		// 픽셀 순회하며 알파값 확인
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int idx = (i * stride) + (j * 4);

				// BGRA 포맷
				BYTE b = srcBits[idx + 0];
				BYTE g = srcBits[idx + 1];
				BYTE r = srcBits[idx + 2];
				BYTE a = srcBits[idx + 3];

				// 알파값이 작으면(투명하면) 마젠타 색(255, 0, 255)으로 칠함 -> TransparentBlt의 키 컬러로 사용
				if (a < 128) {
					destBits[idx + 0] = 255;
					destBits[idx + 1] = 0;
					destBits[idx + 2] = 255;
					destBits[idx + 3] = 255;
				}
				else {
					destBits[idx + 0] = b;
					destBits[idx + 1] = g;
					destBits[idx + 2] = r;
					destBits[idx + 3] = 255;	// 불투명하게 강제
				}
			}
		}

		tempBitmap.UnlockBits(&destData);
		src->UnlockBits(&srcData);

		// 마젠타가 배경인 HBITMAP 생성
		tempBitmap.GetHBITMAP(Gdiplus::Color(255, 0, 255), &mhRightBitmap);

		// 좌측 반전 비트맵 생성
		tempBitmap.RotateFlip(Gdiplus::RotateNoneFlipX);
		tempBitmap.GetHBITMAP(Gdiplus::Color(255, 0, 255), &mhLeftBitmap);
		tempBitmap.RotateFlip(Gdiplus::RotateNoneFlipX);

	}

	// 기본적으로 우측 비트맵을 DC에 선택해둠
	mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhRightBitmap);
}

void CTexture::DeleteHBitmap() {
	// DC 복구 및 비트맵 삭제 루틴
	if (mhOldBitmap != nullptr) {
		SelectObject(mhDCMem, mhOldBitmap);
	}

	if (mhRightBitmap != nullptr) {
		DeleteObject(mhRightBitmap);
		mhRightBitmap = nullptr;
	}

	if (mhLeftBitmap != nullptr) {
		DeleteObject(mhLeftBitmap);
		mhLeftBitmap = nullptr;
	}
}

// 픽셀 단위로 밝기를 분석하여 특정 범위의 색상을 변경 (팔레트 스왑 효과)
void CTexture::ApplyOtherColorToWantedAreas(BYTE tBlackThreshold, BYTE tWhiteThreshold, float tR, float tG, float tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage)
{
	if (tImage == nullptr) {
		return;
	}

	int width = tImage->GetWidth();
	int height = tImage->GetHeight();

	// 1. 원본 이미지(Basic)가 있다면 먼저 복구 (누적 변경 방지)
	if (tBasicImage != nullptr) {
		Gdiplus::Graphics graphics(tImage);
		graphics.Clear(Gdiplus::Color(0, 0, 0, 0));
		graphics.DrawImage(tBasicImage, 0, 0, width, height);
	}
	
	Gdiplus::Bitmap* targetBitmap = (Gdiplus::Bitmap*)tImage;
	Gdiplus::Rect rect(0, 0, width, height);
	Gdiplus::BitmapData bitmapData;

	// 2. 픽셀 조작을 위해 메모리 잠금
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

			// 명도(Brightness) 계산
			BYTE brigtness = (BYTE)((r + g + b) / 3);

			// 특정 밝기 범위에 있는 픽셀만 타겟 색상으로 변경
			if (brigtness >= tBlackThreshold && brigtness <= tWhiteThreshold) {
				float ratio = brigtness / 255.0f;	// 명암비 유지

				// 명암 비율 곱해서 적용
				row[j * 4 + 0] = (BYTE)(tB * ratio);
				row[j * 4 + 1] = (BYTE)(tG * ratio);
				row[j * 4 + 2] = (BYTE)(tR * ratio);
			}
		}
	}

	targetBitmap->UnlockBits(&bitmapData);
}

// 이미지 전체를 특정 단색으로 덮어씌움 (피격 시 깜빡임 효과 등)
void CTexture::ApplySolidColor(BYTE tR, BYTE tG, BYTE tB, Gdiplus::Image* tImage, Gdiplus::Image* tBasicImage) {
	if (tImage == nullptr) return;

	int width = tImage->GetWidth();
	int height = tImage->GetHeight();

	// 원본 복구
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

			// 투명하지 않은 픽셀은 모두 지정된 색으로 덮어쓰기s
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

// 텍스처 합치기 (Baking)
// 현재 텍스처(mImage) 위에 다른 이미지(tImage)를 지정된 위치에 그린다.
void CTexture::BakedTex(float tSrcX, float tSrcY, float tWidth, float tHeight, Gdiplus::Image* tImage)
{
	Gdiplus::Graphics graphics(mImage);

	graphics.DrawImage(tImage,
		Gdiplus::Rect(tSrcX, tSrcY, tWidth, tHeight),
		0, 0, tImage->GetWidth(), tImage->GetHeight(),
		Gdiplus::UnitPixel);
}