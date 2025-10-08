#include "CPlayScene.h"
#include "CTitleScene.h"

#include "CPlayer.h"
#include "CCat.h"
#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"

#include "CAnimator.h"
#include "CCamera.h"
#include "CPlayerScript.h"
#include "CCatScript.h"

#include "CRenderer.h"
#include "Enums.h"
#include "Object.h"

void CPlayScene::OnCreate(CAPIEngine* tEngine)
{
	CInputMgr::GetInst()->AddKeyInfo("DoMoveLt", 'A');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveRt", 'D');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveFt", 'W');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveBt", 'S');

	CScene::OnCreate(tEngine);

	GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(336.0f, 423.0f));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;

	mPlayer = Instantiate<CPlayer>(tEngine, eLayerType::Player, SVector2D(336.0f, 423.0f));
	mPlayer->AddComponent<CPlayerScript>();

	CTexture* plImg = CResourceMgr::Find<CTexture>(L"Cat");

	CAnimator* anim = mPlayer->AddComponent<CAnimator>();
	anim->CreateAnimation(L"UpWalk", plImg, SVector2D(0.0f, 0.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	anim->CreateAnimation(L"RightWalk", plImg, SVector2D(0.0f, 32.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	anim->CreateAnimation(L"DownWalk", plImg, SVector2D(0.0f, 64.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	anim->CreateAnimation(L"LeftWalk", plImg, SVector2D(0.0f, 96.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	anim->CreateAnimation(L"SitDown", plImg, SVector2D(0.0f, 128.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	anim->CreateAnimation(L"Grooming", plImg, SVector2D(0.0f, 160.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);

	anim->PlayAnimation(L"SitDown", false);

	// CAT
	CCat* Cat = Instantiate<CCat>(tEngine, eLayerType::Animal, SVector2D(200.0f, 200.0f));
	Cat->AddComponent<CCatScript>();

	CTexture* catImg = CResourceMgr::Find<CTexture>(L"Cat");

	CAnimator* catAnim = Cat->AddComponent<CAnimator>();

	catAnim->CreateAnimation(L"UpWalk", catImg, SVector2D(0.0f, 0.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"RightWalk", catImg, SVector2D(0.0f, 32.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"DownWalk", catImg, SVector2D(0.0f, 64.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"LeftWalk", catImg, SVector2D(0.0f, 96.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"SitDown", catImg, SVector2D(0.0f, 128.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"Grooming", catImg, SVector2D(0.0f, 160.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);
	catAnim->CreateAnimation(L"LayDown", plImg, SVector2D(0.0f, 192.0f), SVector2D(32.0f, 32.0f), SVector2D(0.0f, 0.0f), 4, 0.5f);

	catAnim->PlayAnimation(L"SitDown", false);

	Cat->GetComponent<CTransform>()->SetScale(SVector2D(3.0f, 3.0f));

	/*GameObject* bg;
	bg = Instantiate<GameObject>(tEngine, eLayerType::BackGround, SVector2D(0.0f, 0.0f));

	CSpriteRenderer* bgSr = bg->GetComponent<CSpriteRenderer>();

	CTexture* bgImg = CResourceMgr::Find<CTexture>(L"BG");
	bgSr->SetTexture(bgImg);
	bgSr->SetName(L"SR");
	bgSr->SetSize(SVector2D(3.0f, 3.0f));*/
}

void CPlayScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CPlayScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CPlayScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);

	/*if (CInputMgr::GetInst()->GetKeyDown("ChangeScene")) {
		CSceneMgr::LoadScene(L"TitleScene");
	}*/
}

void CPlayScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

	/*wchar_t str[50] = L"Play Scene";
	TextOut(tHDC, 0, 0, str, 10);*/
}

void CPlayScene::OnEnter()
{
}

void CPlayScene::OnExit()
{
}