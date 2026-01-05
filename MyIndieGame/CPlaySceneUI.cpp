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

	mFpsText = new CUIText(SVector2D(), 0.0f, 50.0f);
	mFpsText->SetColor(Gdiplus::Color::White);
	mFpsText->SetFontSize(25.0f);

	this->AddChild(mFpsText);


	CUIPanel* hudPanel = new CUIPanel(SVector2D(20.0f, 20.0f), 330.0f, 140.0f);
	this->AddChild(hudPanel);


	CUIHUD* hpHUD = new CUIHUD(SVector2D(), 320.0f, 48.0f);
	hudPanel->AddChild(hpHUD);


	CUIImg* hpBg = new CUIImg(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDBg"));

	hpHUD->AddChild(hpBg);

	CUIHPBar* hpBar = new CUIHPBar(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight());
	hpBar->SetPlayer(CPlayScene::GetPlayer());

	hpHUD->AddChild(hpBar);

	CUIImg* hpFrame = new CUIImg(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDFrame"));

	hpHUD->AddChild(hpFrame);

	



	CUIHUD* EXPHUD = new CUIHUD(SVector2D(0.0f, 53.0f), 320.0f, 48.0f);
	hudPanel->AddChild(EXPHUD);

	CUIImg* epxBg = new CUIImg(SVector2D(), EXPHUD->GetWidth(), EXPHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDBg"));

	EXPHUD->AddChild(epxBg);

	CUIEXPBar* expBar = new CUIEXPBar(SVector2D(), EXPHUD->GetWidth(), EXPHUD->GetHeight());
	expBar->SetPlayer(CPlayScene::GetPlayer());

	EXPHUD->AddChild(expBar);

	CUIImg* expFrame = new CUIImg(SVector2D(), EXPHUD->GetWidth(), EXPHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDFrame"));

	EXPHUD->AddChild(expFrame);

	CUIImg* moneyIcon = new CUIImg(SVector2D(0.0f, 100.0f), 50.0f, 50.0f, CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	moneyIcon->SetImageMode(CUIImg::eImageMode::KeepAspect);

	hudPanel->AddChild(moneyIcon);

	mMoneyTex = new CUIText(SVector2D(55.0f, 100.0f), hudPanel->GetWidth() - moneyIcon->GetWidth() - 5.0f, 50.0f);
	mMoneyTex->SetFontSize(30.0f);
	mMoneyTex->SetColor(Gdiplus::Color::White);
	mMoneyTex->SetStrokeWidth(1.0f);
	mMoneyTex->SetOutline(1.0f, Gdiplus::Color::Black);

	hudPanel->AddChild(mMoneyTex);


	CUIPanel* infoPanel = new CUIPanel(SVector2D(), 200.0f, 100.0f);
	infoPanel->SetPos(SVector2D(this->GetWidth() / 2 - infoPanel->GetWidth() / 2, 20.0f));

	this->AddChild(infoPanel);

	CUIText* stageTex = new CUIText(SVector2D(), 2 * infoPanel->GetWidth() / 3 - 20.0f, 50.0f, L"¿þÀÌºê ");
	stageTex->SetColor(Gdiplus::Color::White);
	stageTex->SetFontSize(30.0f);
	stageTex->SetStrokeWidth(1.0f);
	stageTex->SetOutline(2.0f, Gdiplus::Color::Black);
	stageTex->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentCenter);

	infoPanel->AddChild(stageTex);

	mStageNumTex = new CUIText(SVector2D(stageTex->GetWidth() + 20.0f, 0.0f), infoPanel->GetWidth() - stageTex->GetWidth(), stageTex->GetHeight());
	mStageNumTex->SetColor(Gdiplus::Color::White);
	mStageNumTex->SetFontSize(30.0f);
	mStageNumTex->SetStrokeWidth(1.0f);
	mStageNumTex->SetOutline(2.0f, Gdiplus::Color::Black);
	mStageNumTex->SetAlign(Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentCenter);

	infoPanel->AddChild(mStageNumTex);


	mTimeTex = new CUIText(SVector2D(0.0f, stageTex->GetHeight()), infoPanel->GetWidth(), stageTex->GetHeight());
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
	for (int i = 0; i < mUpgradeCheckPanels.size(); ++i)
	{
		if (mUpgradeCheckPanels[i] != nullptr)
		{
			mUpgradeCheckPanels[i]->GetParent()->RemoveChild(mUpgradeCheckPanels[i]);
			mUpgradeCheckPanels[i]->OnDestroy();
			SAFE_DELETE(mUpgradeCheckPanels[i]);
		}
	}

	mUpgradeCheckPanels.clear();

	CUIBase::InActive();
}

void CPlaySceneUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CPlaySceneUI::OnUpdate(float tDeltaTime)
{
	int fps = 0;

	mCurrentTime = GetTickCount64();
	float deltaTime = (mCurrentTime - mLastTime) / 1000.0f;
	mLastTime = mCurrentTime;

	mFrameCount++;
	mTimeElapsed += deltaTime;

	if (mTimeElapsed >= 1.0f) {
		fps = mFrameCount;
		mFpsText->SetText(std::to_wstring(fps));
		mFpsText->SetWidth(mFpsText->CalculateTextSize().Width);
		mFpsText->SetPos(SVector2D(this->GetWidth() - mFpsText->GetWidth(), 0.0f));

		mFrameCount = 0;
		mTimeElapsed = 0.0f;
	}


	mMoneyTex->SetText(std::to_wstring(CPlayScene::GetPlayer()->GetComponent<CPlayerScript>()->GetMoney()));
	mStageNumTex->SetText(std::to_wstring(CPlayScene::GetStageNum() + 1));
	mTimeTex->SetText(std::to_wstring((int)CMonsterSpawnMgr::GetTime()));

	if (mCurCheckLevel < CPlayScene::GetPlayer()->GetComponent<CPlayerScript>()->GetLevel()) {
		mCurCheckLevel++;
		MakeUpgradeCheckPanel();
	}

	int x = this->GetWidth() - 75.0f;

	for (int i = 0; i < mUpgradeCheckPanels.size(); i++) {
		mUpgradeCheckPanels[i]->SetPos(SVector2D(x, 50.0f));
		x -= 75.0f;
	}

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

CUIPanel* CPlaySceneUI::MakeUpgradeCheckPanel()
{
	CUIPanel* upgradeImgPanel = new CUIPanel(SVector2D(), 75.0f, 75.0f);
	this->AddChild(upgradeImgPanel);
	mUpgradeCheckPanels.push_back(upgradeImgPanel);

	CUIImg* upgradeImg = new CUIImg(SVector2D(), 75.0f, 75.0f, CResourceMgr::Find<CTexture>(L"UpgradeIcon"));
	upgradeImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	upgradeImgPanel->AddChild(upgradeImg);

	return upgradeImgPanel;
}
