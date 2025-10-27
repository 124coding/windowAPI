#include "CPlayScene.h"
#include "CTitleScene.h"

#include "CPlayer.h"
#include "CBabyAlien.h"
#include "CCat.h"
#include "CTile.h"
#include "CFloor.h"

#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"

#include "CBoxCollider2D.h"
#include "CCircleCollider2D.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CTilemapRenderer.h"
#include "CRigidbody.h"
#include "CAudioClip.h"
#include "CAudioListner.h"
#include "CAudioSource.h"

#include "CBabyAlienScript.h"

#include "CUIHPBar.h"
#include "CUIEXPBar.h"

#include "CRenderer.h"
#include "Enums.h"
#include "Object.h"

void CPlayScene::OnCreate(CAPIEngine* tEngine)
{
	LoadMap(tEngine, L"..\\resources\\Maps\\Here");

	GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None);
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;

	mPlayer = Instantiate<CPlayer>(tEngine, eLayerType::Player, SVector2D(windowWidth / 2, windowHeight / 2));
	// DontDestroyOnLoad(mPlayer);

	// mPlayer->AddComponent<CRigidbody>();
	mPlayer->AddComponent<CAudioListner>();

	CCircleCollider2D* cPlCollider = mPlayer->AddComponent<CCircleCollider2D>();

	cameraComp->SetTarget(mPlayer);

	CTexture* plImg = CResourceMgr::Find<CTexture>(L"Player");

	CTransform* plTr = mPlayer->GetComponent<CTransform>();
	mPlayer->SetAnchorPoint(250.0f, 250.0f, mPlayer->GetSize(), plTr->GetScale());

	CAnimator* plAnim = mPlayer->AddComponent<CAnimator>();
	plAnim->CreateAnimation(L"Idle", plImg, SVector2D(2000.0f, 250.0f), SVector2D(250.0f, 250.0f), SVector2D(), 1, 0.1f);
	plAnim->CreateAnimation(L"FrontGiveWater", plImg, SVector2D(0.0f, 2000.0f), SVector2D(250.0f, 250.0f), SVector2D(), 12, 0.1f);

	plAnim->PlayAnimation(L"Idle", false);

	// plAnim->GetCompleteEvent(L"FrontGiveWater");

	CBabyAlien* Enemy = Instantiate<CBabyAlien>(tEngine, eLayerType::Enemy, SVector2D(300.0f, 300.0f));

	CBabyAlienScript* EnemyScript = Enemy->GetComponent<CBabyAlienScript>();
	EnemyScript->SetTarget(mPlayer);
	
	CCircleCollider2D* cCatCollider = Enemy->AddComponent<CCircleCollider2D>();

	CTexture* babyAlienImg = CResourceMgr::Find<CTexture>(L"BabyAlien");

	CSpriteRenderer* enemySr = Enemy->AddComponent<CSpriteRenderer>();
	enemySr->SetTexture(babyAlienImg);

	CTransform* babyAlienTr = Enemy->GetComponent<CTransform>();

	Enemy->SetSize(SVector2D(0.1f, 0.1f));
	Enemy->SetAnchorPoint(babyAlienImg->GetWidth(), babyAlienImg->GetHeight(), babyAlienTr->GetScale(), Enemy->GetSize());



	/*CAnimator* EnemyAnim = Enemy->AddComponent<CAnimator>();

	EnemyAnim->CreateAnimationByFolder(tEngine, L"MushroomIdle", L"../resources/Sprites/Mushrooms", SVector2D(), 0.5f);
	EnemyAnim->PlayAnimation(L"MushroomIdle");*/

	CScene::OnCreate(tEngine);

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
	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);

	CUIMgr::Push(eUIType::HPBar);
	dynamic_cast<CUIHPBar*>(CUIMgr::FindUI(eUIType::HPBar))->SetPlayer(mPlayer);

	CUIMgr::Push(eUIType::EXPBar);
	dynamic_cast<CUIEXPBar*>(CUIMgr::FindUI(eUIType::EXPBar))->SetPlayer(mPlayer);

	CScene::OnEnter();

	CSceneMgr::SetDontDestroyOnLoad(true);
}

void CPlayScene::OnExit()
{
	CScene::OnExit();

	CUIMgr::Pop(eUIType::HPBar);
	CUIMgr::Pop(eUIType::EXPBar);
}

void CPlayScene::LoadMap(CAPIEngine* tEngine, const wchar_t* tPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, tPath, L"rb");

	while (true) {
		int idxX = 0;
		int idxY = 0;

		int posX = 0;
		int posY = 0;

		if (fread(&idxX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&idxY, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posY, sizeof(int), 1, pFile) == NULL) break;

		CTile* tile = Instantiate<CTile>(tEngine, eLayerType::Tile, SVector2D(posX, posY));
		tile->SetSize(SVector2D(2.0f, 2.0f));

		CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));
		tmr->SetIndex(SVector2D(idxX, idxY));
	}

	fclose(pFile);
}
