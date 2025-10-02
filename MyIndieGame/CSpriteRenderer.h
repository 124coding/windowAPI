#pragma once
#include "CComponent.h"

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <windows.h>


class CSpriteRenderer : public CComponent
{
public:
	CSpriteRenderer() {}
	~CSpriteRenderer() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC hDC) override;

	void ImageLoad(const std::wstring& path);

private:
	Gdiplus::Image* mImage;
	UINT mWidth;
	UINT mHeight;
};

