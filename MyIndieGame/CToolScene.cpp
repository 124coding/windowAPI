#include "CToolScene.h"

#include "CTile.h"
#include "CTexture.h"

#include "CResourceMgr.h"
#include "CInputMgr.h"

#include "CCamera.h"
#include "CCameraScript.h"
#include "CTilemapRenderer.h"

#include "CRenderer.h"
#include "Object.h"

#include <commdlg.h>

SVector2D CToolScene::mIndex;

void CToolScene::OnCreate(CAPIEngine* tEngine)
{
	CScene::OnCreate(tEngine);

	mEngine = tEngine;

	GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(800, 400));
	CCamera* cameraComp = camera->AddComponent<CCamera>();
	camera->AddComponent<CCameraScript>();

	mainCamera = cameraComp;

	CTile* tile = Instantiate<CTile>(mEngine, eLayerType::Tile, SVector2D());
	tile->SetSize(SVector2D(2.0f, 2.0f));

	CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
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

		pos = mainCamera->CaluateTilePosition(pos);

		if (pos.mX >= 0.0f && pos.mY >= 0.0f) {
			SVector2D index;
			index = pos / CTilemapRenderer::TileSize;

			index.mX = (int)index.mX;
			index.mY = (int)index.mY;

			CTile* tile = Instantiate<CTile>(mEngine, eLayerType::Tile);
			tile->SetSize(SVector2D(2.0f, 2.0f));

			CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
			tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));
			tmr->SetIndex(mIndex);

			tile->SetIndexPosition(index);
			mTiles.push_back(tile);
		}
		else {
			int a = 0;
		}
	}

	if (CInputMgr::GetInst()->GetKeyDown("DoSave")) {
		Save();
	}

	if (CInputMgr::GetInst()->GetKeyDown("DoLoad")) {
		Load();
	}
}

void CToolScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

	for (size_t i = 0; i < 200; i++) {
		SVector2D pos = mainCamera->CaluatePosition(SVector2D(CTilemapRenderer::TileSize.mX * i, 0.0f));
		
		MoveToEx(tHDC, pos.mX, 0, NULL);
		LineTo(tHDC, pos.mX, 1000);		
	}

	for (size_t i = 0; i < 200; i++) {
		SVector2D pos = mainCamera->CaluatePosition(SVector2D(0.0f, CTilemapRenderer::TileSize.mY * i));

		MoveToEx(tHDC, 0, pos.mY, NULL);
		LineTo(tHDC, 2000, pos.mY);

	}

	CTexture* texture = CResourceMgr::Load<CTexture>(mEngine, L"SpringFloor", L"../resources/SpringFloor.bmp");

	TransparentBlt(mhToolDC, 0, 0,
		texture->GetWidth() * 2, texture->GetHeight() * 2,
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

void CToolScene::Save()
{
	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"wb");

	for (CTile* t : mTiles) {
		CTilemapRenderer* tmr = t->GetComponent<CTilemapRenderer>();
		CTransform* tr = t->GetComponent<CTransform>();

		SVector2D sourceIndex = tmr->GetIndex();
		SVector2D pos = tr->GetPos();

		int x = (int)sourceIndex.mX;
		fwrite(&x, sizeof(int), 1, pFile);
		int y = (int)sourceIndex.mY;
		fwrite(&y, sizeof(int), 1, pFile);

		x = (int)pos.mX;
		fwrite(&x, sizeof(int), 1, pFile);
		y = (int)pos.mY;
		fwrite(&y, sizeof(int), 1, pFile);
	}

	fclose(pFile);
}

void CToolScene::Load()
{
	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"rb");

	while (true) {
		int idxX = 0;
		int idxY = 0;

		int posX = 0;
		int posY = 0;

		if (fread(&idxX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&idxY, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posY, sizeof(int), 1, pFile) == NULL) break;

		CTile* tile = Instantiate<CTile>(mEngine, eLayerType::Tile, SVector2D(posX, posY));
		tile->SetSize(SVector2D(2.0f, 2.0f));

		CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));
		tmr->SetIndex(SVector2D(idxX, idxY));

		// tile->SetIndexPosition(SVector2D(posX, posY) / CTilemapRenderer::TileSize);
		mTiles.push_back(tile);
	}

	fclose(pFile);
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

    RECT rect = { 0, 0, texture->GetWidth() * 2, texture->GetHeight() * 2 };
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
    case WM_LBUTTONDOWN:
    {
		CInputMgr::GetInst()->Update(hWnd);

		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			SVector2D pos = CInputMgr::GetMousePosition();

			SVector2D index;
			index = pos / CTilemapRenderer::TileSize;

			index.mX = (int)index.mX;
			index.mY = (int)index.mY;

			mIndex = index;
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
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
