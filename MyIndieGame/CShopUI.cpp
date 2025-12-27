#include "CShopUI.h"

#include "StatSet.h"
#include "Effect.h"

#include "CDataMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CPlayerScript.h"
#include "CItemMgr.h"
#include "CWeaponMgr.h"
#include "CWeaponScript.h"

#include "CUIButton.h"

#include <regex>
#include <algorithm>

void CShopUI::OnCreate()
{
	mPl = CPlayScene::GetPlayer();
	mPlSc = mPl->GetComponent<CPlayerScript>();
	mPlItemMgr = mPl->GetComponent<CItemMgr>();
	mPlWeaponMgr = mPl->GetComponent<CWeaponMgr>();
	mPlItems = &mPlItemMgr->GetItems();
	mPlWeapons = &mPlWeaponMgr->GetWeapons();

	SetWidth(windowWidth);
	SetHeight(windowHeight);

	// 상점의 메인이 될 패널
	mShopMainPanel = new CUIPanel(SVector2D(), 3 * this->GetWidth() / 4, this->GetHeight());

	this->AddChild(mShopMainPanel);


	CUIText* shopTex = new CUIText(SVector2D(20.0f, 20.0f), 200.0f, 40.0f, L"상점");

	shopTex->SetColor(Gdiplus::Color::White);
	shopTex->SetFontSize(35.0f);
	shopTex->SetStrokeWidth(1.0f);
	shopTex->SetOutline(2.0f, Gdiplus::Color::Black);

	mShopMainPanel->AddChild(shopTex);


	CUIPanel* shopMoneyPanel = new CUIPanel(SVector2D(), 300.0f, 40.0f);

	shopMoneyPanel->SetPos(SVector2D(mShopMainPanel->GetWidth() / 2 - shopMoneyPanel->GetWidth() / 2, 20.0f));

	mShopMainPanel->AddChild(shopMoneyPanel);


	CUIImg* shopMoneyIcon = new CUIImg(SVector2D(0.0f, 5.0f), 50.0f, 40.0f, CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	shopMoneyIcon->SetImageMode(CUIImg::eImageMode::KeepAspect);

	shopMoneyPanel->AddChild(shopMoneyIcon);

	mMoneyTex = new CUIText(SVector2D(55.0f, 0.0f), mShopMainPanel->GetWidth() - shopMoneyIcon->GetWidth() - 5.0f, 40.0f);
	mMoneyTex->SetFontSize(30.0f);
	mMoneyTex->SetColor(Gdiplus::Color::White);
	mMoneyTex->SetStrokeWidth(1.0f);
	mMoneyTex->SetOutline(2.0f, Gdiplus::Color::Black);

	shopMoneyPanel->AddChild(mMoneyTex);

	

	mResetButton = new CUIButton(SVector2D(), 0.0f, 40.0f);
	mResetButton->SetBackColor(Gdiplus::Color::Black);
	mResetButton->SetCornerRadius(10);

	mResetTex = new CUIText(SVector2D(10.0f, 0.0f), 0.0f, 40.0f);
	mResetTex->SetFontSize(30.0f);
	mResetTex->SetStrokeWidth(1.0f);
	mResetTex->SetColor(Gdiplus::Color::White);
	mResetTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	mResetButton->AddChild(mResetTex);


	mResetImg = new CUIImg(SVector2D(), 50.0f, 40.0f, CResourceMgr::Find<CTexture>(L"HarvestIcon"));
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

	mShopMainPanel->AddChild(mResetButton);


	// 상점에서 구매할 수 있는 아이템들을 보여주는 패널
	CUIPanel* goodsPanel = new CUIPanel(SVector2D(0.0f, 100.0f), mShopMainPanel->GetWidth(), mShopMainPanel->GetHeight() / 2);

	mShopMainPanel->AddChild(goodsPanel);

	mResetButton->SetEventClick([=]() {

		if (mPlSc->GetMoney() > mResetCost) {
			mPlSc->ChangeMoney(-mResetCost);
			mResetCount++;
			mResetCost *= 2;
		}
		else {
			return;
		}

		std::vector<std::pair<CUIPanel*, bool>> lockedItems;
		lockedItems.reserve(4);

		for (int i = 0; i < 4; ++i)
		{
			if (mGoods[i].second == true) {
				lockedItems.push_back(mGoods[i]);
			}
			else
			{
				if (mGoods[i].first != nullptr)
				{
					mGoods[i].first->GetParent()->RemoveChild(mGoods[i].first);
					mGoods[i].first->OnDestroy();
					SAFE_DELETE(mGoods[i].first);
				}
			}
		}

		mGoods.clear();

		for (const auto& item : lockedItems)
		{
			mGoods.push_back(item);
		}

		int currentCount = (int)mGoods.size();

		float panelW = goodsPanel->GetWidth();
		float panelH = goodsPanel->GetHeight();
		float margin = 30.0f;

		float cellWidth = panelW / (float)4;
		float itemWidth = cellWidth - margin;
		float itemHeight = 4.0f * panelH / 5.0f;

		for (int i = mGoods.size(); i < 4; i++) {
			mGoods.push_back(MakeGoods(i, itemWidth, itemHeight));
			goodsPanel->AddChild(mGoods[i].first);
		}

		for (int i = 0; i < 4; i++) {
			CUIPanel* panel = mGoods[i].first;
			if (panel == nullptr) continue;
		
			float xPos = (i * cellWidth) + (margin / 2.0f);
			float yPos = 5.0f;

			mGoods[i].first->SetPos(SVector2D(xPos, yPos));
		}
		
		});

	for (int i = 0; i < 4; i++) {
		int itemCount = 4;
		float panelW = goodsPanel->GetWidth();
		float panelH = goodsPanel->GetHeight();
		float margin = 30.0f;

		float cellWidth = panelW / (float)itemCount;
		float itemWidth = cellWidth - margin;       
		float itemHeight = 4.0f * panelH / 5.0f;    

		mGoods[i] = MakeGoods(i, itemWidth, itemHeight);

		float xPos = (i * cellWidth) + (margin / 2.0f);
		float yPos = 5.0f;

		mGoods[i].first->SetPos(SVector2D(xPos, yPos));
		goodsPanel->AddChild(mGoods[i].first);
	}

	// 현재 가지고 있는 아이템들을 표시해주는 패널
	mHaveItemPanel = new CUIPanel(SVector2D(0.0f, goodsPanel->GetPos().mY + goodsPanel->GetHeight()), 2 * goodsPanel->GetWidth() / 3, windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	mShopMainPanel->AddChild(mHaveItemPanel);

	mHaveItemText = new CUIText(SVector2D(20.0f, 20.0f), 0.0f, 0.0f, L"아이템");
	mHaveItemText->SetFontSize(30.0f);
	mHaveItemText->SetColor(Gdiplus::Color::White);
	mHaveItemText->SetOutline(2.0f, Gdiplus::Color::Black);

	mHaveItemText->SetWidth(mHaveItemText->CalculateTextSize().Width);
	mHaveItemText->SetHeight(mHaveItemText->CalculateTextSize().Height);

	mHaveItemPanel->AddChild(mHaveItemText);

	float x = 20.0f;
	float y = mHaveItemText->GetPos().mY + mHaveItemText->GetHeight() + 20.0f;
	float offset = 10.0f;

	// 플레이어 아이템 정보에서 가져옴
	for (auto& item : *mPlItems) {
		CUIPanel* itemPanel = MakeItemPanel(item.first, x, y);

		CUIText* itemCountTex = new CUIText(SVector2D(), itemPanel->GetWidth(), itemPanel->GetHeight(), std::to_wstring(item.second));
		itemCountTex->SetBold(true);
		itemCountTex->SetFontSize(20.0f);
		itemCountTex->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentFar);
		itemCountTex->SetColor(Gdiplus::Color::LightYellow);
		itemPanel->AddChild(itemCountTex);
		mHaveItemPanel->AddChild(itemPanel);

		mItems.push_back(std::make_pair(itemPanel, itemCountTex));

		x += itemPanel->GetWidth() + offset;

		if (x > mHaveItemPanel->GetWidth()) {
			x = 20.0f;
			y += itemPanel->GetPos().mY + itemPanel->GetHeight() + offset;
		}
	}


	// 현재 가지고 있는 무기들을 보여주는 패널
	mHaveWeaponPanel = new CUIPanel(SVector2D(mHaveItemPanel->GetWidth(), goodsPanel->GetPos().mY + goodsPanel->GetHeight()), goodsPanel->GetWidth() / 3, windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	mShopMainPanel->AddChild(mHaveWeaponPanel);

	mHaveWeaponText = new CUIText(SVector2D(20.0f, 20.0f), 0.0f, 0.0f, L"무기");

	mHaveWeaponText->SetFontSize(30.0f);
	mHaveWeaponText->SetColor(Gdiplus::Color::White);
	mHaveWeaponText->SetOutline(2.0f, Gdiplus::Color::Black);

	mHaveWeaponText->SetWidth(mHaveWeaponText->CalculateTextSize().Width);
	mHaveWeaponText->SetHeight(mHaveWeaponText->CalculateTextSize().Height);

	mHaveWeaponPanel->AddChild(mHaveWeaponText);





	x = 20.0f;
	y = mHaveWeaponText->GetPos().mY + mHaveWeaponText->GetHeight() + 20.0f;
	offset = 10.0f;


	// 가지고 있는 플레이어 무기 데이터에서 가져옴
	for (auto& weapon : *mPlWeapons) {
		CUIButton* weaponButton = MakeWeaponButton(mPlWeapons, weapon, x, y);
		x += weaponButton->GetWidth() + offset;

		if (x > mHaveWeaponPanel->GetWidth()) {
			x = 20.0f;
			y = y + weaponButton->GetPos().mY + weaponButton->GetHeight() + offset;
		}
	}




	// 사이드에서 현재 플레이어의 스탯을 보여주기 위한 패널
	CUIPanel* shopSidePanel = new CUIPanel(SVector2D(mShopMainPanel->GetWidth(), 0.0f), this->GetWidth() - mShopMainPanel->GetWidth(), this->GetHeight());
	shopSidePanel->SetPos(SVector2D(mShopMainPanel->GetWidth(), 0.0f));

	this->AddChild(shopSidePanel);

	CUIPanel* statPanel = new CUIPanel(SVector2D(20.0f, 20.0f), shopSidePanel->GetWidth() - 40.0f, 3 * shopSidePanel->GetHeight() / 4);
	statPanel->SetBackColor(Gdiplus::Color::Gray);
	statPanel->SetCornerRadius(10);

	shopSidePanel->AddChild(statPanel);

	CUIText* statTex = new CUIText(SVector2D(), 0.0f, 0.0f, L"능력치");
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


	// 다음 스테이지로 이동하는 이동 버튼
	CUIButton* nextStageButton = new CUIButton(SVector2D(), statPanel->GetWidth(), 50.0f);
	nextStageButton->SetCornerRadius(10);
	nextStageButton->SetBackColor(Gdiplus::Color::Black);
	nextStageButton->SetPos(SVector2D(20.0f, shopSidePanel->GetHeight() - nextStageButton->GetHeight() - 10.0f));

	shopSidePanel->AddChild(nextStageButton);

	CUIText* nextTex = new CUIText(SVector2D(), nextStageButton->GetWidth(), nextStageButton->GetHeight(), L"이동");
	nextTex->SetFontSize(35.0f);
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
		CSceneMgr::LoadScene(L"PlayScene");
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
	mMoneyTex->SetText(std::to_wstring(mPlSc->GetMoney()));

	mResetTex->SetText(L"초기화 -" + std::to_wstring(mResetCost));
	mResetTex->SetWidth(mResetTex->CalculateTextSize().Width);
	mResetTex->SetPos(SVector2D(mResetButton->GetWidth() / 2 - 2 * mResetTex->GetWidth() / 3, 0.0f));

	mResetImg->SetPos(SVector2D(mResetTex->GetPos().mX + mResetTex->GetWidth() + 5.0f, 0.0f));

	mResetButton->SetWidth(mResetTex->GetWidth() + 60.0f);
	mResetButton->SetPos(SVector2D(mShopMainPanel->GetWidth() - mResetButton->GetWidth() - 20.0f, 20.0f));

	mLevel->SetColor(Gdiplus::Color::White);
	mLevel->SetText(std::to_wstring(mPlSc->GetLevel()));

	SettingStatTex(mPlSc->GetMaxHP(), mMaxHP);
	SettingStatTex(mPlSc->GetHPGeneration(), mHPRegen);
	SettingStatTex(mPlSc->GetLifeSteal(), mHPSteal);
	SettingStatTex(mPlSc->GetDamagePercent(), mDamage);
	SettingStatTex(mPlSc->GetMeleeDamage(), mMeleeDamage);
	SettingStatTex(mPlSc->GetRangedDamage(), mRangedDamage);
	SettingStatTex(mPlSc->GetAttackSpeedPercent(), mAttackSpeed);
	SettingStatTex(mPlSc->GetCriticalChancePercent(), mCritChance);
	SettingStatTex(mPlSc->GetRange(), mRange);
	SettingStatTex(mPlSc->GetArmor(), mArmor);
	SettingStatTex(mPlSc->GetDodge(), mDodge);
	SettingStatTex(mPlSc->GetSpeedPercent(), mSpeed);

	for (int i = 0; i < mGoods.size(); i++) {
		if (mGoods[i].first == nullptr) continue;

		if (!mGoods[i].first->GetEnabled()) {
			mGoods[i].first->GetParent()->RemoveChild(mGoods[i].first);
			mGoods[i].first->OnDestroy();
			SAFE_DELETE(mGoods[i].first);
			mGoods[i].second = false;
		}
	}

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

std::pair<CUIPanel*, bool> CShopUI::MakeGoods(int tIdx, float tWidth, float tHeight)
{
	int rand = std::rand() % 3;
	// 아이템과 무기 중 랜덤하게

	int curStage = CPlayScene::GetStageNum();

	CUIPanel* goodsPanel = new CUIPanel(SVector2D(5.0f, 5.0f), tWidth, tHeight);
	goodsPanel->SetBackColor(Gdiplus::Color::Black);
	goodsPanel->SetCornerRadius(10);

	CUIPanel* goodsImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	goodsImgPanel->SetBackColor(Gdiplus::Color::LightGray);
	goodsImgPanel->SetCornerRadius(10.0f);

	goodsPanel->AddChild(goodsImgPanel);

	CUIImg* goodsImg = new CUIImg(SVector2D(), goodsImgPanel->GetWidth(), goodsImgPanel->GetHeight());

	goodsImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	goodsImgPanel->AddChild(goodsImg);

	CUIText* goodsNameTex = new CUIText(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY), 100.0f, 25.0f);
	goodsNameTex->SetFontSize(20.0f);
	goodsNameTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(goodsNameTex);

	CUIText* goodsTex = new CUIText(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY + 30.0f), 100.0f, 25.0f);
	goodsTex->SetFontSize(15.0f);
	goodsTex->SetColor(Gdiplus::Color::LightYellow);

	goodsPanel->AddChild(goodsTex);


	CUIText* descriptionGoodsTex = new CUIText(SVector2D(goodsImgPanel->GetPos().mX, goodsImgPanel->GetPos().mY + goodsImgPanel->GetHeight() + 10.0f), 200.0f, 250.0f);
	descriptionGoodsTex->SetFontSize(15.0f);
	descriptionGoodsTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(descriptionGoodsTex);

	CUIButton* buyButton = new CUIButton(SVector2D(), 0.0f, 0.0f);

	int cost = 0;

	if (curStage != 0) {
		cost += std::rand() % (10 * curStage) - std::rand() % (10 * curStage * 2);
	}

	CUIText* buyCost = new CUIText(SVector2D(5.0f, -5.0f), 0.0f, 0.0f);
	buyCost->SetText(std::to_wstring(cost));
	buyCost->SetWidth(buyCost->CalculateTextSize().Width + 10.0f);
	buyCost->SetHeight(buyCost->CalculateTextSize().Height + 20.0f);
	buyCost->SetFontSize(30.0f);
	buyCost->SetColor(Gdiplus::Color::White);
	buyCost->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	buyButton->AddChild(buyCost);

	CDataMgr::SItem curItem = CDataMgr::SItem();
	std::pair<int, CDataMgr::SWeapon> curWeapon = std::make_pair(0, CDataMgr::SWeapon());

	// 각 아이템 or 무기 가져오기
	if (rand > 0) {
		curItem = CDataMgr::GetRandomItemByStage(curStage);
		goodsImg->SetTexture(CResourceMgr::Find<CTexture>(curItem.name + L"Icon"));
		UINT color = 0;

		switch (curItem.tier) {
		case 2:
			color = Gdiplus::Color::SteelBlue;
			break;
		case 3:
			color = Gdiplus::Color::BlueViolet;
			break;
		case 4:
			color = Gdiplus::Color::DarkRed;
			break;
		default:
			color = 0xFF111111;
			break;
		}

		goodsImgPanel->SetBackColor(color);
		goodsNameTex->SetText(curItem.name);
		goodsTex->SetText(L"아이템");

		std::wstring finalDiscription = L"";

		for (auto& [effectID, args] : curItem.effects) {
			auto it = CDataMgr::GetEffectDatas().find(effectID);
			if (it == CDataMgr::GetEffectDatas().end()) {
				continue;
			}

			std::wstring rawDesc = it->second.description;

			int index = 0;

			for (auto& arg : args) {
				std::wstring value = arg.value;

				std::wstring colorStr = L"#FFFFFF"; // 기본값
				if (arg.color != L"") {
					colorStr = arg.color;
				}

				if (colorStr == L"#00FF00")
				{
					try {
						int iVal = std::stoi(arg.value);

						if (iVal > 0) {
							value = L"+" + value;
						}
					}
					catch (...) {
						// 변환 실패
					}
				}

				std::wstring taggedStr = L"<c=" + colorStr + L">" + value + L"</c>";

				// 설명글 치환 ({0} -> 태그 문자열)
				// L"\\{" + 숫자 + L"\\}" 형태의 정규식 패턴 생성
				std::wstring pattern = L"\\{" + std::to_wstring(index) + L"\\}";

				try {
					// rawDesc 안에 있는 {i}를 taggedStr로 교체
					rawDesc = std::regex_replace(rawDesc, std::wregex(pattern), taggedStr);
				}
				catch (...) {
					// 정규식 에러 예외처리 (원본 유지)
				}
				index++;
			}

			finalDiscription += rawDesc + L"\n";
		}

		finalDiscription = CUIText::InsertLineBreaks(finalDiscription, goodsPanel->GetWidth() - descriptionGoodsTex->GetPos().mX * 2, L"Noto Sans KR Medium", 15.0f, false);

		descriptionGoodsTex->SetText(finalDiscription);

		cost += curItem.basePrice * (1.0f + curStage * 0.2f - 0.2f);
		buyCost->SetText(std::to_wstring(cost));
	}
	else {
		curWeapon = CDataMgr::GetRandomWeaponByStage(curStage);

		UINT color = 0;

		switch (curWeapon.first) {
		case 2:
			color = Gdiplus::Color::SteelBlue;
			break;
		case 3:
			color = Gdiplus::Color::BlueViolet;
			break;
		case 4:
			color = Gdiplus::Color::DarkRed;
			break;
		default:
			color = 0xFF111111;
			break;
		}

		goodsImg->SetTexture(CResourceMgr::Find<CTexture>(curWeapon.second.iconTexture));

		goodsImgPanel->SetBackColor(color);
		goodsNameTex->SetText(curWeapon.second.name);
		goodsTex->SetText(curWeapon.second.classType);

		std::wstring weaponDiscription = L"";

		auto& data = curWeapon.second.tier[curWeapon.first - 1];

		// 깔끔한 wstring으로 바꿔주는 도우미 람다
		auto GetCleanVal = [](float value) -> std::wstring {
			std::wstring valStr = std::to_wstring(value);
			if (valStr.find(L'.') != std::wstring::npos) {
				valStr.erase(valStr.find_last_not_of(L'0') + 1);
				if (valStr.back() == L'.') valStr.pop_back();
			}
			return valStr;
			};

		// 텍스트를 붙여주는 도우미 람다
		auto descriptionStat = [&](std::wstring label, float value, std::wstring suffix = L"") {
			if (value <= 0.0f) return;
			weaponDiscription += L"<c=#FFFFE0>" + label + L":</c> " + GetCleanVal(value) + suffix + L"\n";
			};

		descriptionStat(L"데미지", data.damage);

		if (data.critDamagePer > 0.0f) {
			weaponDiscription += L"<c=#FFFFE0>치명타:</c> x" + GetCleanVal(data.critDamagePer);

			if (data.critChancePer > 0.0f) {
				weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
			}
			weaponDiscription += L"\n";
		}

		descriptionStat(L"쿨다운", data.delay, L"s");
		descriptionStat(L"범위", data.range);

		if (data.lifeSteal != 0.0f) {
			descriptionStat(L"생명 훔침", data.lifeSteal, L"%");
		}

		descriptionGoodsTex->SetText(weaponDiscription);

		cost += curWeapon.second.tier[curWeapon.first].basePrice * (1.0f + curStage * 0.2f - 0.2f);
		buyCost->SetText(std::to_wstring(cost));
	}

	CUIImg* buyImg = new CUIImg(SVector2D(buyCost->GetWidth() + 5.0f, 0.0f), 40.0f, buyCost->GetHeight(), CResourceMgr::Find<CTexture>(L"HarvestIcon"));
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
		if (mPlSc->GetMoney() < cost) return;

		if (curItem.ID != L"") {
			for (auto effect : curItem.effects) {
				ApplyEffect(effect.first, effect.second);
			}

			int index = mPlItemMgr->PlusItem(curItem.ID);
			if (index >= mItems.size()) {
				float x = 20.0f;
				float y = mHaveItemText->GetPos().mY + mHaveItemText->GetHeight() + 20.0f;
				float offset = 10.0f;

				if (mItems.size() > 0) {
					y = mItems[mItems.size() - 1].first->GetPos().mY;

					if (mItems[mItems.size() - 1].first->GetPos().mX + mItems[mItems.size() - 1].first->GetWidth() * 2 + offset > mHaveItemPanel->GetWidth()) {
						x = 20.0f;
						y = mItems[mItems.size() - 1].first->GetPos().mY + mItems[mItems.size() - 1].first->GetHeight() + offset;
					}
					else {
						x = mItems[mItems.size() - 1].first->GetPos().mX + mItems[mItems.size() - 1].first->GetWidth() + offset;
					}
				}

				CUIPanel* itemPanel = MakeItemPanel(curItem.ID, x, y);

				CUIText* itemCountTex = new CUIText(SVector2D(), itemPanel->GetWidth(), itemPanel->GetHeight(), std::to_wstring((*mPlItems)[index].second));
				itemCountTex->SetBold(true);
				itemCountTex->SetFontSize(20.0f);
				itemCountTex->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentFar);
				itemCountTex->SetColor(Gdiplus::Color::LightYellow);
				itemPanel->AddChild(itemCountTex);

				mHaveItemPanel->AddChild(itemPanel);

				mItems.push_back(std::make_pair(itemPanel, itemCountTex));
			}
			else {
				mItems[index].second->SetText(std::to_wstring((*mPlItems)[index].second));
			}
		}
		else if (curWeapon.first != 0) {
			eLayerType type = eLayerType::None;

			if (curWeapon.second.ID[0] == L'M') {
				type = eLayerType::MeleeWeapon;
			}
			else if (curWeapon.second.ID[0] == L'R') {
				type = eLayerType::RangedWeapon;
			}

			std::pair<int, CWeapon*> weaponIndex = mPlWeaponMgr->PlusWeapon(type, curWeapon.second.ID, curWeapon.first);

			if (weaponIndex.first == -1) {
				return;
			}

			if (weaponIndex.first >= mWeapons.size()) {
				MakeWeaponButton(mPlWeapons, weaponIndex.second, 0.0f, 0.0f);
			}
			else {
				ReSettingWeaponButton(weaponIndex.second->GetComponent<CWeaponScript>(), 
					weaponIndex.second, 
					mHaveWeaponPanel, 
					mWeapons[weaponIndex.first], 
					mWeaponsDesc[weaponIndex.first], 
					mWeaponsImgPanel[weaponIndex.first], 
					mRecycleButtons[weaponIndex.first],
					mRecycleTexts[weaponIndex.first]);
			}

			WeaponButtonsReSetting(20.0f, mHaveWeaponText->GetPos().mY + mHaveWeaponText->GetHeight() + 20.0f, 10.0f, mHaveWeaponPanel->GetWidth());
		}

		goodsPanel->InActive();

		mPlSc->ChangeMoney(-cost);
		});


	CUIButton* rockButton = new CUIButton(SVector2D(), 60.0f, 40.0f);
	rockButton->SetPos(SVector2D(goodsPanel->GetWidth() / 2 - rockButton->GetWidth() / 2, goodsPanel->GetHeight() + 30.0f));
	rockButton->SetBackColor(Gdiplus::Color::Black);
	rockButton->SetCornerRadius(10);

	goodsPanel->AddChild(rockButton);

	CUIText* rockText = new CUIText(SVector2D(), 60.0f, 40.0f, L"잠금");
	rockText->SetColor(Gdiplus::Color::White);
	rockText->SetStrokeWidth(1.0f);
	rockText->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	rockButton->AddChild(rockText);

	CUIImg* rockImg = new CUIImg(SVector2D(rockButton->GetPos().mX + rockButton->GetWidth() + 10.0f, rockButton->GetPos().mY), 30.0f, 30.0f, CResourceMgr::Find<CTexture>(L""));
	rockImg->InActive();
	rockImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	goodsPanel->AddChild(rockImg);

	rockButton->SetEventHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::White);
		rockText->SetColor(Gdiplus::Color::Black);
		});
	rockButton->SetEventOutHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::Black);
		rockText->SetColor(Gdiplus::Color::White);
		});
	rockButton->SetEventClick([=]() {
		mGoods[tIdx].second = !mGoods[tIdx].second;
		bool isLocked = mGoods[tIdx].second;

		if (isLocked) {
			rockImg->Active();
		}
		else {
			rockImg->InActive();
		}
		});


	return std::make_pair(goodsPanel, false);
}

