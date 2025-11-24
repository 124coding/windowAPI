#include "CToolScene.h"

#include "CTile.h"
#include "CTexture.h"

#include "CInputMgr.h"

#include "CCamera.h"
#include "CCameraScript.h"
#include "CTilemapRenderer.h"

#include "CRenderer.h"
#include "Object.h"

#include <commdlg.h>

SVector2D CToolScene::mIndex;
CTexture* CToolScene::mMapTileTexture = nullptr;

void CToolScene::OnCreate()
{
	mEngine = mainEngine;

	SetMapTileTexture(L"Tile1");

	CTile* tile = Instantiate<CTile>(eLayerType::Tile);

	CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();

	GameObject* mapOutLine = Instantiate<GameObject>(eLayerType::Tile);
	CTexture* mapOLImg = CResourceMgr::Find<CTexture>(L"TileOutLine");

	CTransform* mapOLTr = mapOutLine->GetComponent<CTransform>();

	float aspectRatioX = (float)mapWidth / mapOLImg->GetWidth();
	float aspectRatioY = (float)mapHeight / mapOLImg->GetHeight();

	mapOutLine->SetSize(SVector2D(aspectRatioX, aspectRatioY));
	mapOutLine->SetAnchorPoint(mapOLImg->GetWidth() / 2, mapOLImg->GetHeight() / 2);

	mapOLTr->SetPos(SVector2D(mapWidth / 2, mapHeight / 2));

	CSpriteRenderer* mapOLSr = mapOutLine->AddComponent<CSpriteRenderer>();
	mapOLSr->SetTexture(mapOLImg);

	CScene::OnCreate();
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
		SVector2D mousePos = CInputMgr::GetMousePosition();

		SVector2D pos = mainCamera->CalculateTilePosition(mousePos);

		if (pos.mX >= 0.0f && pos.mY >= 0.0f) {
			SVector2D index;
			index = pos / CTilemapRenderer::TileSize;

			index.mX = (int)index.mX;
			index.mY = (int)index.mY;

			CTile* tile = Instantiate<CTile>(eLayerType::Tile);

			CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
			tmr->SetTexture(mMapTileTexture);
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
	for (size_t i = 0; i < 47; i++) {
		SVector2D pos = mainCamera->CalculatePosition(SVector2D(CTilemapRenderer::TileSize.mX * i, 0.0f));
		
		MoveToEx(tHDC, pos.mX, 0, NULL);
		LineTo(tHDC, pos.mX, windowHeight);		
	}

	for (size_t i = 0; i < 47; i++) {
		SVector2D pos = mainCamera->CalculatePosition(SVector2D(0.0f, CTilemapRenderer::TileSize.mY * i));

		MoveToEx(tHDC, 0, pos.mY, NULL);
		LineTo(tHDC, windowWidth, pos.mY);

	}

	if (mMapTileTexture->GetTextureType() == CTexture::eTextureType::Bmp) {

		if (mMapTileTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;

			func.SourceConstantAlpha = 255;
			AlphaBlend(mhToolDC, 0, 0,
				mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight(),
				mMapTileTexture->GetDCMem(),
				0, 0,
				mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight(), func);
		}
		else {
			TransparentBlt(mhToolDC, 0, 0,
				mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight(),
				mMapTileTexture->GetDCMem(),
				0, 0,
				mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight(),
				RGB(255, 0, 255));
		}
	}
	else if (mMapTileTexture->GetTextureType() == CTexture::eTextureType::Png) {
		Gdiplus::ImageAttributes imgAtt = {};

		// 투명화 시킬 픽셀의 색 범위
		imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

		Gdiplus::Graphics graphics(mhToolDC);

		graphics.DrawImage(mMapTileTexture->GetImage(),
			0, 0,
			mMapTileTexture->GetWidth(),
			mMapTileTexture->GetHeight()
		);
	}

	CScene::Render(tHDC);
}

void CToolScene::OnEnter()
{
	CScene::OnEnter();

	mainCamera->GetOwner()->AddComponent<CCameraScript>();

    CreateTileWindow(mEngine->hInst, SW_SHOWDEFAULT); // nCmdShow에 임의의 값 투입

	mhToolDC = GetDC(mhToolWnd);

	mTileBackBuffer = new CTexture();
	mTileBackBuffer->CreateBackBuffer(mhToolDC);
}

void CToolScene::OnExit()
{
	CScene::OnExit();

	mainCamera->GetOwner()->RemoveComponent<CCameraScript>();

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

		CTile* tile = Instantiate<CTile>(eLayerType::Tile, SVector2D(posX, posY));

		CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		tmr->SetTexture(mMapTileTexture);
		tmr->SetIndex(SVector2D(idxX, idxY));

		tile->SetIndexPosition(SVector2D(posX, posY) / CTilemapRenderer::TileSize);
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

    RECT rect = { 0, 0, mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight() };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

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
