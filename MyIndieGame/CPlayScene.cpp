#include "CPlayScene.h"
#include "CTitleScene.h"

#include "CPlayer.h"
#include "CBabyAlien.h"
#include "CCat.h"
#include "CTile.h"
#include "CFloor.h"
#include "CWeapon.h"

#include "CLayer.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CMonsterSpawnMgr.h"

#include "CBoxCollider2D.h"
#include "CCircleCollider2D.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CTilemapRenderer.h"
#include "CRigidbody.h"
#include "CAudioClip.h"
#include "CAudioListner.h"
#include "CAudioSource.h"

#include "CMeleeWeaponScript.h"
#include "CRangedWeaponScript.h"
#include "CBabyAlienScript.h"

#include "CUIHPBar.h"
#include "CUIEXPBar.h"

#include "CRenderer.h"
#include "Enums.h"
#include "Object.h"

void CPlayScene::OnCreate(CAPIEngine* tEngine)
{
	// LoadMap(tEngine, L"..\\resources\\Maps\\Here");


	GameObject* camera = Instantiate<GameObject>(eLayerType::None);
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;








	mPlayer = Instantiate<CPlayer>(eLayerType::Player, SVector2D(windowWidth / 2, windowHeight / 2 + 55.0f));
	// DontDestroyOnLoad(mPlayer);

	// mPlayer->AddComponent<CRigidbody>();
	// mPlayer->AddComponent<CAudioListner>();

	CCircleCollider2D* cPlCollider = mPlayer->AddComponent<CCircleCollider2D>();
	cPlCollider->SetSize(SVector2D(0.40f, 0.40f));
	cPlCollider->SetOffset(SVector2D(0.0f, -45.0f));

	cameraComp->SetTarget(mPlayer);

	CTexture* plImg = CResourceMgr::Find<CTexture>(L"PlayerBase");

	CTransform* plTr = mPlayer->GetComponent<CTransform>();
	mPlayer->SetSize(SVector2D(0.20f, 0.20f)); 
	mPlayer->SetAnchorPoint(plImg->GetWidth() / 2, plImg->GetHeight());

	CAnimator* plAnim = mPlayer->AddComponent<CAnimator>();
	CSpriteRenderer* plSr = mPlayer->AddComponent<CSpriteRenderer>();
	plSr->SetTexture(plImg);







	/*CWeapon* weapon = Instantiate<CWeapon>(eLayerType::MeleeWeapon, SVector2D(plTr->GetPos().mX - 10.0f, plTr->GetPos().mY));

	CTransform* wpTr = weapon->GetComponent<CTransform>();
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	weapon->AddComponent<CMeleeWeaponScript>();
	
	CBoxCollider2D* wpCl = weapon->AddComponent<CBoxCollider2D>();
	wpCl->SetSize(SVector2D(0.6f, 0.5f));
	wpCl->SetActivate(false);

	CTexture* wpImg = CResourceMgr::Find<CTexture>(L"Dagger");
	weapon->SetSize(SVector2D(0.20f, 0.20f));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 100.0f, wpImg->GetHeight() / 2);

	CMeleeWeaponScript* wpScript = weapon->GetComponent<CMeleeWeaponScript>();
	wpScript->SetPlayer(mPlayer);*/

	CWeapon* weapon = Instantiate<CWeapon>(eLayerType::RangedWeapon, SVector2D(plTr->GetPos().mX - 10.0f, plTr->GetPos().mY));

	CTransform* wpTr = weapon->GetComponent<CTransform>();
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	weapon->AddComponent<CRangedWeaponScript>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(L"Pistol");
	weapon->SetSize(SVector2D(0.15f, 0.15f));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 100.0f, wpImg->GetHeight() / 2);

	CRangedWeaponScript* wpScript = weapon->GetComponent<CRangedWeaponScript>();
	wpScript->SetPlayer(mPlayer);



	// CMonsterSpawnMgr::MonsterSpawn<CBabyAlien>("M_001", mPlayer);


	/*CBabyAlien* Enemy = Instantiate<CBabyAlien>(eLayerType::Enemy, SVector2D(300.0f, 300.0f));

	CBabyAlienScript* EnemyScript = Enemy->GetComponent<CBabyAlienScript>();
	EnemyScript->SetTarget(mPlayer);
	
	Enemy->AddComponent<CCircleCollider2D>();

	CTexture* babyAlienImg = CResourceMgr::Find<CTexture>(L"BabyAlien");

	CSpriteRenderer* enemySr = Enemy->AddComponent<CSpriteRenderer>();
	enemySr->SetTexture(babyAlienImg);

	CTransform* babyAlienTr = Enemy->GetComponent<CTransform>();

	Enemy->SetSize(SVector2D(0.05f, 0.05f));
	Enemy->SetAnchorPoint(babyAlienImg->GetWidth() / 2, babyAlienImg->GetHeight());*/



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
	CMonsterSpawnMgr::MonsterSpawnEvent(tDeltaTime, mPlayer);
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
	mStageNum++;
	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);
	CCollisionMgr::CollisionLayerCheck(eLayerType::MeleeWeapon, eLayerType::Enemy, true);
	CCollisionMgr::CollisionLayerCheck(eLayerType::Bullet, eLayerType::Enemy, true);

	CUIMgr::Push(eUIType::HPBar);
	dynamic_cast<CUIHPBar*>(CUIMgr::FindUI(eUIType::HPBar))->SetPlayer(mPlayer);

	CUIMgr::Push(eUIType::EXPBar);
	dynamic_cast<CUIEXPBar*>(CUIMgr::FindUI(eUIType::EXPBar))->SetPlayer(mPlayer);

	CMonsterSpawnMgr::LoadStageSpawnEvents(mStageNum);

	CScene::OnEnter();

	CSceneMgr::SetDontDestroyOnLoad(true);
}

void CPlayScene::OnExit()
{
	CScene::OnExit();

	CMonsterSpawnMgr::DestroyStageSpawnEvents();
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

		CTile* tile = Instantiate<CTile>(eLayerType::Tile, SVector2D(posX, posY));
		tile->SetSize(SVector2D(2.0f, 2.0f));

		CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		tmr->SetTexture(CResourceMgr::Find<CTexture>(L"SpringFloor"));
		tmr->SetIndex(SVector2D(idxX, idxY));
	}

	fclose(pFile);
}
