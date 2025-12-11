#include "CShopUI.h"

#include "StatSet.h"

#include "CDataMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CItemMgr.h"

#include "CUIButton.h"

void CShopUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);

	CUIPanel* basePanel = new CUIPanel();
	basePanel->SetWidth(this->GetWidth());
	basePanel->SetHeight(this->GetHeight());

	this->AddChild(basePanel);

	mShopMainPanel = new CUIPanel();
	mShopMainPanel->SetWidth(3 * basePanel->GetWidth() / 4);
	mShopMainPanel->SetHeight(basePanel->GetHeight());

	basePanel->AddChild(mShopMainPanel);


	CUIText* shopTex = new CUIText();

	shopTex->SetPos(SVector2D(20.0f, 20.0f));
	shopTex->SetWidth(200.0f);
	shopTex->SetHeight(40.0f);
	shopTex->SetText(L"상점");
	shopTex->SetColor(Gdiplus::Color::White);
	shopTex->SetFontSize(35.0f);
	shopTex->SetStrokeWidth(1.0f);
	shopTex->SetOutline(2.0f, Gdiplus::Color::Black);

	mShopMainPanel->AddChild(shopTex);


	CUIPanel* shopMoneyPanel = new CUIPanel();
	shopMoneyPanel->SetWidth(300.0f);
	shopMoneyPanel->SetHeight(40.0f);

	shopMoneyPanel->SetPos(SVector2D(mShopMainPanel->GetWidth() / 2 - shopMoneyPanel->GetWidth() / 2, 20.0f));

	mShopMainPanel->AddChild(shopMoneyPanel);


	CUIImg* shopMoneyIcon = new CUIImg();
	shopMoneyIcon->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	shopMoneyIcon->SetPos(SVector2D(0.0f, 5.0f));
	shopMoneyIcon->SetWidth(50.0f);
	shopMoneyIcon->SetHeight(40.0f);
	shopMoneyIcon->SetImageMode(CUIImg::eImageMode::KeepAspect);

	shopMoneyPanel->AddChild(shopMoneyIcon);

	mMoneyTex = new CUIText();
	mMoneyTex->SetPos(SVector2D(55.0f, 0.0f));
	mMoneyTex->SetWidth(mShopMainPanel->GetWidth() - shopMoneyIcon->GetWidth() - 5.0f);
	mMoneyTex->SetHeight(40.0f);
	mMoneyTex->SetFontSize(30.0f);
	mMoneyTex->SetColor(Gdiplus::Color::White);
	mMoneyTex->SetStrokeWidth(1.0f);
	mMoneyTex->SetOutline(2.0f, Gdiplus::Color::Black);

	shopMoneyPanel->AddChild(mMoneyTex);

	

	mResetButton = new CUIButton();
	mResetButton->SetHeight(50.0f);
	mResetButton->SetCornerRadius(10.0f);
	mResetButton->SetBackColor(Gdiplus::Color::Black);

	mResetTex = new CUIText();
	mResetTex->SetPos(SVector2D(10.0f, 0.0f));
	mResetTex->SetHeight(40.0f);
	mResetTex->SetFontSize(30.0f);
	mResetTex->SetStrokeWidth(1.0f);
	mResetTex->SetColor(Gdiplus::Color::White);

	mResetButton->AddChild(mResetTex);


	mResetImg = new CUIImg();
	mResetImg->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	mResetImg->SetWidth(50.0f);
	mResetImg->SetHeight(40.0f);
	mResetImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	mResetButton->AddChild(mResetImg);


	mResetButton->SetEventHover([=]() {
		mResetButton->SetBackColor(Gdiplus::Color::White);
		mResetTex->SetColor(Gdiplus::Color::Black);
		});
	mResetButton->SetEventOutHover([=]() {
		mResetButton->SetBackColor(Gdiplus::Color::Black);
		mResetTex->SetColor(Gdiplus::Color::White);
		});
	mResetButton->SetEventClick([=]() {

		});

	mShopMainPanel->AddChild(mResetButton);


	CUIPanel* goodsPanel = new CUIPanel();
	goodsPanel->SetPos(SVector2D(0.0f, 100.0f));
	goodsPanel->SetWidth(mShopMainPanel->GetWidth());
	goodsPanel->SetHeight(mShopMainPanel->GetHeight() / 2);

	mShopMainPanel->AddChild(goodsPanel);


	for (int i = 0; i < 4; i++) {
		int itemCount = 4;
		float panelW = goodsPanel->GetWidth();
		float panelH = goodsPanel->GetHeight();
		float margin = 30.0f;

		float cellWidth = panelW / (float)itemCount;
		float itemWidth = cellWidth - margin;       
		float itemHeight = 4.0f * panelH / 5.0f;    

		mGoods[i] = MakeGoods(itemWidth, itemHeight);

		float xPos = (i * cellWidth) + (margin / 2.0f);
		float yPos = 5.0f;

		mGoods[i]->SetPos(SVector2D(xPos, yPos));

		goodsPanel->AddChild(mGoods[i]);
	}


	CUIPanel* haveItemPanel = new CUIPanel();
	haveItemPanel->SetPos(SVector2D(0.0f, goodsPanel->GetPos().mY + goodsPanel->GetHeight()));
	haveItemPanel->SetWidth(2 * goodsPanel->GetWidth() / 3);
	haveItemPanel->SetHeight(windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	basePanel->AddChild(haveItemPanel);

	CUIText* haveItemText = new CUIText();
	haveItemText->SetText(L"아이템");
	haveItemText->SetFontSize(30.0f);
	haveItemText->SetPos(SVector2D(20.0f, 20.0f));
	haveItemText->SetColor(Gdiplus::Color::White);
	haveItemText->SetOutline(2.0f, Gdiplus::Color::Black);

	haveItemText->SetWidth(haveItemText->CalculateTextSize().Width);
	haveItemText->SetHeight(haveItemText->CalculateTextSize().Height);

	haveItemPanel->AddChild(haveItemText);

	CPlayScene::GetPlayer()->GetComponent<CItemMgr>();

	/* 아이템 이미지 가져오는거 만들기(Hover하면 설명도)*/

	CUIPanel* haveWeaponPanel = new CUIPanel();
	haveWeaponPanel->SetPos(SVector2D(haveItemPanel->GetWidth(), goodsPanel->GetPos().mY + goodsPanel->GetHeight()));
	haveWeaponPanel->SetWidth(goodsPanel->GetWidth() / 3);
	haveWeaponPanel->SetHeight(windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	basePanel->AddChild(haveWeaponPanel);

	CUIText* haveWeaponText = new CUIText();

	haveWeaponText->SetText(L"무기");
	haveWeaponText->SetFontSize(30.0f);
	haveWeaponText->SetPos(SVector2D(20.0f, 20.0f));
	haveWeaponText->SetColor(Gdiplus::Color::White);
	haveWeaponText->SetOutline(2.0f, Gdiplus::Color::Black);

	haveWeaponText->SetWidth(haveWeaponText->CalculateTextSize().Width);
	haveWeaponText->SetHeight(haveWeaponText->CalculateTextSize().Height);

	haveWeaponPanel->AddChild(haveWeaponText);

	/* 무기 이미지 가져오는거 만들기(Hover하면 설명도)*/


	CUIPanel* shopSidePanel = new CUIPanel();
	shopSidePanel->SetWidth(basePanel->GetWidth() - mShopMainPanel->GetWidth());
	shopSidePanel->SetHeight(basePanel->GetHeight());
	shopSidePanel->SetPos(SVector2D(mShopMainPanel->GetWidth(), 0.0f));

	basePanel->AddChild(shopSidePanel);

	CUIPanel* statPanel = new CUIPanel();
	statPanel->SetWidth(shopSidePanel->GetWidth() - 40.0f);
	statPanel->SetHeight(3 * shopSidePanel->GetHeight() / 4);
	statPanel->SetPos(SVector2D(20.0f, 20.0f));
	statPanel->SetBackColor(Gdiplus::Color::Gray);
	statPanel->SetCornerRadius(10);

	shopSidePanel->AddChild(statPanel);

	CUIText* statTex = new CUIText();
	statTex->SetText(L"능력치");
	statTex->SetFontSize(30.0f);
	statTex->SetStrokeWidth(1.0f);
	statTex->SetOutline(2.0f, Gdiplus::Color::Black);
	statTex->SetWidth(statTex->CalculateTextSize().Width);
	statTex->SetHeight(statTex->CalculateTextSize().Height);
	statTex->SetPos(SVector2D(statPanel->GetWidth() / 2 - statTex->GetWidth() / 2, 5.0f));
	statTex->SetColor(Gdiplus::Color::White);

	statPanel->AddChild(statTex);

	CUIPanel* statLevel = MakeStat(statPanel->GetWidth(), 30.0f, L"UpgradeIcon", L"현재 레벨", mLevel);
	statLevel->SetPos(SVector2D(20.0f, statTex->GetHeight() + statTex->GetPos().mY + 20.0f));
	statPanel->AddChild(statLevel);

	float statPosY = (statPanel->GetHeight() - 30.0f - (statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f)) / 12;

	CUIPanel* statMaxHP = MakeStat(statPanel->GetWidth(), 30.0f, L"MaxHP", L"최대 체력", mMaxHP);
	statMaxHP->SetPos(SVector2D(20.0f, statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f));
	statPanel->AddChild(statMaxHP);

	CUIPanel* statHPRegen = MakeStat(statPanel->GetWidth(), 30.0f, L"HPRegen", L"HP 재생", mHPRegen);
	statHPRegen->SetPos(SVector2D(20.0f, statMaxHP->GetPos().mY + statPosY));
	statPanel->AddChild(statHPRegen);

	CUIPanel* statHPSteal = MakeStat(statPanel->GetWidth(), 30.0f, L"LifeSteal", L"% 생명 훔침", mHPSteal);
	statHPSteal->SetPos(SVector2D(20.0f, statHPRegen->GetPos().mY + statPosY));
	statPanel->AddChild(statHPSteal);




	CUIPanel* statDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"Damage", L"% 데미지", mDamage);
	statDamage->SetPos(SVector2D(20.0f, statHPSteal->GetPos().mY + statPosY));
	statPanel->AddChild(statDamage);

	CUIPanel* statMeleeDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"MeleeDamage", L"근거리 데미지", mMeleeDamage);
	statMeleeDamage->SetPos(SVector2D(20.0f, statDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statMeleeDamage);

	CUIPanel* statRangedDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"RangedDamage", L"원거리 데미지", mRangedDamage);
	statRangedDamage->SetPos(SVector2D(20.0f, statMeleeDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statRangedDamage);

	CUIPanel* statAttackSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"AttackSpeed", L"% 공격 속도", mAttackSpeed);
	statAttackSpeed->SetPos(SVector2D(20.0f, statRangedDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statAttackSpeed);

	CUIPanel* statCritChance = MakeStat(statPanel->GetWidth(), 30.0f, L"CritChance", L"% 치명타율", mCritChance);
	statCritChance->SetPos(SVector2D(20.0f, statAttackSpeed->GetPos().mY + statPosY));
	statPanel->AddChild(statCritChance);

	CUIPanel* statRange = MakeStat(statPanel->GetWidth(), 30.0f, L"Range", L"범위", mRange);
	statRange->SetPos(SVector2D(20.0f, statCritChance->GetPos().mY + statPosY));
	statPanel->AddChild(statRange);

	CUIPanel* statArmor = MakeStat(statPanel->GetWidth(), 30.0f, L"Armor", L"방어구", mArmor);
	statArmor->SetPos(SVector2D(20.0f, statRange->GetPos().mY + statPosY));
	statPanel->AddChild(statArmor);

	CUIPanel* statDodge = MakeStat(statPanel->GetWidth(), 30.0f, L"Dodge", L"% 회피", mDodge);
	statDodge->SetPos(SVector2D(20.0f, statArmor->GetPos().mY + statPosY));
	statPanel->AddChild(statDodge);

	CUIPanel* statSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"Speed", L"% 속도", mSpeed);
	statSpeed->SetPos(SVector2D(20.0f, statDodge->GetPos().mY + statPosY));
	statPanel->AddChild(statSpeed);

	CUIButton* nextStageButton = new CUIButton();
	nextStageButton->SetWidth(statPanel->GetWidth());
	nextStageButton->SetHeight(50.0f);
	nextStageButton->SetCornerRadius(10);
	nextStageButton->SetBackColor(Gdiplus::Color::Black);
	nextStageButton->SetPos(SVector2D(20.0f, shopSidePanel->GetHeight() - nextStageButton->GetHeight() - 10.0f));

	shopSidePanel->AddChild(nextStageButton);

	CUIText* nextTex = new CUIText();
	nextTex->SetText(L"이동");
	nextTex->SetFontSize(35.0f);
	nextTex->SetWidth(nextStageButton->GetWidth());
	nextTex->SetHeight(nextStageButton->GetHeight());
	nextTex->SetColor(Gdiplus::Color::White);
	nextTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	nextStageButton->AddChild(nextTex);

	nextStageButton->SetEventHover([=]() {
		nextTex->SetColor(Gdiplus::Color::Black);
		nextStageButton->SetBackColor(Gdiplus::Color::White);
		});
	nextStageButton->SetEventOutHover([=]() {
		nextTex->SetColor(Gdiplus::Color::White);
		nextStageButton->SetBackColor(Gdiplus::Color::Black);
		});
	nextStageButton->SetEventClick([=]() {
		// 스테이지 넘어가는 것 구현
		});

	CUIBase::OnCreate();
}