CUIPanel* CShopUI::MakeItemPanel(std::wstring tItemID, float tX, float tY)
{
	auto currentItem = CDataMgr::GetItemDatas().find(tItemID);

	if (currentItem == CDataMgr::GetItemDatas().end()) return nullptr;

	UINT color = 0;

	switch (currentItem->second.tier) {
	case 2:
		color = Gdiplus::Color::SteelBlue;
		break;
	case 3:
		color = Gdiplus::Color::BlueViolet;
		break;
	case 4:
		color = Gdiplus::Color::DarkRed;
		break;
	default:
		color = 0xFF111111;
		break;
	}

	CUIPanel* itemPanel = new CUIPanel(SVector2D(tX, tY), 75.0f, 75.0f);
	itemPanel->SetCornerRadius(10);
	itemPanel->SetBackColor(color);
	// itemPanel->SetUsedClipping(true);

	CUIImg* itemImg = new CUIImg(SVector2D(), itemPanel->GetWidth(), itemPanel->GetHeight(), CResourceMgr::Find<CTexture>(currentItem->second.name + L"Icon"));
	itemImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	itemPanel->AddChild(itemImg);




	CUIPanel* itemDescPanel = new CUIPanel(SVector2D(), mShopMainPanel->GetWidth() / 4.0f - 30.0f, 0.0f);
	itemDescPanel->SetCornerRadius(10);
	itemDescPanel->InActive();
	itemDescPanel->SetBackColor(0xDD000000);

	itemPanel->AddChild(itemDescPanel);

	CUIPanel* itemImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	itemImgPanel->SetCornerRadius(10);
	itemImgPanel->SetBackColor(color);

	itemDescPanel->AddChild(itemImgPanel);

	CUIImg* itemDescImg = new CUIImg(SVector2D(), itemImgPanel->GetWidth(), itemImgPanel->GetHeight(), CResourceMgr::Find<CTexture>(currentItem->second.name + L"Icon"));
	itemDescImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	itemDescPanel->AddChild(itemDescImg);

	CUIText* itemNameTex = new CUIText(SVector2D(itemImgPanel->GetPos().mX + itemImgPanel->GetWidth() + 10.0f, itemDescPanel->GetPos().mY), 100.0f, 25.0f, currentItem->second.name);
	itemNameTex->SetFontSize(20.0f);
	itemNameTex->SetColor(Gdiplus::Color::White);

	itemDescPanel->AddChild(itemNameTex);

	CUIText* itemCategoryTex = new CUIText(SVector2D(itemImgPanel->GetPos().mX + itemImgPanel->GetWidth() + 10.0f, itemDescPanel->GetPos().mY + 30.0f), 100.0f, 25.0f, L"아이템");
	itemCategoryTex->SetFontSize(15.0f);
	itemCategoryTex->SetColor(Gdiplus::Color::LightYellow);

	itemDescPanel->AddChild(itemCategoryTex);

	CUIText* itemDescTex = new CUIText(SVector2D(itemImgPanel->GetPos().mX, itemImgPanel->GetPos().mY + itemImgPanel->GetHeight() + 10.0f), 200.0f, 0.0f);
	itemDescTex->SetFontSize(15.0f);
	itemDescTex->SetColor(Gdiplus::Color::White);

	itemDescPanel->AddChild(itemDescTex);

	std::wstring finalDiscription = L"";

	for (auto& [effectID, args] : currentItem->second.effects) {
		auto it = CDataMgr::GetEffectDatas().find(effectID);
		if (it == CDataMgr::GetEffectDatas().end()) {
			continue;
		}

		std::wstring rawDesc = it->second.description;

		int index = 0;

		for (auto& arg : args) {
			std::wstring value = arg.value;

			std::wstring colorStr = L"#FFFFFF"; // 기본값
			if (arg.color != L"") {
				colorStr = arg.color;
			}

			if (colorStr == L"#00FF00")
			{
				try {
					int iVal = std::stoi(arg.value);

					if (iVal > 0) {
						value = L"+" + value;
					}
				}
				catch (...) {
					// 변환 실패
				}
			}

			std::wstring taggedStr = L"<c=" + colorStr + L">" + value + L"</c>";

			// 설명글 치환 ({0} -> 태그 문자열)
			// L"\\{" + 숫자 + L"\\}" 형태의 정규식 패턴 생성
			std::wstring pattern = L"\\{" + std::to_wstring(index) + L"\\}";

			try {
				// rawDesc 안에 있는 {i}를 taggedStr로 교체
				rawDesc = std::regex_replace(rawDesc, std::wregex(pattern), taggedStr);
			}
			catch (...) {
				// 정규식 에러 예외처리 (원본 유지)
			}
			index++;
		}

		finalDiscription += rawDesc + L"\n";
	}

	finalDiscription = CUIText::InsertLineBreaks(finalDiscription, itemDescPanel->GetWidth() - itemDescPanel->GetPos().mX * 2, L"Noto Sans KR Medium", 15.0f, false);

	itemDescTex->SetText(finalDiscription);
	itemDescTex->SetHeight(itemDescTex->CalculateTextSize().Height);
	itemDescPanel->SetHeight(itemImgPanel->GetPos().mY * 2 + itemImgPanel->GetHeight() + itemDescTex->GetHeight());
	itemDescPanel->SetPos(SVector2D(0.0f, -(10.0f + itemDescPanel->GetHeight())));

	itemPanel->SetEventHover([=]() {
		itemPanel->SetBackColor(Gdiplus::Color::White);
		itemDescPanel->Active();
		});
	itemPanel->SetEventOutHover([=]() {
		itemPanel->SetBackColor(color);
		itemDescPanel->InActive();
		});

	return itemPanel;
}

