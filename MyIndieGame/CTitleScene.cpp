#include "CTitleScene.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"

#include "CPlayScene.h"

#include"CTitleObjectScript.h"

#include "Object.h"

void CTitleScene::OnCreate(CAPIEngine* tEngine)
{
	CScene::OnCreate(tEngine);

	GameObject* titleMap = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	float aspectRatioX = 0.0f;
	float aspectRatioY = 0.0f;

	CTexture* titleMapImg = CResourceMgr::Find<CTexture>(L"TitleMap");
	titleMap->AddComponent<CSpriteRenderer>();

	CTransform* tlMapTr = titleMap->GetComponent<CTransform>();

	aspectRatioX = (float)windowWidth / 1920;
	aspectRatioY = (float)windowHeight / 1080;

	tlMapTr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	tlMapTr->SetPos(SVector2D((float)(windowWidth) / 2, (float)windowHeight));

	CSpriteRenderer* tlMapSr = titleMap->GetComponent<CSpriteRenderer>();
	titleMap->SetAnchorPoint(titleMapImg, tlMapTr->GetScale(), titleMap->GetSize());
	tlMapSr->SetTexture(titleMapImg);


	GameObject* titleMob1 = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleMob1Img = CResourceMgr::Find<CTexture>(L"TitleMob1");
	titleMob1->AddComponent<CSpriteRenderer>();

	CTransform* tlMob1Tr = titleMob1->GetComponent<CTransform>();
	tlMob1Tr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	tlMob1Tr->SetPos(SVector2D((float)(windowWidth) / 2, (float)windowHeight));

	titleMob1->AddComponent<CTitleObjectScript>();

	CTitleObjectScript* titleMob1Script = titleMob1->GetComponent<CTitleObjectScript>();
	titleMob1Script->SetSwing(1.0f);

	CSpriteRenderer* tlMob1Sr = titleMob1->GetComponent<CSpriteRenderer>();
	titleMob1->SetAnchorPoint(titleMob1Img, tlMob1Tr->GetScale(), titleMob1->GetSize());
	tlMob1Sr->SetTexture(titleMob1Img);


	GameObject* titleMob3 = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleMob3Img = CResourceMgr::Find<CTexture>(L"TitleMob3");
	titleMob3->AddComponent<CSpriteRenderer>();

	CTransform* tlMob3Tr = titleMob3->GetComponent<CTransform>();
	tlMob3Tr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	tlMob3Tr->SetPos(SVector2D((float)(windowWidth) / 2, (float)windowHeight));


	CSpriteRenderer* tlMob3Sr = titleMob3->GetComponent<CSpriteRenderer>();
	titleMob3->SetAnchorPoint(titleMob3Img, tlMob3Tr->GetScale(), titleMob3->GetSize());
	tlMob3Sr->SetTexture(titleMob3Img);


	GameObject* titleMob2 = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleMob2Img = CResourceMgr::Find<CTexture>(L"TitleMob2");
	titleMob2->AddComponent<CSpriteRenderer>();

	CTransform* tlMob2Tr = titleMob2->GetComponent<CTransform>();
	tlMob2Tr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	tlMob2Tr->SetPos(SVector2D((float)(windowWidth) / 2, (float)windowHeight));

	titleMob2->AddComponent<CTitleObjectScript>();

	CTitleObjectScript* titleMob2Script = titleMob2->GetComponent<CTitleObjectScript>();
	titleMob2Script->SetSwing(-1.0f);

	CSpriteRenderer* tlMob2Sr = titleMob2->GetComponent<CSpriteRenderer>();
	titleMob2->SetAnchorPoint(titleMob2Img, tlMob2Tr->GetScale(), titleMob2->GetSize());
	tlMob2Sr->SetTexture(titleMob2Img);


	GameObject* titleBrotato = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleBrotatoImg = CResourceMgr::Find<CTexture>(L"TitleBrotato");
	titleBrotato->AddComponent<CSpriteRenderer>();

	CTransform* titleBrotatoTr = titleBrotato->GetComponent<CTransform>();

	aspectRatioX = (float)windowWidth / titleBrotatoImg->GetWidth();
	aspectRatioY = (float)windowHeight / titleBrotatoImg->GetHeight();

	titleBrotatoTr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	titleBrotatoTr->SetPos(SVector2D(windowWidth / 2, 5 * windowHeight / 6));

	CSpriteRenderer* titleBrotatoSr = titleBrotato->GetComponent<CSpriteRenderer>();
	titleBrotato->SetSize(SVector2D(0.3f, 0.3f));
	titleBrotato->SetAnchorPoint(titleBrotatoImg, titleBrotatoTr->GetScale(), titleBrotato->GetSize());
	titleBrotatoSr->SetTexture(titleBrotatoImg);
	

	GameObject* titleLogo = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleLogoImg = CResourceMgr::Find<CTexture>(L"TitleLogo");
	titleLogo->AddComponent<CSpriteRenderer>();

	CTransform* titleLogoTr = titleLogo->GetComponent<CTransform>();

	aspectRatioX = (float)windowWidth / titleLogoImg->GetWidth();
	aspectRatioY = (float)windowHeight / titleLogoImg->GetHeight();

	titleLogoTr->SetScale(SVector2D(aspectRatioX, aspectRatioY));
	titleLogoTr->SetPos(SVector2D(windowWidth / 2, 2 * windowHeight / 5));

	CSpriteRenderer* titleLogoSr = titleLogo->GetComponent<CSpriteRenderer>();
	titleLogo->SetSize(SVector2D(0.5f, 0.3f));
	titleLogo->SetAnchorPoint(titleLogoImg, titleLogoTr->GetScale(), titleLogo->GetSize());
	titleLogoSr->SetTexture(titleLogoImg);

	// 최종적 : x = (titleLogoImg->GetWidth() * titleLogoTr->GetScale().mX * titleLogo->GetSize().mX), y 도 동일


	GameObject* titleLight = Instantiate<GameObject>(tEngine, eLayerType::BackGround);

	CTexture* titleLightImg = CResourceMgr::Find<CTexture>(L"TitleLight");
	titleLight->AddComponent<CSpriteRenderer>();

	CTransform* tlLightTr = titleLight->GetComponent<CTransform>();
	tlLightTr->SetScale(SVector2D((float)(windowWidth) / 1920, (float)windowHeight / 1080));
	tlLightTr->SetPos(SVector2D((float)(windowWidth) / 2, (float)windowHeight));

	CSpriteRenderer* tlLightSr = titleLight->GetComponent<CSpriteRenderer>();
	titleLight->SetAnchorPoint(titleLightImg, tlLightTr->GetScale(), titleLight->GetSize());
	tlLightSr->SetTexture(titleLightImg);
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
}

void CTitleScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);
}

void CTitleScene::OnEnter()
{
	CScene::OnEnter();
	CSceneMgr::SetDontDestroyOnLoad(false);
	CUIMgr::Push(eUIType::Button);
}

void CTitleScene::OnExit()
{
	CUIMgr::Pop(eUIType::Button);
	CScene::OnExit();
	CSceneMgr::SetDontDestroyOnLoad(true);
}
