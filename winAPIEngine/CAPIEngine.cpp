#include "CAPIEngine.h"

#include "Resource.h"
#include "CTexture.h"

#include "CResourceMgr.h"
#include "CInputMgr.h"

#include "winMacro.h"

HINSTANCE CAPIEngine::hInst = nullptr;

CAPIEngine::CAPIEngine() {
    // 메모리 초기화 함수를 이용하여 C스타일 문자열 데이터 초기화
    memset(szTitle, 0, MAX_LOADSTRING * sizeof(WCHAR));
    memset(szWindowClass, 0, MAX_LOADSTRING * sizeof(WCHAR));
}

BOOL CAPIEngine::Create(HINSTANCE hInstance, int nCmdShow) {
    /*LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);*/
    wcscpy_s(szTitle, L"MyTestClass");
    wcscpy_s(szWindowClass, L"MyTestClass");

    MyRegisterClass(hInstance, szWindowClass, WndProc);

    RECT rect = { 0, 0, windowWidth, windowHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    SetWindowPos(mhWnd, nullptr, 
        0, 0, 
        rect.right - rect.left, 
        rect.bottom - rect.top, 
        0);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}

MSG CAPIEngine::Run() {
    Gdiplus::GdiplusStartup(&gpToken, &gpsi, NULL);

    mhDC = GetDC(mhWnd);

    mBackBuffer = new CTexture();
    mBackBuffer->CreateBackBuffer(mhDC);

    CInputMgr::GetInst();

    OnCreate();

    QueryPerformanceFrequency(&mSecond);

    QueryPerformanceCounter(&mPrevTime);

    MSG msg = { 0 };

    // 기본 메시지 루프입니다:
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {

            QueryPerformanceCounter(&mCurrentTime);

            mDeltaTime = (float)(mCurrentTime.QuadPart - mPrevTime.QuadPart) / mSecond.QuadPart;
            mPrevTime = mCurrentTime;

            CInputMgr::GetInst()->Update(mhWnd);
            OnUpdate(mDeltaTime);
            OnLateUpdate(mDeltaTime);
        }
    }

    OnDestroy();

    CInputMgr::ReleaseInst();

    if (mBackBuffer != nullptr) {
        mBackBuffer->UnLoad();
        SAFE_DELETE(mBackBuffer);
    }

    ReleaseDC(mhWnd, mhDC);

    Gdiplus::GdiplusShutdown(gpToken);

    return msg;
}

ATOM CAPIEngine::MyRegisterClass(HINSTANCE hInstance, const wchar_t* tName, WNDPROC tProc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = tProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = tName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL CAPIEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    mhWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 
        windowWidth, windowHeight, 
        nullptr, nullptr, hInstance, nullptr);

    if (!mhWnd)
    {
        return FALSE;
    }

    ShowWindow(mhWnd, nCmdShow);
    UpdateWindow(mhWnd);

    return TRUE;
}

LRESULT CALLBACK CAPIEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK CAPIEngine::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CAPIEngine::Clear(float tR, float tG, float tB)
{
    HDC tHDC = mBackBuffer->GetDCMem();

    HPEN thOldPen = NULL;
    HBRUSH thOldBrush = NULL;

    HPEN thPen = NULL;
    HBRUSH thBrush = NULL;

    thPen = CreatePen(PS_SOLID, 2, RGB(tR * 255, tG * 255, tB * 255));
    thBrush = CreateSolidBrush(RGB(tR * 255, tG * 255, tB * 255));

    thOldPen = (HPEN)SelectObject(tHDC, thPen);
    thOldBrush = (HBRUSH)SelectObject(tHDC, thBrush);

    Rectangle(tHDC, 0, 0, windowWidth, windowHeight);

    SelectObject(tHDC, thOldPen);
    SelectObject(tHDC, thOldBrush);

    DeleteObject(thPen);
    DeleteObject(thBrush);
}

void CAPIEngine::Present()
{
    BitBlt(
        mhDC,
        0, 0,
        windowWidth,
        windowHeight,
        mBackBuffer->GetDCMem(),
        0, 0,
        SRCCOPY);
}