// 결합이 클릭된 버튼의 수정 사항들을 보이기 위한 함수
void CShopUI::ReSettingWeaponButton(CWeaponScript* tWpScript, CWeapon* tCurWp, CUIPanel* tParPanel, CUIButton* tWpButton, CUIText* tDescTex, CUIPanel* tWpImgPanel, CUIButton* tRecycleButton, CUIText* tRecycleTex)
{

	auto currentWeapon = CDataMgr::GetWeaponDatas().find(tCurWp->GetID());

	if (currentWeapon == CDataMgr::GetWeaponDatas().end()) return;

	UINT color = 0;

	switch (tWpScript->GetTier()) {
	case 2:
		color = Gdiplus::Color::SteelBlue;
		break;
	case 3:
		color = Gdiplus::Color::BlueViolet;
		break;
	case 4:
		color = Gdiplus::Color::DarkRed;
		break;
	default:
		color = 0xFF111111;
		break;
	}


	tWpButton->SetBackColor(color);
	tWpImgPanel->SetBackColor(color);

	std::wstring weaponDiscription = L"";

	auto& data = currentWeapon->second.tier[tWpScript->GetTier() - 1];

	// 깔끔한 wstring으로 바꿔주는 도우미 람다
	auto GetCleanVal = [](float value) -> std::wstring {
		std::wstring valStr = std::to_wstring(value);
		if (valStr.find(L'.') != std::wstring::npos) {
			valStr.erase(valStr.find_last_not_of(L'0') + 1);
			if (valStr.back() == L'.') valStr.pop_back();
		}
		return valStr;
		};

	// 텍스트를 붙여주는 도우미 람다
	auto descriptionStat = [&](std::wstring label, float value, std::wstring suffix = L"") {
		if (value <= 0.0f) return;
		weaponDiscription += L"<c=#FFFFE0>" + label + L":</c> " + GetCleanVal(value) + suffix + L"\n";
		};

	descriptionStat(L"데미지", data.damage);

	if (data.critDamagePer > 0.0f) {
		weaponDiscription += L"<c=#FFFFE0>치명타:</c> x" + GetCleanVal(data.critDamagePer);

		if (data.critChancePer > 0.0f) {
			weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
		}
		weaponDiscription += L"\n";
	}

	descriptionStat(L"쿨다운", data.delay, L"s");
	descriptionStat(L"범위", data.range);

	if (data.lifeSteal != 0.0f) {
		descriptionStat(L"생명 훔침", data.lifeSteal, L"%");
	}

	tDescTex->SetText(weaponDiscription);

	int recycleCost = data.basePrice / 3.0f;

	tRecycleButton->SetEventClick([=]() {
		mPlSc->ChangeMoney(recycleCost);

		for (int i = 0; i < mPlWeapons->size(); i++) {

			if ((*mPlWeapons)[i]->GetID() == tCurWp->GetID() &&
				(*mPlWeapons)[i]->GetComponent<CWeaponScript>()->GetTier() == tWpScript->GetTier())
			{
				if (mWeapons[i] != nullptr) {
					tParPanel->RemoveChild(mWeapons[i]);
				}

				mWeapons.erase(mWeapons.begin() + i);
				mPlWeaponMgr->RemoveWeapon(i);
				break;
			}
		}
		});

	tRecycleTex->SetText(L"재활용 (+" + std::to_wstring(recycleCost) + L")");
}

