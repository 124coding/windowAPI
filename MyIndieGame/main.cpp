#include "framework.h"
#include "Resource.h"
#include "GameEngine.h"

#include "CSceneMgr.h"

#pragma comment(lib,"winAPIEngine.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GameEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    MSG msg = { 0 };
    msg = tEngine.Run();

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    return (int)msg.wParam;
}