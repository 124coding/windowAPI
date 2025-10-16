#pragma once
#include "framework.h"

#include <ole2.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib,"msimg32.lib")

#define MAX_LOADSTRING 100

class CTexture;

class CAPIEngine
{
	friend class CToolScene;
private:
	static HINSTANCE hInst;                                // 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

	HDC mhDC = nullptr; // DC 핸들
	// HDC mhToolDC = nullptr;

	HWND mhWnd = nullptr; // 윈도우 핸들
	// HWND mhToolWnd = nullptr; // 툴용 윈도우 핸들

protected:
	CTexture* mBackBuffer = nullptr; // 백 버퍼
	// CTexture* mTileBackBuffer = nullptr; // 타일맵 백 버퍼

	// 시간 관련 변수
private:
	LARGE_INTEGER mSecond = {};
	LARGE_INTEGER mCurrentTime = {};
	LARGE_INTEGER mPrevTime = {};

	float mDeltaTime = 0.0f;

public:
	void SetmhDC(HDC tHDC) {
		this->mhDC = tHDC;
	}

	HDC GetmhDC() {
		return this->mhDC;
	}

	HWND GetmhWnd() {
		return this->mhWnd;
	}

	// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
protected:
	ATOM                MyRegisterClass(HINSTANCE hInstance, const wchar_t* tName, WNDPROC tProc);
	BOOL                InitInstance(HINSTANCE, int);
	// BOOL                InitInstanceTileWIndow(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	// static LRESULT CALLBACK	   WndTileProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

public:
	CAPIEngine();
	virtual ~CAPIEngine() {};

	CAPIEngine(const CAPIEngine& t) = delete; // 엔진 복제 막음 (필요 시 해제 가능)
	CAPIEngine operator=(const CAPIEngine& t) = delete;

	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	// BOOL CreateTileWindow(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float tDeltaTime) = 0;
	virtual void OnLateUpdate(float tDeltaTime) = 0;
	virtual void Render() = 0;

	void Clear(float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	void Present();

	// Gdiplus용 변수
public:
	ULONG_PTR gpToken;
	Gdiplus::GdiplusStartupInput gpsi;
};