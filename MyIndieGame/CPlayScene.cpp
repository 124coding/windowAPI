#include "CPlayScene.h"
#include "CTitleScene.h"

#include "CPlayer.h"
#include "CCat.h"

#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"

#include "CBoxCollider2D.h"
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
	CInputMgr::GetInst()->AddKeyInfo("MouseLeftClick", VK_LBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseRightClick", VK_RBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseWheelClick", VK_MBUTTON);

	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Animal, true);

	CScene::OnCreate(tEngine);

	/*GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(336.0f, 423.0f));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;*/

	mPlayer = Instantiate<CPlayer>(tEngine, eLayerType::Player, SVector2D(100.0f, 100.0f));
	mPlayer->AddComponent<CPlayerScript>();
	CBoxCollider2D* bPlCollider = mPlayer->AddComponent<CBoxCollider2D>();
	bPlCollider->SetOffset(SVector2D(-50.0f, -50.0f));

	// cameraComp->SetTarget(mPlayer);

	CTexture* plImg = CResourceMgr::Find<CTexture>(L"Player");

	CAnimator* plAnim = mPlayer->AddComponent<CAnimator>();
	plAnim->CreateAnimation(L"Idle", plImg, SVector2D(2000.0f, 250.0f), SVector2D(250.0f, 250.0f), SVector2D(), 1, 0.1f);
	plAnim->CreateAnimation(L"FrontGiveWater", plImg, SVector2D(0.0f, 2000.0f), SVector2D(250.0f, 250.0f), SVector2D(), 12, 0.1f);

	plAnim->PlayAnimation(L"Idle", false);

	// plAnim->GetCompleteEvent(L"FrontGiveWater");

	mPlayer->GetComponent<CTransform>()->SetScale(SVector2D(1.0f, 1.0f));

	CCat* Cat = Instantiate<CCat>(tEngine, eLayerType::Animal, SVector2D(300.0f, 300.0f));
	Cat->AddComponent<CCatScript>();
	CBoxCollider2D* bCatCollider = Cat->AddComponent<CBoxCollider2D>();
	bCatCollider->SetOffset(SVector2D(-50.0f, -50.0f));

	CTexture* catImg = CResourceMgr::Find<CTexture>(L"Cat");

	CAnimator* catAnim = Cat->AddComponent<CAnimator>();

	catAnim->CreateAnimationByFolder(tEngine, L"MushroomIdle", L"../resources/Sprites/Mushrooms", SVector2D(), 0.5f);
	catAnim->PlayAnimation(L"MushroomIdle");

	// CAT
	/*CCat* Cat = Instantiate<CCat>(tEngine, eLayerType::Animal, SVector2D(200.0f, 200.0f));
	Cat->AddComponent<CCatScript>();

	CTexture* catImg = CResourceMgr::Find<CTexture>(L"Cat");

	CAnimator* catAnim = Cat->AddComponent<CAnimator>();

	catAnim->CreateAnimation(L"UpWalk", catImg, SVector2D(0.0f, 0.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"RightWalk", catImg, SVector2D(0.0f, 32.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"DownWalk", catImg, SVector2D(0.0f, 64.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"LeftWalk", catImg, SVector2D(0.0f, 96.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"SitDown", catImg, SVector2D(0.0f, 128.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"Grooming", catImg, SVector2D(0.0f, 160.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);
	catAnim->CreateAnimation(L"LayDown", plImg, SVector2D(0.0f, 192.0f), SVector2D(32.0f, 32.0f), SVector2D(), 4, 0.5f);

	catAnim->PlayAnimation(L"SitDown", false);

	Cat->GetComponent<CTransform>()->SetScale(SVector2D(1.5f, 1.5f));*/
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

}

void CPlayScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

}

void CPlayScene::OnEnter()
{
}

void CPlayScene::OnExit()
{
}