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
	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

	HDC mhDC = nullptr; // DC �ڵ�
	// HDC mhToolDC = nullptr;

	HWND mhWnd = nullptr; // ������ �ڵ�
	// HWND mhToolWnd = nullptr; // ���� ������ �ڵ�

protected:
	CTexture* mBackBuffer = nullptr; // �� ����
	// CTexture* mTileBackBuffer = nullptr; // Ÿ�ϸ� �� ����

	// �ð� ���� ����
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

	// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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

	CAPIEngine(const CAPIEngine& t) = delete; // ���� ���� ���� (�ʿ� �� ���� ����)
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

	// Gdiplus�� ����
public:
	ULONG_PTR gpToken;
	Gdiplus::GdiplusStartupInput gpsi;
};