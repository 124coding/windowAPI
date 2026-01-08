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

	// 편집에 사용할 타일셋 텍스처 로드
	SetMapTileTexture(L"Tile1");

	// 기본 타일 객체 생성 (프로토타입 역할 가능성)
	CTile* tile = Instantiate<CTile>(eLayerType::Tile);

	CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();

	// [Map Boundary] 맵의 전체 크기를 시각적으로 보여줄 외곽선 가이드 생성
	GameObject* mapOutLine = Instantiate<GameObject>(eLayerType::Tile);
	CTexture* mapOLImg = CResourceMgr::Find<CTexture>(L"TileOutLine");

	CTransform* mapOLTr = mapOutLine->GetComponent<CTransform>(eComponentType::Transform);

	// 맵 크기에 맞춰 외곽선 이미지 스케일링
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

// 메인 게임 화면에서의 상호작용 (타일 배치, 저장/로드 단축키)
void CToolScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);

	// 마우스 좌클릭 시 타일 배치
	if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
		SVector2D mousePos = CInputMgr::GetMousePosition();

		// [Coordinate Conversion] Screen Pos -> World Pos -> Tile Index
		SVector2D pos = mainCamera->CalculateTilePosition(mousePos);

		if (pos.mX >= 0.0f && pos.mY >= 0.0f) {
			SVector2D index;
			index = pos / CTilemapRenderer::TileSize;

			index.mX = (int)index.mX;
			index.mY = (int)index.mY;

			// 타일 인스턴스 생성 및 배치
			CTile* tile = Instantiate<CTile>(eLayerType::Tile);

			CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
			tmr->SetTexture(mMapTileTexture);
			tmr->SetIndex(mIndex);			// 현재 팔레트에서 선택된 인덱스(mIndex) 적용

			tile->SetIndexPosition(index);
			mTiles.push_back(tile);			// 관리 리스트에 추가
		}
	}

	// 저장/로드 단축키 처리
	if (CInputMgr::GetInst()->GetKeyDown("DoSave")) {
		Save();
	}

	if (CInputMgr::GetInst()->GetKeyDown("DoLoad")) {
		Load();
	}
}

// [Rendering] 메인 화면과 툴 화면(팔레트) 렌더링
void CToolScene::Render(HDC tHDC)
{
	// 1. [Main Window] 그리드(격자) 그리기
	// 타일 배치 편의를 위해 화면에 격자를 그림
	for (size_t i = 0; i < 47; i++) {
		SVector2D pos = mainCamera->CalculatePosition(SVector2D(tileSizeX * i, 0.0f));
		
		MoveToEx(tHDC, pos.mX, 0, NULL);
		LineTo(tHDC, pos.mX, windowHeight);		
	}

	for (size_t i = 0; i < 47; i++) {
		SVector2D pos = mainCamera->CalculatePosition(SVector2D(0.0f, tileSizeY * i));

		MoveToEx(tHDC, 0, pos.mY, NULL);
		LineTo(tHDC, windowWidth, pos.mY);

	}

	// 2. [Tool Window] 타일 팔레트 그리기
	// 메인 DC(tHDC)가 아니라 툴 윈도우의 DC(mhToolDC)에 그림
	if (mMapTileTexture->GetTextureType() == CTexture::eTextureType::Bmp) {
		// BMP 렌더링 (투명도 처리)
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
		// GDI+ 렌더링 (PNG)
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

	// 툴 모드에서는 카메라를 자유롭게 움직일 수 있도록 스크립트 부착
	mainCamera->GetOwner()->AddComponent<CCameraScript>();

	// [Multi-Window Initialization]
	// 씬 진입 시 타일 팔레트용 서브 윈도우 생성
    CreateTileWindow(mEngine->hInst, SW_SHOWDEFAULT); // nCmdShow에 임의의 값 투입

	mhToolDC = GetDC(mhToolWnd);

	// 더블 버퍼링용 백버퍼 생성
	mTileBackBuffer = new CTexture();
	mTileBackBuffer->CreateBackBuffer(mhToolDC);
}

void CToolScene::OnExit()
{
	CScene::OnExit();

	mainCamera->GetOwner()->RemoveComponent<CCameraScript>(eComponentType::Script);

	// 툴 윈도우 리소스 정리
	if (mTileBackBuffer != nullptr) {
	    mTileBackBuffer->UnLoad();
	    SAFE_DELETE(mTileBackBuffer);
	}

	ReleaseDC(mhToolWnd, mhToolDC);

    DestroyWindow(mhToolWnd);	// 윈도우 파괴
}

// [Serialization] 맵 데이터 저장
void CToolScene::Save()
{
	// Windows Common Dialog (파일 저장 창) 호출
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

	// 바이너리 모드로 파일 쓰기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"wb");

	// 배치된 모든 타일 순회하며 정보 저장
	for (CTile* t : mTiles) {
		CTilemapRenderer* tmr = t->GetComponent<CTilemapRenderer>(eComponentType::TileRenderer);
		CTransform* tr = t->GetComponent<CTransform>(eComponentType::Transform);

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

// [Serialization] 맵 데이터 로드
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

// [Sub-Window Creation] 툴 윈도우 등록 및 생성
BOOL CToolScene::CreateTileWindow(HINSTANCE hInstance, int nCmdShow)
{
	// 윈도우 클래스 등록 (WndTileProc 프로시저 연결)
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

	// 윈도우 생성
	mhToolWnd = CreateWindowW(L"TILEWINDOW", L"TILE", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        windowWidth, windowHeight,
        nullptr, nullptr, hInstance, nullptr);

    if (!mhToolWnd)
    {
        return FALSE;
    }

	// 클라이언트 영역 크기 조정 (타일셋 이미지 크기에 맞춤)
    RECT rect = { 0, 0, mMapTileTexture->GetWidth(), mMapTileTexture->GetHeight() };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// 메인 윈도우 옆에 위치하도록 설정
    SetWindowPos(mhToolWnd, nullptr,
		windowWidth, 0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0);

    ShowWindow(mhToolWnd, nCmdShow);
    UpdateWindow(mhToolWnd);

    return TRUE;
}

// [Tool Window Procedure] 팔레트 윈도우 메시지 처리기
LRESULT CALLBACK CToolScene::WndTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
		// [Palette Picking] 팔레트에서 타일 선택
		CInputMgr::GetInst()->Update(hWnd);	// 해당 윈도우 기준으로 입력 업데이트

		if (CInputMgr::GetInst()->GetKeyDown("MouseLeftClick")) {
			SVector2D pos = CInputMgr::GetMousePosition();

			// 클릭한 위치를 타일 크기로 나누어 인덱스 계산
			SVector2D index;
			index = pos / CTilemapRenderer::TileSize;

			index.mX = (int)index.mX;
			index.mY = (int)index.mY;

			// 현재 그리기 도구(브러쉬)로 선택된 인덱스 업데이트
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
