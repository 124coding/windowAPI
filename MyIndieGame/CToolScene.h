#pragma once
#include "framework.h"

#include "CResourceMgr.h"
#include "CScene.h"

class CTile;

class CToolScene : public CScene
{
public:
	CToolScene() {}
	virtual ~CToolScene() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

	// ==========================================
	// Serialization (Data Persistence)
	// ==========================================
	// 배치된 타일 정보(인덱스, 속성 등)를 파일로 내보내거나 불러오는 기능
	void Save();
	void Load();

	// ==========================================
	// Tile Resource Management
	// ==========================================
	void SetMapTileTexture(const std::wstring& tTextureName) {
		mMapTileTexture = CResourceMgr::Find<CTexture>(tTextureName);
	}

	static CTexture* GetMapTileTexture() {
		return mMapTileTexture;
	}

private:
	// 현재 툴에서 '선택된' 타일 인덱스 (브러쉬 역할)
	// 모든 인스턴스가 공유해야 하므로 static으로 선언됨
	static SVector2D mIndex;

	// 배치된 타일 객체들의 리스트 (실제 맵 데이터)
	std::vector<CTile*> mTiles;

private:
	CAPIEngine* mEngine = nullptr;

	// ==========================================
	// Tool Window Handles (Sub-Window)
	// ==========================================
	// 메인 게임 윈도우가 아닌, 타일 선택창(Palette)을 위한 별도 핸들
	HDC mhToolDC = nullptr;
	HWND mhToolWnd = nullptr;

	// 타일셋 이미지와 이를 툴 윈도우에 그리기 위한 백버퍼
	static CTexture* mMapTileTexture;
	CTexture* mTileBackBuffer = nullptr;

private:
	// ==========================================
	// Native WinAPI Integration
	// ==========================================
	// 툴 전용 윈도우 클래스 등록 및 생성 함수들
	BOOL                InitInstanceTileWIndow(HINSTANCE, int);

	// 툴 윈도우 전용 메시지 처리기 (WndProc)
	// 타일 팔레트에서의 마우스 클릭(타일 선택) 이벤트를 처리함
	static LRESULT CALLBACK	   WndTileProc(HWND, UINT, WPARAM, LPARAM);

	BOOL CreateTileWindow(HINSTANCE hInstance, int nCmdShow);
};

