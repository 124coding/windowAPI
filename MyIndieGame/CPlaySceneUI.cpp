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

	// 좌측 상단: HP바, EXP바, 돈 표시 패널
	CUIPanel* hudPanel = new CUIPanel(SVector2D(20.0f, 20.0f), 330.0f, 140.0f);
	this->AddChild(hudPanel);

	// [HP Bar] 배경 -> 게이지 -> 프레임 순서로 적층(Layering)하여 생성
	CUIHUD* hpHUD = new CUIHUD(SVector2D(), 320.0f, 48.0f);
	hudPanel->AddChild(hpHUD);


	CUIImg* hpBg = new CUIImg(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDBg"));

	hpHUD->AddChild(hpBg);

	CUIHPBar* hpBar = new CUIHPBar(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight());
	hpBar->SetPlayer(CPlayScene::GetPlayer());

	hpHUD->AddChild(hpBar);

	CUIImg* hpFrame = new CUIImg(SVector2D(), hpHUD->GetWidth(), hpHUD->GetHeight(), CResourceMgr::Find<CTexture>(L"UIHUDFrame"));

	hpHUD->AddChild(hpFrame);

	


	// [EXP Bar]
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

	CUIText* stageTex = new CUIText(SVector2D(), 2 * infoPanel->GetWidth() / 3 - 20.0f, 50.0f, L"웨이브 ");
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
	// 씬 전환 시 동적으로 생성된 업그레이드 아이콘들 정리
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
	// ==========================================
	// Performance Monitoring (FPS Calculation)
	// ==========================================
	// GetTickCount64를 이용해 1초마다 프레임 수를 측정
	int fps = 0;

	mCurrentTime = GetTickCount64();
	float deltaTime = (mCurrentTime - mLastTime) / 1000.0f;
	mLastTime = mCurrentTime;

	mFrameCount++;
	mTimeElapsed += deltaTime;

	// 1초가 경과했을 때 FPS 갱신
	if (mTimeElapsed >= 1.0f) {
		fps = mFrameCount;
		mFpsText->SetText(std::to_wstring(fps));
		// 우측 상단에 정렬 (Anchor: Top-Right)
		mFpsText->SetWidth(mFpsText->CalculateTextSize().Width);
		mFpsText->SetPos(SVector2D(this->GetWidth() - mFpsText->GetWidth(), 0.0f));

		mFrameCount = 0;
		mTimeElapsed = 0.0f;
	}

	// ==========================================
	// Data Binding Update
	// ==========================================
	// 플레이어 데이터 -> UI 텍스트 동기화
	mMoneyTex->SetText(std::to_wstring(CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script)->GetMoney()));
	mStageNumTex->SetText(std::to_wstring(CPlayScene::GetStageNum() + 1));
	mTimeTex->SetText(std::to_wstring((int)CMonsterSpawnMgr::GetTime()));

	// [Level Up Notification]
	// 플레이어 레벨이 UI에 표시된 레벨보다 높아지면 (레벨업 감지)
	if (mCurCheckLevel < CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script)->GetLevel()) {
		mCurCheckLevel++;
		MakeUpgradeCheckPanel();
	}

	// 알림 아이콘들 위치 정렬 (우측 -> 좌측 순으로 쌓임)
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

// 레벨업 알림 아이콘 생성 (동적 UI)
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
