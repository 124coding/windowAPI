#include "CToolScene.h"
#include "CTile.h"
#include "CTexture.h"

#include "CResourceMgr.h"
#include "CInputMgr.h"

#include "CCamera.h"
#include "CTilemapRenderer.h"

#include "CRenderer.h"
#include "Object.h"

void CToolScene::OnCreate(CAPIEngine* tEngine)
{
	CScene::OnCreate(tEngine);

	mEngine = tEngine;

	GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(800, 400));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;

	CTile* tile = Instantiate<CTile>(eLayerType::Tile);
	tile->SetSize(SVector2D(3.0f, 3.0f));

	CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
	tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));
}

void CToolScene::OnDestroy()
{
	CScene::OnDestroy();

	mEngine = nullptr;
}

void CToolScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CToolScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);

	if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
		SVector2D pos = CInputMgr::GetMousePosition();

		SVector2D index;
		index = pos / CTilemapRenderer::TileSize;

		index.mX = (int)index.mX;
		index.mY = (int)index.mY;

		CTile* tile = Instantiate<CTile>(eLayerType::Tile);
		tile->SetSize(SVector2D(3.0f, 3.0f));

		CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));

		tile->SetPosition(index);
	}

	/*CInputMgr::GetInst()->Update(mhToolWnd);

	if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
		int a = 0;
	}*/
}

void CToolScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

	for (size_t i = 0; i < 200; i++) {
		MoveToEx(tHDC, CTilemapRenderer::TileSize.mX * i, 0, NULL);
		LineTo(tHDC, CTilemapRenderer::TileSize.mX * i, 1000);
	}

	for (size_t i = 0; i < 200; i++) {
		MoveToEx(tHDC, 0, CTilemapRenderer::TileSize.mY * i, NULL);
		LineTo(tHDC, 2000, CTilemapRenderer::TileSize.mY * i);
	}

	CTexture* texture = CResourceMgr::Load<CTexture>(mEngine, L"SpringFloor", L"../resources/SpringFloor.bmp");

	TransparentBlt(mhToolDC, 0, 0,
		texture->GetWidth(), texture->GetHeight(),
		texture->GetDCMem(),
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		RGB(255, 0, 255));
}

void CToolScene::OnEnter()
{
	CScene::OnEnter();

    CreateTileWindow(mEngine->hInst, SW_SHOWDEFAULT); // nCmdShow에 임의의 값 투입

	mhToolDC = GetDC(mhToolWnd);

	mTileBackBuffer = new CTexture();
	mTileBackBuffer->CreateBackBuffer(mhToolDC);
}

void CToolScene::OnExit()
{
	CScene::OnExit();

	if (mTileBackBuffer != nullptr) {
	    mTileBackBuffer->UnLoad();
	    SAFE_DELETE(mTileBackBuffer);
	}

	ReleaseDC(mhToolWnd, mhToolDC);

    DestroyWindow(mhToolWnd);
}

BOOL CToolScene::CreateTileWindow(HINSTANCE hInstance, int nCmdShow)
{
    mEngine->MyRegisterClass(hInstance, L"TILEWINDOW", WndTileProc);

    if (!InitInstanceTileWIndow(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CToolScene::InitInstanceTileWIndow(HINSTANCE hInstance, int nCmdShow)
{
    CAPIEngine::hInst = hInstance;

	mhToolWnd = CreateWindowW(L"TILEWINDOW", L"TILE", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        windowWidth, windowHeight,
        nullptr, nullptr, hInstance, nullptr);

    if (!mhToolWnd)
    {
        return FALSE;
    }

    CTexture* texture = CResourceMgr::Load<CTexture>(mEngine, L"SpringFloor", L"../resources/SpringFloor.bmp");

    RECT rect = { 0, 0, texture->GetWidth(), texture->GetHeight() };
    AdjustWindowRect(&rect, WS_OVERLAPPED, false);

    SetWindowPos(mhToolWnd, nullptr,
		windowWidth, 0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0);

    ShowWindow(mhToolWnd, nCmdShow);
    UpdateWindow(mhToolWnd);

    return TRUE;
}

LRESULT CALLBACK CToolScene::WndTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
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
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
