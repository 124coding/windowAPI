#pragma once
#include "framework.h"

#include "CScene.h"

class CTile;

class CToolScene : public CScene
{
public:
	CToolScene() {}
	virtual ~CToolScene() {}

	void OnCreate(CAPIEngine* tEngine) override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

	void Save();
	void Load();

private:
	static SVector2D mIndex;
	std::vector<CTile*> mTiles;

private:
	CAPIEngine* mEngine = nullptr;

	HDC mhToolDC = nullptr;
	HWND mhToolWnd = nullptr;

	CTexture* mTileBackBuffer = nullptr;

private:
	BOOL                InitInstanceTileWIndow(HINSTANCE, int);
	static LRESULT CALLBACK	   WndTileProc(HWND, UINT, WPARAM, LPARAM);

	BOOL CreateTileWindow(HINSTANCE hInstance, int nCmdShow);
};