void CShopUI::Active()
{
	CUIBase::Active();
}

void CShopUI::InActive()
{
	CUIBase::InActive();
}

void CShopUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CShopUI::OnUpdate(float tDeltaTime)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	mMoneyTex->SetText(std::to_wstring(plSc->GetMoney()));

	mResetTex->SetText(L"초기화 -" + std::to_wstring(mResetCost));
	mResetTex->SetWidth(mResetTex->CalculateTextSize().Width);

	mResetImg->SetPos(SVector2D(mResetTex->GetWidth() + 5.0f, 5.0f));

	mResetButton->SetWidth(mResetTex->GetWidth() + 60.0f);
	mResetButton->SetPos(SVector2D(mShopMainPanel->GetWidth() - mResetButton->GetWidth() - 20.0f, 20.0f));

	mLevel->SetColor(Gdiplus::Color::White);
	mLevel->SetText(std::to_wstring(plSc->GetLevel()));

	SettingStatTex(plSc->GetMaxHP(), mMaxHP);
	SettingStatTex(plSc->GetHPGeneration(), mHPRegen);
	SettingStatTex(plSc->GetLifeSteal(), mHPSteal);
	SettingStatTex(plSc->GetDamagePercent(), mDamage);
	SettingStatTex(plSc->GetMeleeDamage(), mMeleeDamage);
	SettingStatTex(plSc->GetRangedDamage(), mRangedDamage);
	SettingStatTex(plSc->GetAttackSpeedPercent(), mAttackSpeed);
	SettingStatTex(plSc->GetCriticalChancePercent(), mCritChance);
	SettingStatTex(plSc->GetRange(), mRange);
	SettingStatTex(plSc->GetArmor(), mArmor);
	SettingStatTex(plSc->GetDodge(), mDodge);
	SettingStatTex(plSc->GetSpeedPercent(), mSpeed);

	CUIBase::OnUpdate(tDeltaTime);
}

void CShopUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CShopUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CShopUI::UIClear()
{
	CUIBase::UIClear();
}

CUIPanel* CShopUI::MakeGoods(float tWidth, float tHeight)
{
	int rand = std::rand() % 2;
	// 아이템과 무기 중 랜덤하게

	CUIPanel* goodsPanel = new CUIPanel();
	goodsPanel->SetPos(SVector2D(5.0f, 5.0f));
	goodsPanel->SetWidth(tWidth);
	goodsPanel->SetHeight(tHeight);
	goodsPanel->SetBackColor(Gdiplus::Color::Black);
	goodsPanel->SetCornerRadius(10);

	CUIPanel* goodsImgPanel = new CUIPanel();

	goodsImgPanel->SetWidth(75.0f);
	goodsImgPanel->SetHeight(75.0f);
	goodsImgPanel->SetPos(SVector2D(10.0f, 10.0f));
	goodsImgPanel->SetBackColor(Gdiplus::Color::LightGray);
	goodsImgPanel->SetCornerRadius(10.0f);

	goodsPanel->AddChild(goodsImgPanel);

	CUIImg* goodsImg = new CUIImg();

	goodsImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	goodsImg->SetWidth(goodsImgPanel->GetWidth());
	goodsImg->SetHeight(goodsImgPanel->GetHeight());

	goodsImgPanel->AddChild(goodsImg);

	CUIText* goodsNameTex = new CUIText();

	goodsNameTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY));
	goodsNameTex->SetWidth(100.0f);
	goodsNameTex->SetHeight(25.0f);

	goodsNameTex->SetText(L"Test");
	goodsNameTex->SetFont(L"Noto Sans KR Medium");
	goodsNameTex->SetFontSize(20.0f);
	goodsNameTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(goodsNameTex);

	CUIText* goodsTex = new CUIText();

	goodsTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY + 30.0f));
	goodsTex->SetWidth(100.0f);
	goodsTex->SetHeight(25.0f);

	goodsTex->SetText(L"Test");
	goodsTex->SetFont(L"Noto Sans KR Medium");
	goodsTex->SetFontSize(15.0f);
	goodsTex->SetColor(Gdiplus::Color::LightYellow);

	goodsPanel->AddChild(goodsTex);


	CUIText* descriptionGoodsTex = new CUIText();

	descriptionGoodsTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX, goodsImgPanel->GetPos().mY + goodsImgPanel->GetHeight() + 10.0f));
	descriptionGoodsTex->SetWidth(200.0f);
	descriptionGoodsTex->SetHeight(250.0f);

	descriptionGoodsTex->SetFont(L"Noto Sans KR Medium");
	descriptionGoodsTex->SetFontSize(15.0f);
	descriptionGoodsTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(descriptionGoodsTex);

	// 각 아이템 or 무기 가져오기
	if (rand == 0) {

	}
	else {

	}

	CUIButton* buyButton = new CUIButton();

	CUIText* buyCost = new CUIText();
	buyCost->SetText(std::to_wstring(10));
	buyCost->SetWidth(buyCost->CalculateTextSize().Width + 10.0f);
	buyCost->SetPos(SVector2D(5.0f, -5.0f));
	buyCost->SetHeight(buyCost->CalculateTextSize().Height + 20.0f);
	buyCost->SetFontSize(30.0f);
	buyCost->SetColor(Gdiplus::Color::White);
	buyCost->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	buyButton->AddChild(buyCost);

	CUIImg* buyImg = new CUIImg();
	buyImg->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	buyImg->SetPos(SVector2D(buyCost->GetWidth() + 5.0f, 0.0f));
	buyImg->SetWidth(40.0f);
	buyImg->SetHeight(buyCost->GetHeight());
	buyImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	buyButton->AddChild(buyImg);

	buyButton->SetWidth(buyCost->GetWidth() + buyImg->GetWidth() + 10.0f);
	buyButton->SetHeight(buyCost->GetHeight());
	buyButton->SetPos(SVector2D(goodsPanel->GetWidth() / 2 - buyButton->GetWidth() / 2, goodsPanel->GetHeight() - buyButton->GetHeight() - 20.0f));
	buyButton->SetBackColor(Gdiplus::Color::Gray);
	buyButton->SetCornerRadius(10);

	goodsPanel->AddChild(buyButton);

	buyButton->SetEventHover([=]() {
		buyButton->SetBackColor(Gdiplus::Color::White);
		buyCost->SetColor(Gdiplus::Color::Black);
		});
	buyButton->SetEventOutHover([=]() {
		buyButton->SetBackColor(Gdiplus::Color::Gray);
		buyCost->SetColor(Gdiplus::Color::White);
		});
	buyButton->SetEventClick([=]() {
		// 아이템 능력 구현
		});


	CUIButton* rockButton = new CUIButton();
	rockButton->SetWidth(60.0f);
	rockButton->SetHeight(40.0f);
	rockButton->SetPos(SVector2D(goodsPanel->GetWidth() / 2 - rockButton->GetWidth() / 2, goodsPanel->GetHeight() + 30.0f));
	rockButton->SetBackColor(Gdiplus::Color::Black);
	rockButton->SetCornerRadius(10);

	goodsPanel->AddChild(rockButton);

	CUIText* rockText = new CUIText();
	rockText->SetText(L"잠금");
	rockText->SetWidth(60.0f);
	rockText->SetHeight(40.0f);
	rockText->SetColor(Gdiplus::Color::White);
	rockText->SetStrokeWidth(1.0f);
	rockText->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	rockButton->AddChild(rockText);

	rockButton->SetEventHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::White);
		rockText->SetColor(Gdiplus::Color::Black);
		});
	rockButton->SetEventOutHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::Black);
		rockText->SetColor(Gdiplus::Color::White);
		});
	rockButton->SetEventClick([=]() {
		
		// 잠금 구현
		});


	return goodsPanel;
}