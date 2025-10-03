#include "CPlayScene.h"

#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"

#include "CTitleScene.h"

#include "Enums.h"
#include "Object.h"

void CPlayScene::OnCreate(CAPIEngine* tEngine)
{
	/*CInputMgr::GetInst()->AddKeyInfo("DoMoveLt", 'A');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveRt", 'D');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveFt", 'W');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveBt", 'S');*/

	CScene::OnCreate(tEngine);

	/*CPlayer* bg = new CPlayer();
	CTransform* tr = bg->AddComponent<CTransform>();
	tr->SetPos(SVector2D(0.0f, 0.0f));

	tr->SetName(L"TR");

	CSpriteRenderer* sr = bg->AddComponent<CSpriteRenderer>();
	sr->SetName(L"SR");
	sr->ImageLoad(L"../resources/blue_sky.jpg");

	AddGameObject(bg, eLayerType::BackGround);*/

	CPlayer* bg;
	bg = Instantiate<CPlayer>(tEngine, eLayerType::BackGround, SVector2D(100.0f, 100.0f), L"../resources/blue_sky.png");
}

void CPlayScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CPlayScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);

	/*SVector2D CurrentVelocity;

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveLt")) {
		CurrentVelocity.mX += -1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveRt")) {
		CurrentVelocity.mX += 1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveFt")) {
		CurrentVelocity.mY += -1.0f;
	}

	if (CInputMgr::GetInst()->GetKeyPressed("DoMoveBt")) {
		CurrentVelocity.mY += 1.0f;
	}

	if (CurrentVelocity.Length() > 0.0f) {
		CurrentVelocity.Normalize();
	}*/
}

void CPlayScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);

	if (CInputMgr::GetInst()->GetKeyDown("ChangeScene")) {
		CSceneMgr::LoadScene(L"TitleScene");
	}
}

void CPlayScene::Render(HDC hDC)
{
	CScene::Render(hDC);

	wchar_t str[50] = L"Play Scene";
	TextOut(hDC, 0, 0, str, 10);
}

void CPlayScene::OnEnter()
{
}

void CPlayScene::OnExit()
{
}
