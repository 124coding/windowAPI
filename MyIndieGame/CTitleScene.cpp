#include "CTitleScene.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CPlayScene.h"

void CTitleScene::OnCreate(CAPIEngine* tEngine)
{
	CScene::OnCreate(tEngine);
}

void CTitleScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CTitleScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CTitleScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);

	if (CInputMgr::GetInst()->GetKeyDown("ChangeScene")) {
		CSceneMgr::LoadScene(L"PlayScene");
	}
}

void CTitleScene::Render(HDC hDC)
{
	CScene::Render(hDC);
	wchar_t str[50] = L"Title Scene";
	TextOut(hDC, 0, 0, str, 10);
}

void CTitleScene::OnEnter()
{
}

void CTitleScene::OnExit()
{
}
