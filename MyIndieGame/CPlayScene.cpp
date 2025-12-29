#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CToolScene.h"

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
#include "CEffectMgr.h"

#include "CBoxCollider2D.h"
#include "CCircleCollider2D.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CTilemapRenderer.h"
#include "CRigidbody.h"
#include "CAudioClip.h"
#include "CAudioListner.h"
#include "CAudioSource.h"
#include "CWeaponMgr.h"
#include "CItemMgr.h"

#include "CMeleeWeaponScript.h"
#include "CRangedWeaponScript.h"
#include "CBabyAlienScript.h"

#include "CUIHPBar.h"
#include "CUIEXPBar.h"

#include "CRenderer.h"
#include "Enums.h"
#include "Object.h"

CPlayer* CPlayScene::mPlayer = nullptr;
int CPlayScene::mStageNum = 0;

void CPlayScene::OnCreate()
{

	GameObject* camera = Instantiate<GameObject>(eLayerType::None, SVector2D(windowWidth / 2, windowHeight / 2));
	CCamera* cameraComp = camera->AddComponent<CCamera>();

	mainCamera = cameraComp;

	DontDestroyOnLoad(camera);

	CEffectMgr::OnCreate();


	/*GameObject* mapOutLine = Instantiate<GameObject>(eLayerType::None);

	CTexture* mapOLImg = CResourceMgr::Find<CTexture>(L"TileOutLine");
	
	mapOutLine->SetSize(SVector2D(mapWidth / mapOLImg->GetWidth(), mapHeight / mapOLImg->GetHeight()));

	CSpriteRenderer* mapOLSr = mapOutLine->AddComponent<CSpriteRenderer>();
	mapOLSr->SetTexture(mapOLImg);*/



	mBakedMap = Instantiate<GameObject>(eLayerType::BackGround);
	mBakedMap->AddComponent<CSpriteRenderer>();

	mPlayer = Instantiate<CPlayer>(eLayerType::Player, SVector2D(mapWidth / 2, mapHeight / 2 + 55.0f));
	// DontDestroyOnLoad(mPlayer);

	// mPlayer->AddComponent<CRigidbody>();
	// mPlayer->AddComponent<CAudioListner>();

	CCircleCollider2D* cPlCollider = mPlayer->AddComponent<CCircleCollider2D>();
	cPlCollider->SetSize(SVector2D(0.40f, 0.40f));
	cPlCollider->SetOffset(SVector2D(0.0f, -35.0f));

	CTexture* plImg = CResourceMgr::Find<CTexture>(L"PlayerBase");

	CTransform* plTr = mPlayer->GetComponent<CTransform>();
	mPlayer->SetSize(SVector2D(0.8f, 0.8f)); 
	mPlayer->SetAnchorPoint(plImg->GetWidth() / 2, plImg->GetHeight());

	CAnimator* plAnim = mPlayer->AddComponent<CAnimator>();

	CPlayerScript* plSc = mPlayer->GetComponent<CPlayerScript>();
	
	plSc->SetBaseTexture(plImg);

	mPlayer->AddComponent<CSpriteRenderer>();

	// Player 외모 변화 체크
	/*CTexture* rangerEyesImg = CResourceMgr::Find<CTexture>(L"RangerEyes");
	CTexture* rangerMouthImg = CResourceMgr::Find<CTexture>(L"RangerMouth");

	plSc->SetEyesTexture(rangerEyesImg);
	plSc->SetMouthTexture(rangerMouthImg);

	plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), rangerEyesImg->GetImage());
	plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), rangerMouthImg->GetImage());*/

	mPlayer->AddComponent<CWeaponMgr>();
	mPlayer->AddComponent<CItemMgr>();


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

	/*CWeapon* weapon = Instantiate<CWeapon>(eLayerType::RangedWeapon, SVector2D(plTr->GetPos().mX - 10.0f, plTr->GetPos().mY));

	CTransform* wpTr = weapon->GetComponent<CTransform>();
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	weapon->AddComponent<CRangedWeaponScript>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(L"Pistol");
	weapon->SetSize(SVector2D(0.15f, 0.15f));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 100.0f, wpImg->GetHeight() / 2);

	CRangedWeaponScript* wpScript = weapon->GetComponent<CRangedWeaponScript>();
	wpScript->SetPlayer(mPlayer);*/



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

	CScene::OnCreate();
}

void CPlayScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CPlayScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
	CMonsterSpawnMgr::MonsterSpawnEvent(mPlayer);

	if (mStatUpgrading) return;

	// (WaitTime 어떻게 굴릴 지랑 체력 없을 때랑 시간 다 되었을 때에 대해서)수정 필요
	if (mPlayer->GetComponent<CPlayerScript>()->GetHP() <= 0) {
		if (CSceneMgr::GetGameObjects(eLayerType::Enemy).size() > 0) {
			for (auto& enemy : CSceneMgr::GetGameObjects(eLayerType::Enemy)) {
				enemy->GetComponent<CEnemyScript>()->SetHP(0);
			}
		}
		else if (mWaitTime <= 0.0f) {
			mWaitTime = 2.0f;
			GameEnd();
		}

		mWaitTime -= tDeltaTime;
	}
	else {
		if (CMonsterSpawnMgr::GetTime() > 0) {
			CMonsterSpawnMgr::MinusTime(tDeltaTime);
		}
		else {
			if (CSceneMgr::GetGameObjects(eLayerType::Enemy).size() > 0) {
				for (auto& enemy : CSceneMgr::GetGameObjects(eLayerType::Enemy)) {
					enemy->GetComponent<CEnemyScript>()->SetHP(0);
				}
			}
			else if (mWaitTime <= 0.0f) {
				mWaitTime = 2.0f;
				if (mStageNum == 4) {
					GameEnd();
					return;
				}
				if (mPlayer->GetComponent<CPlayerScript>()->GetCurStageLevelUpCount() > 0) {
					CUIMgr::Push(eUIType::LevelUpUI);
					mStatUpgrading = true;
				}
				else {
					StagePass();
				}
			}

			mWaitTime -= tDeltaTime;
		}
	}

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
	// LoadBakedMap(L"..\\resources\\Maps\\Here");
	mStatUpgrading = false;
	RandomBakedMap();

	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Enemy, true);
	CCollisionMgr::CollisionLayerCheck(eLayerType::Player, eLayerType::Material, true);
	CCollisionMgr::CollisionLayerCheck(eLayerType::MeleeWeapon, eLayerType::Enemy, true);
	CCollisionMgr::CollisionLayerCheck(eLayerType::Bullet, eLayerType::Enemy, true);

	mPlayer->SetState(true);

	CTransform* plTr = mPlayer->GetComponent<CTransform>();
	CSpriteRenderer* plSr = mPlayer->GetComponent<CSpriteRenderer>();
	CPlayerScript* plSc = mPlayer->GetComponent<CPlayerScript>();
	plSc->SetHP(plSc->GetMaxHP());
	plSc->SetState(true);
	plSr->SetGdiplusDraw(false);

	plTr->SetPos(SVector2D(mapWidth / 2, mapHeight / 2 + 55.0f));
	plTr->SetScale(SVector2D(1.0f, 1.0f));
	mPlayer->SetAnchorPoint(CResourceMgr::Find<CTexture>(L"PlayerBase")->GetWidth() / 2, CResourceMgr::Find<CTexture>(L"PlayerBase")->GetHeight());

	plSr->SetTexture(plSc->GetBaseTexture()->Clone());
	CTexture* plImg = plSr->GetTexture();

	if (plSc->GetClothTexture() != nullptr) {
		plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), plSc->GetClothTexture()->GetImage());
	}
	if (plSc->GetMouthTexture() != nullptr) {
		plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), plSc->GetMouthTexture()->GetImage());
	}
	if (plSc->GetEyesTexture() != nullptr) {
		plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), plSc->GetEyesTexture()->GetImage());
	}
	if (plSc->GetHairTexture() != nullptr) {
		plImg->BakedTex(0.0f, 0.0f, plImg->GetWidth(), plImg->GetHeight(), plSc->GetHairTexture()->GetImage());
	}

	if (plSr != nullptr && plSr->GetTexture() != nullptr) {
		plSr->GetTexture()->CreateHBitmapFromGdiPlus(false);
	}

	CWeaponMgr* plWeaponMgr = mPlayer->GetComponent<CWeaponMgr>();
	for (auto& weapon : plWeaponMgr->GetWeapons()) {
		weapon->SetState(true);
	}

	plWeaponMgr->WeaponsPosition();

	CUIMgr::Push(eUIType::PlaySceneUI);

	CMonsterSpawnMgr::LoadStageSpawnEvents(mStageNum);

	mainCamera->SetTarget(mPlayer);

	CScene::OnEnter();
}

