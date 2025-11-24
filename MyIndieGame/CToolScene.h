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

	void Save();
	void Load();

	void SetMapTileTexture(const std::wstring& tTextureName) {
		mMapTileTexture = CResourceMgr::Find<CTexture>(tTextureName);
	}

	static CTexture* GetMapTileTexture() {
		return mMapTileTexture;
	}

private:
	static SVector2D mIndex;
	std::vector<CTile*> mTiles;

private:
	CAPIEngine* mEngine = nullptr;

	HDC mhToolDC = nullptr;
	HWND mhToolWnd = nullptr;

	static CTexture* mMapTileTexture;
	CTexture* mTileBackBuffer = nullptr;

private:
	BOOL                InitInstanceTileWIndow(HINSTANCE, int);
	static LRESULT CALLBACK	   WndTileProc(HWND, UINT, WPARAM, LPARAM);

	BOOL CreateTileWindow(HINSTANCE hInstance, int nCmdShow);
};

