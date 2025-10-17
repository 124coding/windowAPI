#include "CPlayScene.h"
#include "CTitleScene.h"

#include "CPlayer.h"
#include "CCat.h"
#include "CTile.h"

#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"

#include "CBoxCollider2D.h"
#include "CCircleCollider2D.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CPlayerScript.h"
#include "CCatScript.h"
#include "CTilemapRenderer.h"

#include "CRenderer.h"
#include "Enums.h"
#include "Object.h"

void CPlayScene::OnCreate(CAPIEngine* tEngine)
{

	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Animal, true);

	CScene::OnCreate(tEngine);

	LoadMap(tEngine, L"..\\resources\\Maps\\Here");

	/*GameObject* camera = Instantiate<GameObject>(tEngine, eLayerType::None, SVector2D(336.0f, 423.0f));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;*/

	mPlayer = Instantiate<CPlayer>(tEngine, eLayerType::Player, SVector2D(100.0f, 100.0f));
	DontDestroyOnLoad(mPlayer);

	mPlayer->AddComponent<CPlayerScript>();

	CCircleCollider2D* cPlCollider = mPlayer->AddComponent<CCircleCollider2D>();
	cPlCollider->SetOffset(SVector2D(-50.0f, -50.0f));

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