void CPlayScene::OnExit()
{
	CScene::OnExit();

	if (mPlayer != nullptr)
	{
		CSpriteRenderer* plSr = mPlayer->GetComponent<CSpriteRenderer>();
		if (plSr != nullptr)
		{
			CTexture* pTex = plSr->GetTexture();

			if (pTex != nullptr)
			{
				SAFE_DELETE(pTex);
			}
		}
	}

	mStageNum++;

	for (auto& material : CSceneMgr::GetGameObjects(eLayerType::Material)) {
		ObjDestroy(material);
	}

	CMonsterSpawnMgr::DestroyStageSpawnEvents();
	CUIMgr::Pop(eUIType::PlaySceneUI);
	CUIMgr::Pop(eUIType::LevelUpUI);
}

void CPlayScene::StagePass()
{
	mPlayer->GetComponent<CTransform>()->SetPos(SVector2D(windowWidth / 2, windowHeight / 2 + 55.0f));
	CSceneMgr::LoadScene(L"ShopScene");
}

void CPlayScene::GameEnd()
{
	mPlayer->GetComponent<CTransform>()->SetPos(SVector2D(windowWidth / 2, windowHeight / 2 + 55.0f));
	CSceneMgr::LoadScene(L"EndingScene");
}

void CPlayScene::LoadBakedMap(const wchar_t* tPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, tPath, L"rb");

	CSpriteRenderer* mBakedMapSr = mBakedMap->AddComponent<CSpriteRenderer>();
	CTexture* mBakedMapImg = CTexture::Create(L"BakedBG", mapWidth, mapHeight);

	mBakedMapSr->SetTexture(mBakedMapImg);

	Gdiplus::Graphics graphics(mBakedMapImg->GetImage());

	CTexture* tileTex = CToolScene::GetMapTileTexture();
	int tileW = tileSizeX;
	int tileH = tileSizeY;

	while (true) {
		int idxX = 0;
		int idxY = 0;

		int posX = 0;
		int posY = 0;

		if (fread(&idxX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&idxY, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posY, sizeof(int), 1, pFile) == NULL) break;

		//CTile* tile = Instantiate<CTile>(eLayerType::Tile, SVector2D(posX, posY));
		//tile->SetSize(SVector2D(1.0f, 1.0f));

		//CTilemapRenderer* tmr = tile->AddComponent<CTilemapRenderer>();
		//tmr->SetTexture(CToolScene::GetMapTileTexture());
		//tmr->SetIndex(SVector2D(idxX, idxY));

		int srcX = idxX * tileW;
		int srcY = idxY * tileH;

		Gdiplus::Rect destRect(posX, posY, tileW, tileH);

		graphics.DrawImage(tileTex->GetImage(),
			destRect,
			srcX, srcY, tileW, tileH,
			Gdiplus::UnitPixel);
	}

	fclose(pFile);

	OutLineFill(&graphics, tileW, tileH);

	mBakedMapImg->CreateHBitmapFromGdiPlus(false);
}

void CPlayScene::RandomBakedMap()
{
	// 랜덤하게 타일 리소스를 가져옴
	int randTile = std::rand() % 5 + 1;
	std::wstring tileName = L"Tile" + std::to_wstring(randTile);
	CTexture* randomMapTex = CResourceMgr::Find<CTexture>(tileName);

	CSpriteRenderer* mBakedMapSr = mBakedMap->GetComponent<CSpriteRenderer>();
	CTexture* mBakedMapImg = CTexture::Create(L"BakedBG", mapWidth, mapHeight);

	mBakedMapSr->SetTexture(mBakedMapImg);

	Gdiplus::Graphics graphics(mBakedMapImg->GetImage());

	int tileW = tileSizeX;
	int tileH = tileSizeY;

	int tileCountWidth = mapWidth / tileSizeX;
	int tileCountHeight = mapHeight / tileSizeY;

	int tileMapWidth = randomMapTex->GetWidth() / tileSizeX;
	int tileMapHeight = randomMapTex->GetHeight() / tileSizeY;

	int srcX = 0;
	int srcY = 0;

	// 타일 채우는 for문
	for (int i = 0; i < tileCountWidth; i++) {
		for (int j = 0; j < tileCountHeight; j++) {
			int randNum = std::rand() % 10;

			// 90프로 확률로 빈 타일이 나오게 설정
			if (randNum > 0) {
				srcX = (tileMapWidth - 1) * tileW;
				srcY = (tileMapHeight - 1) * tileH;
			}
			else {
				int randX = 0;
				int randY = 0;
				
				while (true) {
					randX = std::rand() % tileMapWidth;
					randY = std::rand() % tileMapHeight;

					if (randX != tileMapWidth - 1 && randY != tileMapHeight - 1) {
						srcX = randX * tileW;
						srcY = randY * tileH;
						break;
					}
				}
			}
			graphics.DrawImage(randomMapTex->GetImage(),
				Gdiplus::Rect(i * tileW, j * tileH, tileW, tileH),
				srcX, srcY, tileW, tileH,
				Gdiplus::UnitPixel);
		}
	}

	OutLineFill(&graphics, tileW, tileH);

	mBakedMapImg->CreateHBitmapFromGdiPlus(false);
}