// 결합이나 재결합이 클릭되었을 때 모든 무기 버튼들의 수정 사항 (위치, 설명 패널 위치, 결합 버튼 활성화 여부)
void CShopUI::WeaponButtonsReSetting(float tX, float tY, float tOffset, float tMax)
{

	for (int i = 0; i < mWeapons.size(); i++) {
		mWeapons[i]->SetPos(SVector2D(tX, tY));
		tX += mWeapons[i]->GetWidth() + tOffset;

		mWeaponsDescPanel[i]->SetPos(SVector2D(mWeapons[i]->GetPos().mX + mWeapons[i]->GetWidth() - mWeaponsDescPanel[i]->GetWidth(), mWeapons[i]->GetPos().mY - (10.0f + mWeaponsDescPanel[i]->GetHeight())));
		
		if (tX + mWeapons[i]->GetWidth() > tMax) {
			tX = 20.0f;
			tY = mWeapons[i]->GetPos().mY + mWeapons[i]->GetHeight() + tOffset;
		}
	}

	int index = 0;

	for (auto button : mCombinationButtons) {
		button->InActive();
		auto weapon = (*mPlWeapons)[index];
		CWeaponScript* curSc = weapon->GetComponent<CWeaponScript>();

		int count = 0;
		for (int i = 0; i < mPlWeapons->size(); i++) {
			if ((*mPlWeapons)[i]->GetID() == weapon->GetID() && (*mPlWeapons)[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) count++;

			if (count >= 2) {
				button->Active();
				count = 0;
				break;
			}
		}
		index++;
	}
}

CUIButton* CShopUI::MakeWeaponButton(std::vector<CWeapon*>* tWeapons, CWeapon* tWeapon, float tX, float tY)
{


	CWeaponScript* curSc = tWeapon->GetComponent<CWeaponScript>();
	auto currentWeapon = CDataMgr::GetWeaponDatas().find(tWeapon->GetID());

	if (currentWeapon == CDataMgr::GetWeaponDatas().end()) return nullptr;

	UINT color = 0;

	switch (curSc->GetTier()) {
	case 2:
		color = Gdiplus::Color::SteelBlue;
		break;
	case 3:
		color = Gdiplus::Color::BlueViolet;
		break;
	case 4:
		color = Gdiplus::Color::DarkRed;
		break;
	default:
		color = 0xFF111111;
		break;
	}

	CUIButton* weaponButton = new CUIButton(SVector2D(tX, tY), 75.0f, 75.0f);
	weaponButton->SetCornerRadius(10);
	weaponButton->SetBackColor(color);
	weaponButton->SetUsedClipping(true);

	mWeapons.push_back(weaponButton);

	mHaveWeaponPanel->AddChild(weaponButton);

	CUIImg* weaponImg = new CUIImg(SVector2D(), weaponButton->GetWidth(), weaponButton->GetHeight(), CResourceMgr::Find<CTexture>(currentWeapon->second.iconTexture));
	weaponImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	weaponButton->AddChild(weaponImg);




	CUIPanel* weaponDescPanel = new CUIPanel(SVector2D(), mShopMainPanel->GetWidth() / 4.0f - 30.0f, 0.0f);
	weaponDescPanel->SetCornerRadius(10);
	weaponDescPanel->InActive();
	weaponDescPanel->SetBackColor(0xDD000000);
	mWeaponsDescPanel.push_back(weaponDescPanel);

	mHaveWeaponPanel->AddChild(weaponDescPanel);

	CUIPanel* weaponImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	weaponImgPanel->SetCornerRadius(10);
	weaponImgPanel->SetBackColor(color);

	weaponDescPanel->AddChild(weaponImgPanel);
	mWeaponsImgPanel.push_back(weaponImgPanel);

	CUIImg* weaponDescImg = new CUIImg(SVector2D(), weaponImgPanel->GetWidth(), weaponImgPanel->GetHeight(), CResourceMgr::Find<CTexture>(currentWeapon->second.iconTexture));
	weaponDescImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	weaponImgPanel->AddChild(weaponDescImg);

	CUIText* weaponNameTex = new CUIText(SVector2D(weaponImgPanel->GetPos().mX + weaponImgPanel->GetWidth() + 10.0f, weaponDescPanel->GetPos().mY), 100.0f, 25.0f, currentWeapon->second.name);
	weaponNameTex->SetFontSize(20.0f);
	weaponNameTex->SetColor(Gdiplus::Color::White);

	weaponDescPanel->AddChild(weaponNameTex);

	CUIText* weaponCategoryTex = new CUIText(SVector2D(weaponImgPanel->GetPos().mX + weaponImgPanel->GetWidth() + 10.0f, weaponImgPanel->GetPos().mY + 30.0f), 100.0f, 25.0f, currentWeapon->second.classType);
	weaponCategoryTex->SetFontSize(15.0f);
	weaponCategoryTex->SetColor(Gdiplus::Color::LightYellow);

	weaponDescPanel->AddChild(weaponCategoryTex);

	CUIText* weaponDescTex = new CUIText(SVector2D(weaponImgPanel->GetPos().mX, weaponImgPanel->GetPos().mY + weaponImgPanel->GetHeight() + 10.0f), 200.0f, 0.0f);

	weaponDescTex->SetFontSize(15.0f);
	weaponDescTex->SetColor(Gdiplus::Color::White);

	weaponDescPanel->AddChild(weaponDescTex);
	mWeaponsDesc.push_back(weaponDescTex);

	std::wstring weaponDiscription = L"";

	auto& data = currentWeapon->second.tier[curSc->GetTier() - 1];

	// 깔끔한 wstring으로 바꿔주는 도우미 람다
	auto GetCleanVal = [](float value) -> std::wstring {
		std::wstring valStr = std::to_wstring(value);
		if (valStr.find(L'.') != std::wstring::npos) {
			valStr.erase(valStr.find_last_not_of(L'0') + 1);
			if (valStr.back() == L'.') valStr.pop_back();
		}
		return valStr;
		};

	// 텍스트를 붙여주는 도우미 람다
	auto descriptionStat = [&](std::wstring label, float value, std::wstring suffix = L"") {
		if (value <= 0.0f) return;
		weaponDiscription += L"<c=#FFFFE0>" + label + L":</c> " + GetCleanVal(value) + suffix + L"\n";
		};

	descriptionStat(L"데미지", data.damage);

	if (data.critDamagePer > 0.0f) {
		weaponDiscription += L"<c=#FFFFE0>치명타:</c> x" + GetCleanVal(data.critDamagePer);

		if (data.critChancePer > 0.0f) {
			weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
		}
		weaponDiscription += L"\n";
	}

	descriptionStat(L"쿨다운", data.delay, L"s");
	descriptionStat(L"범위", data.range);

	if (data.lifeSteal != 0.0f) {
		descriptionStat(L"생명 훔침", data.lifeSteal, L"%");
	}

	weaponDescTex->SetText(weaponDiscription);
	weaponDescTex->SetHeight(weaponDescTex->CalculateTextSize().Height);



	CUIButton* combinationButton = new CUIButton(SVector2D(weaponImgPanel->GetPos().mX, weaponDescTex->GetPos().mY + weaponDescTex->GetHeight() + 10.0f), weaponDescPanel->GetWidth() - 20.0f, 20.0f);
	combinationButton->InActive();
	combinationButton->SetBackColor(Gdiplus::Color::Gray);
	combinationButton->SetCornerRadius(10);
	mCombinationButtons.push_back(combinationButton);

	weaponDescPanel->AddChild(combinationButton);

	CUIText* combinationTex = new CUIText(SVector2D(), combinationButton->GetWidth(), combinationButton->GetHeight(), L"결합");
	combinationTex->SetColor(Gdiplus::Color::White);
	combinationTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	combinationButton->AddChild(combinationTex);

	combinationButton->SetEventHover([=]() {
		combinationButton->SetBackColor(Gdiplus::Color::White);
		combinationTex->SetColor(Gdiplus::Color::Black);
		});
	combinationButton->SetEventOutHover([=]() {
		combinationButton->SetBackColor(Gdiplus::Color::Gray);
		combinationTex->SetColor(Gdiplus::Color::White);
		});

	// 결합은 같은 아이디의 같은 티어의 무기가 2개 이상이라면 활성화 시켜줌
	int count = 0;
	for (int i = 0; i < tWeapons->size(); i++) {
		if ((*tWeapons)[i]->GetID() == tWeapon->GetID() && (*tWeapons)[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) count++;

		if (count >= 2) {
			combinationButton->Active();
			count = 0;
			break;
		}
	}

	// 재활용 버튼(무기 판매)
	CUIButton* recycleButton = new CUIButton(SVector2D(combinationButton->GetPos().mX, combinationButton->GetPos().mY + combinationButton->GetHeight() + 10.0f), weaponDescPanel->GetWidth() - 20.0f, 20.0f);
	recycleButton->SetBackColor(Gdiplus::Color::Gray);
	recycleButton->SetCornerRadius(10);

	weaponDescPanel->AddChild(recycleButton);
	mRecycleButtons.push_back(recycleButton);

	int recycleCost = data.basePrice / 3.0f;

	CUIText* recycleTex = new CUIText(SVector2D(), recycleButton->GetWidth(), recycleButton->GetHeight(), L"재활용 (+" + std::to_wstring(recycleCost) + L")");
	recycleTex->SetColor(Gdiplus::Color::White);
	recycleTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	recycleButton->AddChild(recycleTex);
	mRecycleTexts.push_back(recycleTex);

	recycleButton->SetEventHover([=]() {
		recycleButton->SetBackColor(Gdiplus::Color::White);
		recycleTex->SetColor(Gdiplus::Color::Black);
		});
	recycleButton->SetEventOutHover([=]() {
		recycleButton->SetBackColor(Gdiplus::Color::Gray);
		recycleTex->SetColor(Gdiplus::Color::White);
		});
	// 재활용 버튼 클릭 시 무기를 현재 지니고 있는 무기에서 삭제하는게 필요함
	recycleButton->SetEventClick([=]() {
		if ((*tWeapons).size() == 1) return;
		mPlSc->ChangeMoney(recycleCost);
		weaponDescPanel->InActive();

		for (int i = 0; i < (*tWeapons).size(); i++) {
			if ((*tWeapons)[i]->GetID() == tWeapon->GetID() && (*tWeapons)[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) {
				mPlWeaponMgr->RemoveWeapon(i);

				WeaponButtonRemove(i);
				break;
			}
		}

		if (count < 2) {
			combinationButton->InActive();
		}
		WeaponButtonsReSetting(20.0f, mHaveWeaponText->GetPos().mY + mHaveWeaponText->GetHeight() + 20.0f, 10.0f, mHaveWeaponPanel->GetWidth());
		weaponButton->SetSelected(false);
		});

	// 결합 버튼 또한 재활용 버튼과 마찬가지, 다른 점은 결합이 클릭된 무기를 업그레이드 시켜야 함.
	combinationButton->SetEventClick([=]() {
		curSc->SetTier(curSc->GetTier() + 1);

		for (int i = 0; i < (*tWeapons).size(); i++) {
			if ((*tWeapons)[i]->GetID() == tWeapon->GetID() && (*tWeapons)[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier() - 1) {
				mPlWeaponMgr->RemoveWeapon(i);

				WeaponButtonRemove(i);
				break;
			}
		}
		ReSettingWeaponButton(curSc, tWeapon, mHaveWeaponPanel, weaponButton, weaponDescTex, weaponImgPanel, recycleButton, recycleTex);
		WeaponButtonsReSetting(20.0f, mHaveWeaponText->GetPos().mY + mHaveWeaponText->GetHeight() + 20.0f, 10.0f, mHaveWeaponPanel->GetWidth());
		weaponDescPanel->InActive();
		weaponButton->SetSelected(false);
		});



	// 무기 버튼이 선택되어도 아무것도 안하고 넘길 수 있게 취소 버튼
	CUIButton* cancleButton = new CUIButton(SVector2D(recycleButton->GetPos().mX, recycleButton->GetPos().mY + recycleButton->GetHeight() + 10.0f), weaponDescPanel->GetWidth() - 20.0f, 20.0f);
	cancleButton->SetBackColor(Gdiplus::Color::Gray);
	cancleButton->SetCornerRadius(10);

	weaponDescPanel->AddChild(cancleButton);

	CUIText* cancleTex = new CUIText(SVector2D(), cancleButton->GetWidth(), cancleButton->GetHeight(), L"취소");
	cancleTex->SetColor(Gdiplus::Color::White);
	cancleTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	cancleButton->AddChild(cancleTex);

	cancleButton->SetEventHover([=]() {
		cancleButton->SetBackColor(Gdiplus::Color::White);
		cancleTex->SetColor(Gdiplus::Color::Black);
		});
	cancleButton->SetEventOutHover([=]() {
		cancleButton->SetBackColor(Gdiplus::Color::Gray);
		cancleTex->SetColor(Gdiplus::Color::White);
		});
	cancleButton->SetEventClick([=]() {
		weaponButton->SetSelected(false);
		weaponDescPanel->InActive();
		});




	weaponDescPanel->SetHeight(cancleButton->GetPos().mY + cancleButton->GetHeight() + 10.0f);
	weaponDescPanel->SetPos(SVector2D(weaponButton->GetPos().mX + weaponButton->GetWidth() - weaponDescPanel->GetWidth(), weaponButton->GetPos().mY - (10.0f + weaponDescPanel->GetHeight())));

	weaponButton->SetEventHover([=]() {
		weaponButton->SetBackColor(Gdiplus::Color::White);
		weaponDescPanel->Active();
		});
	weaponButton->SetEventOutHover([=]() {

		UINT curColor = 0;

		switch (curSc->GetTier()) {
		case 2:
			curColor = Gdiplus::Color::SteelBlue;
			break;
		case 3:
			curColor = Gdiplus::Color::BlueViolet;
			break;
		case 4:
			curColor = Gdiplus::Color::DarkRed;
			break;
		default:
			curColor = 0xFF111111;
			break;
		}

		weaponButton->SetBackColor(curColor);
		if (!weaponButton->IsSelected()) {
			weaponDescPanel->InActive();
		}
		});
	weaponButton->SetEventClick([=]() {
		weaponButton->SetSelected(true);
		});

	return weaponButton;
}

void CShopUI::WeaponButtonAdd(int tIndex)
{

}

void CShopUI::WeaponButtonRemove(int tIndex)
{
	if (mWeapons[tIndex] != nullptr) {
		mHaveWeaponPanel->RemoveChild(mWeapons[tIndex]);
		mWeapons[tIndex]->OnDestroy();
		SAFE_DELETE(mWeapons[tIndex]);
	}

	mWeapons.erase(mWeapons.begin() + tIndex);
	mWeaponsDescPanel.erase(mWeaponsDescPanel.begin() + tIndex);
	mWeaponsDesc.erase(mWeaponsDesc.begin() + tIndex);
	mWeaponsImgPanel.erase(mWeaponsImgPanel.begin() + tIndex);
	mRecycleButtons.erase(mRecycleButtons.begin() + tIndex);
	mRecycleTexts.erase(mRecycleTexts.begin() + tIndex);
	mCombinationButtons.erase(mCombinationButtons.begin() + tIndex);
}
