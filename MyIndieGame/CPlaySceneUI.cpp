#include "CPlaySceneUI.h"
#include "CPlayScene.h"

#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CMonsterSpawnMgr.h"

#include "CUIPanel.h"
#include "CUIImg.h"
#include "CUIHUD.h"
#include "CUIText.h"

#include "CUIHPBar.h"
#include "CUIEXPBar.h"

void CPlaySceneUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);





	CUIPanel* hudPanel = new CUIPanel();
	hudPanel->SetWidth(330);
	hudPanel->SetHeight(140);

	hudPanel->SetPos(SVector2D(20.0f, 20.0f));

	this->AddChild(hudPanel);





	CUIHUD* hpHUD = new CUIHUD();
	hpHUD->SetPos(SVector2D());
	hpHUD->SetWidth(320);
	hpHUD->SetHeight(48);

	hudPanel->AddChild(hpHUD);

	CUIImg* hpBg = new CUIImg();

	hpBg->SetTexture(CResourceMgr::Find<CTexture>(L"UIHUDBg"));
	hpBg->SetWidth(hpHUD->GetWidth());
	hpBg->SetHeight(hpHUD->GetHeight());

	hpHUD->AddChild(hpBg);

	CUIHPBar* hpBar = new CUIHPBar();
	hpBar->SetWidth(hpHUD->GetWidth());
	hpBar->SetHeight(hpHUD->GetHeight());

	hpBar->SetPlayer(CPlayScene::GetPlayer());

	hpHUD->AddChild(hpBar);

	CUIImg* hpFrame = new CUIImg();

	hpFrame->SetTexture(CResourceMgr::Find<CTexture>(L"UIHUDFrame"));
	hpFrame->SetWidth(hpHUD->GetWidth());
	hpFrame->SetHeight(hpHUD->GetHeight());

	hpHUD->AddChild(hpFrame);

	



	CUIHUD* EXPHUD = new CUIHUD();
	EXPHUD->SetPos(SVector2D(0.0f, 53.0f));
	EXPHUD->SetWidth(320);
	EXPHUD->SetHeight(48);

	hudPanel->AddChild(EXPHUD);

	CUIImg* epxBg = new CUIImg();

	epxBg->SetTexture(CResourceMgr::Find<CTexture>(L"UIHUDBg"));
	epxBg->SetWidth(EXPHUD->GetWidth());
	epxBg->SetHeight(EXPHUD->GetHeight());

	EXPHUD->AddChild(epxBg);

	CUIEXPBar* expBar = new CUIEXPBar();
	expBar->SetWidth(EXPHUD->GetWidth());
	expBar->SetHeight(EXPHUD->GetHeight());

	expBar->SetPlayer(CPlayScene::GetPlayer());

	EXPHUD->AddChild(expBar);

	CUIImg* expFrame = new CUIImg();

	expFrame->SetTexture(CResourceMgr::Find<CTexture>(L"UIHUDFrame"));
	expFrame->SetWidth(EXPHUD->GetWidth());
	expFrame->SetHeight(EXPHUD->GetHeight());

	EXPHUD->AddChild(expFrame);

	CUIImg* moneyIcon = new CUIImg();
	moneyIcon->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	moneyIcon->SetPos(SVector2D(0.0f, 100.0f));
	moneyIcon->SetWidth(50.0f);
	moneyIcon->SetHeight(50.0f);
	moneyIcon->SetImageMode(CUIImg::eImageMode::KeepAspect);

	hudPanel->AddChild(moneyIcon);

	mMoneyTex = new CUIText();
	mMoneyTex->SetPos(SVector2D(55.0f, 100.0f));
	mMoneyTex->SetWidth(hudPanel->GetWidth() - moneyIcon->GetWidth() - 5.0f);
	mMoneyTex->SetHeight(50.0f);
	mMoneyTex->SetFontSize(30.0f);
	mMoneyTex->SetColor(Gdiplus::Color::White);
	mMoneyTex->SetStrokeWidth(1.0f);
	mMoneyTex->SetOutline(1.0f, Gdiplus::Color::Black);

	hudPanel->AddChild(mMoneyTex);


	CUIPanel* infoPanel = new CUIPanel();
	infoPanel->SetWidth(200.0f);
	infoPanel->SetHeight(100.0f);
	infoPanel->SetPos(SVector2D(this->GetWidth() / 2 - infoPanel->GetWidth() / 2, 20.0f));

	this->AddChild(infoPanel);

	CUIText* stageTex = new CUIText();
	stageTex->SetText(L"¿þÀÌºê ");
	stageTex->SetWidth(2 * infoPanel->GetWidth() / 3 - 20.0f);
	stageTex->SetHeight(50.0f);
	stageTex->SetPos(SVector2D());
	stageTex->SetColor(Gdiplus::Color::White);
	stageTex->SetFontSize(30.0f);
	stageTex->SetStrokeWidth(1.0f);
	stageTex->SetOutline(2.0f, Gdiplus::Color::Black);
	stageTex->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentCenter);

	infoPanel->AddChild(stageTex);

	mStageNumTex = new CUIText();
	mStageNumTex->SetWidth(infoPanel->GetWidth() - stageTex->GetWidth());
	mStageNumTex->SetHeight(stageTex->GetHeight());
	mStageNumTex->SetPos(SVector2D(stageTex->GetWidth() + 20.0f, 0.0f));
	mStageNumTex->SetColor(Gdiplus::Color::White);
	mStageNumTex->SetFontSize(30.0f);
	mStageNumTex->SetStrokeWidth(1.0f);
	mStageNumTex->SetOutline(2.0f, Gdiplus::Color::Black);
	mStageNumTex->SetAlign(Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentCenter);

	infoPanel->AddChild(mStageNumTex);


	mTimeTex = new CUIText();
	mTimeTex->SetWidth(infoPanel->GetWidth());
	mTimeTex->SetHeight(stageTex->GetHeight());
	mTimeTex->SetPos(SVector2D(0.0f, stageTex->GetHeight()));
	mTimeTex->SetColor(Gdiplus::Color::White);
	mTimeTex->SetFontSize(50.0f);
	mTimeTex->SetStrokeWidth(1.0f);
	mTimeTex->SetOutline(2.0f, Gdiplus::Color::Black);
	mTimeTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	infoPanel->AddChild(mTimeTex);

	CUIBase::OnCreate();
}

void CPlaySceneUI::Active()
{
	CUIBase::Active();
}

void CPlaySceneUI::InActive()
{
	CUIBase::InActive();
}

void CPlaySceneUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CPlaySceneUI::OnUpdate(float tDeltaTime)
{
	mMoneyTex->SetText(std::to_wstring(CPlayScene::GetPlayer()->GetComponent<CPlayerScript>()->GetMoney()));
	mStageNumTex->SetText(std::to_wstring(CPlayScene::GetStageNum() + 1));
	mTimeTex->SetText(std::to_wstring((int)CMonsterSpawnMgr::GetTime()));
	CUIBase::OnUpdate(tDeltaTime);
}

void CPlaySceneUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CPlaySceneUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CPlaySceneUI::UIClear()
{
	CUIBase::UIClear();
}