void CPlayScene::OutLineFill(Gdiplus::Graphics* tGraphics, int tTileW, int tTileH)
{
	CTexture* outlineTex = CResourceMgr::Find<CTexture>(L"TileOutLine"); // 아웃라인 텍스처 가져오기

	int tileCountWidth = mapWidth / tileSizeX;
	int tileCountHeight = mapHeight / tileSizeY;

	int countOutLineXNotCorner = outlineTex->GetWidth() / tileSizeX - 2;
	int countOutLineYNotCorner = outlineTex->GetHeight() / tileSizeY - 2;

	// 왼쪽 오른쪽 위 모서리와 가장 위 OutLine 채우기
	tGraphics->DrawImage(outlineTex->GetImage(),
		Gdiplus::Rect(0, 0, tTileW, tTileH),
		0, 0, tTileW, tTileH,
		Gdiplus::UnitPixel);

	tGraphics->DrawImage(outlineTex->GetImage(),
		Gdiplus::Rect((tileCountWidth - 1) * tTileW, 0, tTileW, tTileH),
		(outlineTex->GetWidth() / tileSizeX - 1) * tTileW, 0, tTileW, tTileH,
		Gdiplus::UnitPixel);

	for (int i = 1; i < tileCountWidth - 1; i++) {
		tGraphics->DrawImage(outlineTex->GetImage(),
			Gdiplus::Rect(i * tTileW, 0, tTileW, tTileH),
			(i % countOutLineXNotCorner + 1) * tTileW, 0, tTileW, tTileH,
			Gdiplus::UnitPixel);
	}


	// 왼쪽 오른쪽 아래 모서리와 가장 아래 OutLine 채우기

	tGraphics->DrawImage(outlineTex->GetImage(),
		Gdiplus::Rect(0, (tileCountHeight - 1) * tTileH, tTileW, tTileH),
		0, (outlineTex->GetHeight() / tileSizeY - 1) * tTileH, tTileW, tTileH,
		Gdiplus::UnitPixel);

	tGraphics->DrawImage(outlineTex->GetImage(),
		Gdiplus::Rect((tileCountWidth - 1) * tTileW, (tileCountHeight - 1) * tTileH, tTileW, tTileH),
		(outlineTex->GetWidth() / tileSizeX - 1) * tTileW, (outlineTex->GetHeight() / tileSizeY - 1) * tTileH, tTileW, tTileH,
		Gdiplus::UnitPixel);

	for (int i = 1; i < tileCountWidth - 1; i++) {
		tGraphics->DrawImage(outlineTex->GetImage(),
			Gdiplus::Rect(i * tTileW, (tileCountHeight - 1) * tTileH, tTileW, tTileH),
			(i % countOutLineXNotCorner + 1) * tTileW, (outlineTex->GetHeight() / tileSizeY - 1) * tTileH, tTileW, tTileH,
			Gdiplus::UnitPixel);
	}

	// 왼쪽 모서리 OutLine 채우기
	for (int i = 1; i < tileCountHeight - 1; i++) {
		tGraphics->DrawImage(outlineTex->GetImage(),
			Gdiplus::Rect(0, i * tTileH, tTileW, tTileH),
			0, (i % countOutLineYNotCorner + 1) * tTileH, tTileW, tTileH,
			Gdiplus::UnitPixel);
	}

	// 오른쪽 모서리 OutLine 채우기
	for (int i = 1; i < tileCountHeight - 1; i++) {
		tGraphics->DrawImage(outlineTex->GetImage(),
			Gdiplus::Rect((tileCountWidth - 1) * tTileW, i * tTileH, tTileW, tTileH),
			(outlineTex->GetWidth() / tileSizeX - 1) * tTileW, (i % countOutLineYNotCorner + 1) * tTileH, tTileW, tTileH,
			Gdiplus::UnitPixel);
	}
}
