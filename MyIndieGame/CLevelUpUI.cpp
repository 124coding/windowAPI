#include "CLevelUpUI.h"

#include "StatSet.h"
#include "Effect.h"

#include "CDataMgr.h"
#include "CUIMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CPlayerScript.h"

#include "CUIPanel.h"
#include "CUIButton.h"
#include "CUIText.h"
#include "CUIImg.h"

void CLevelUpUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);

	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

	CUIText* levelUp = new CUIText(SVector2D(), windowWidth, windowHeight / 4, L"레벨 업!");
	levelUp->SetFontSize(50.0f);
	levelUp->SetColor(Gdiplus::Color::White);
	levelUp->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentFar);
	levelUp->SetStrokeWidth(1.0f);
	levelUp->SetOutline(1.0f, Gdiplus::Color::Black);

	this->AddChild(levelUp);


	mLevelUpMainPanel = new CUIPanel(SVector2D(0.0f, levelUp->GetPos().mY + levelUp->GetHeight()), 3 * windowWidth / 4, 2 * windowHeight / 3);
	this->AddChild(mLevelUpMainPanel);

	float x = 5.0f;
	float offset = 5.0f;

	for (int i = 0; i < 4; i++) {
		std::pair<std::wstring, CUIPanel*> statUpPanel = MakeStatUpPanel(x, mLevelUpMainPanel->GetHeight() / 4, CPlayScene::GetStageNum());
		if (statUpPanel.first == L"") {
			i--;
			continue;
		}
		mStatUpPanels.push_back(statUpPanel);
		x += statUpPanel.second->GetWidth() + offset;
	}




	mResetButton = new CUIButton(SVector2D(), 0.0f, 50.0f);
	mResetButton->SetCornerRadius(10);
	mResetButton->SetBackColor(Gdiplus::Color::Black);

	mLevelUpMainPanel->AddChild(mResetButton);

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
	mResetButton->SetEventClick([=]() {
		if (plSc->GetMoney() > mResetCost) {
			plSc->ChangeMoney(-mResetCost);
			mResetCount++;
			mResetCost *= 2;
		}
		else {
			return;
		}
		ResetLevelUpPanels(CPlayScene::GetStageNum());
		});



	mStatPanel = new CUIPanel(SVector2D(mLevelUpMainPanel->GetWidth(), mLevelUpMainPanel->GetPos().mY), this->GetWidth() - mLevelUpMainPanel->GetWidth(), 2 * windowHeight / 3);
	mStatPanel->SetBackColor(0xCC000000);
	mStatPanel->SetCornerRadius(10);
	this->AddChild(mStatPanel);

	CUIText* statTex = new CUIText(SVector2D(), 0.0f, 0.0f, L"능력치");
	statTex->SetFontSize(30.0f);
	statTex->SetStrokeWidth(1.0f);
	statTex->SetOutline(2.0f, Gdiplus::Color::Black);
	statTex->SetWidth(statTex->CalculateTextSize().Width);
	statTex->SetHeight(statTex->CalculateTextSize().Height);
	statTex->SetPos(SVector2D(mStatPanel->GetWidth() / 2 - statTex->GetWidth() / 2, 5.0f));
	statTex->SetColor(Gdiplus::Color::White);

	mStatPanel->AddChild(statTex);

	float statPosY = (mStatPanel->GetHeight() - 30.0f - (statTex->GetHeight() + statTex->GetPos().mY + 50.0f)) / 12;

	CUIPanel* statMaxHP = MakeStat(mStatPanel->GetWidth(), 30.0f, L"MaxHP", L"최대 체력", mMaxHP);
	statMaxHP->SetPos(SVector2D(20.0f, statTex->GetHeight() + statTex->GetPos().mY + 50.0f));
	mStatPanel->AddChild(statMaxHP);

	CUIPanel* statHPRegen = MakeStat(mStatPanel->GetWidth(), 30.0f, L"HPRegen", L"HP 재생", mHPRegen);
	statHPRegen->SetPos(SVector2D(20.0f, statMaxHP->GetPos().mY + statPosY));
	mStatPanel->AddChild(statHPRegen);

	CUIPanel* statHPSteal = MakeStat(mStatPanel->GetWidth(), 30.0f, L"LifeSteal", L"% 생명 훔침", mHPSteal);
	statHPSteal->SetPos(SVector2D(20.0f, statHPRegen->GetPos().mY + statPosY));
	mStatPanel->AddChild(statHPSteal);




	CUIPanel* statDamage = MakeStat(mStatPanel->GetWidth(), 30.0f, L"Damage", L"% 데미지", mDamage);
	statDamage->SetPos(SVector2D(20.0f, statHPSteal->GetPos().mY + statPosY));
	mStatPanel->AddChild(statDamage);

	CUIPanel* statMeleeDamage = MakeStat(mStatPanel->GetWidth(), 30.0f, L"MeleeDamage", L"근거리 데미지", mMeleeDamage);
	statMeleeDamage->SetPos(SVector2D(20.0f, statDamage->GetPos().mY + statPosY));
	mStatPanel->AddChild(statMeleeDamage);

	CUIPanel* statRangedDamage = MakeStat(mStatPanel->GetWidth(), 30.0f, L"RangedDamage", L"원거리 데미지", mRangedDamage);
	statRangedDamage->SetPos(SVector2D(20.0f, statMeleeDamage->GetPos().mY + statPosY));
	mStatPanel->AddChild(statRangedDamage);

	CUIPanel* statAttackSpeed = MakeStat(mStatPanel->GetWidth(), 30.0f, L"AttackSpeed", L"% 공격 속도", mAttackSpeed);
	statAttackSpeed->SetPos(SVector2D(20.0f, statRangedDamage->GetPos().mY + statPosY));
	mStatPanel->AddChild(statAttackSpeed);

	CUIPanel* statCritChance = MakeStat(mStatPanel->GetWidth(), 30.0f, L"CritChance", L"% 치명타율", mCritChance);
	statCritChance->SetPos(SVector2D(20.0f, statAttackSpeed->GetPos().mY + statPosY));
	mStatPanel->AddChild(statCritChance);

	CUIPanel* statRange = MakeStat(mStatPanel->GetWidth(), 30.0f, L"Range", L"범위", mRange);
	statRange->SetPos(SVector2D(20.0f, statCritChance->GetPos().mY + statPosY));
	mStatPanel->AddChild(statRange);

	CUIPanel* statArmor = MakeStat(mStatPanel->GetWidth(), 30.0f, L"Armor", L"방어구", mArmor);
	statArmor->SetPos(SVector2D(20.0f, statRange->GetPos().mY + statPosY));
	mStatPanel->AddChild(statArmor);

	CUIPanel* statDodge = MakeStat(mStatPanel->GetWidth(), 30.0f, L"Dodge", L"% 회피", mDodge);
	statDodge->SetPos(SVector2D(20.0f, statArmor->GetPos().mY + statPosY));
	mStatPanel->AddChild(statDodge);

	CUIPanel* statSpeed = MakeStat(mStatPanel->GetWidth(), 30.0f, L"Speed", L"% 속도", mSpeed);
	statSpeed->SetPos(SVector2D(20.0f, statDodge->GetPos().mY + statPosY));
	mStatPanel->AddChild(statSpeed);


	CUIBase::OnCreate();
}

void CLevelUpUI::Active()
{
	CUIBase::Active();
}

void CLevelUpUI::InActive()
{
	CUIBase::InActive();
}

void CLevelUpUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CLevelUpUI::OnUpdate(float tDeltaTime)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

	mResetTex->SetText(L"초기화 -" + std::to_wstring(mResetCost));
	mResetTex->SetWidth(mResetTex->CalculateTextSize().Width);
	mResetTex->SetPos(SVector2D(mResetButton->GetWidth() / 2 - 2 * mResetTex->GetWidth() / 3, 0.0f));

	mResetImg->SetPos(SVector2D(mResetTex->GetPos().mX + mResetTex->GetWidth() + 5.0f, 0.0f));

	mResetButton->SetWidth(mResetTex->GetWidth() + 60.0f);
	mResetButton->SetPos(SVector2D(mLevelUpMainPanel->GetWidth() / 2 - mResetButton->GetWidth() / 2, mStatUpPanels[0].second->GetPos().mY + mStatUpPanels[0].second->GetHeight() + 20.0f));

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

void CLevelUpUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CLevelUpUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CLevelUpUI::UIClear()
{
	CUIBase::UIClear();
}

std::pair<std::wstring, CUIPanel*> CLevelUpUI::MakeStatUpPanel(float tX, float tY, int tStageNum)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	std::pair<int, CDataMgr::SUpgrades> curStat = ChooseRandomUpgradeByStageNum(tStageNum);

	for (auto& id : mStatUpPanels) {
		if (id.first == curStat.second.ID) {
			return std::make_pair(L"", nullptr);
		}
	}

	UINT color = 0;
	std::wstring tierTex = L"";

	switch (curStat.first) {
	case 2:
		color = Gdiplus::Color::SteelBlue;
		tierTex = L"II";
		break;
	case 3:
		color = Gdiplus::Color::BlueViolet;
		tierTex = L"III";
		break;
	case 4:
		color = Gdiplus::Color::DarkRed;
		tierTex = L"IV";
		break;
	default:
		color = 0xFF111111;
		break;
	}

	CUIPanel* statUpPanel = new CUIPanel(SVector2D(tX, tY), mLevelUpMainPanel->GetWidth() / 4.0f - 10.0f, 240.0f);
	statUpPanel->SetCornerRadius(10);
	statUpPanel->SetBackColor(Gdiplus::Color::Black);

	mLevelUpMainPanel->AddChild(statUpPanel);

	CUIPanel* statUpImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	statUpImgPanel->SetCornerRadius(10);
	statUpImgPanel->SetBackColor(color);

	statUpPanel->AddChild(statUpImgPanel);

	CUIImg* statUpImg = new CUIImg(SVector2D(), statUpImgPanel->GetWidth(), statUpImgPanel->GetHeight(), CResourceMgr::Find<CTexture>(curStat.second.texName));
	statUpImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	statUpImgPanel->AddChild(statUpImg);

	CUIText* statUpNameTex = new CUIText(SVector2D(statUpImgPanel->GetPos().mX + statUpImgPanel->GetWidth() + 10.0f, statUpImgPanel->GetPos().mY), 100.0f, 25.0f, curStat.second.texName + L" " + tierTex);

	statUpNameTex->SetFontSize(20.0f);
	statUpNameTex->SetColor(Gdiplus::Color::White);

	statUpPanel->AddChild(statUpNameTex);

	CUIText* statUpCategoryTex = new CUIText(SVector2D(statUpImgPanel->GetPos().mX + statUpImgPanel->GetWidth() + 10.0f, statUpImgPanel->GetPos().mY + 30.0f), 100.0f, 25.0f, L"업그레이드");
	statUpCategoryTex->SetFontSize(15.0f);
	statUpCategoryTex->SetColor(Gdiplus::Color::LightYellow);

	statUpPanel->AddChild(statUpCategoryTex);

	CUIText* statUpDescTex = new CUIText(SVector2D(statUpImgPanel->GetPos().mX, statUpImgPanel->GetPos().mY + statUpImgPanel->GetHeight() + 10.0f), 200.0f, 30.0f);
	statUpDescTex->SetFontSize(15.0f);
	statUpDescTex->SetColor(Gdiplus::Color::White);

	std::wstring value = curStat.second.tiers[curStat.first - 1].arg.value;

	std::wstring colorStr = L"#FFFFFF"; // 기본값
	if (curStat.second.tiers[curStat.first - 1].arg.color != L"") {
		colorStr = curStat.second.tiers[curStat.first - 1].arg.color;
	}

	if (colorStr == L"#00FF00")
	{
		try {
			int iVal = std::stoi(value);

			if (iVal > 0) {
				value = L"+" + value;
			}
		}
		catch (...) {
			// 변환 실패
		}
	}

	std::wstring taggedStr = L"<c=" + colorStr + L">" + value + L"</c>";

	statUpDescTex->SetText(taggedStr + L" " + curStat.second.name);

	statUpPanel->AddChild(statUpDescTex);

	CUIButton* chooseButton = new CUIButton(SVector2D(5.0f, statUpPanel->GetHeight() - 55.0f), statUpPanel->GetWidth() - 10.0f, 50.0f);
	chooseButton->SetBackColor(Gdiplus::Color::Gray);
	chooseButton->SetCornerRadius(10);

	statUpPanel->AddChild(chooseButton);

	CUIText* chooseText = new CUIText(SVector2D(), chooseButton->GetWidth(), chooseButton->GetHeight(), L"선택");
	chooseText->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
	chooseText->SetColor(Gdiplus::Color::White);

	chooseButton->AddChild(chooseText);

	chooseButton->SetEventHover([=]() {
		chooseButton->SetBackColor(Gdiplus::Color::White);
		chooseText->SetColor(Gdiplus::Color::Black);
		});
	chooseButton->SetEventOutHover([=]() {
		chooseButton->SetBackColor(Gdiplus::Color::Gray);
		chooseText->SetColor(Gdiplus::Color::White);
		});
	chooseButton->SetEventClick([=]() {
		std::vector<CDataMgr::SArg> arg;
		arg.push_back(curStat.second.tiers[curStat.first - 1].arg);
		ApplyEffect(curStat.second.effectID, arg);

		plSc->MinusCurStage();
		CUIMgr::CurHoverNull();

		if (plSc->GetCurStageLevelUpCount() <= 0) {
			CPlayScene::GetPlayer()->GetComponent<CTransform>()->SetPos(SVector2D(windowWidth / 2, windowHeight / 2 + 55.0f));
			ResetLevelUpPanels(CPlayScene::GetStageNum() + 1);
			CSceneMgr::LoadScene(L"ShopScene");
		}
		else {
			ResetLevelUpPanels(CPlayScene::GetStageNum());
		}
		});

	return std::make_pair(curStat.second.ID, statUpPanel);
}

std::pair<int, CDataMgr::SUpgrades> CLevelUpUI::ChooseRandomUpgradeByStageNum(int tStageNum)
{
	int rand = std::rand() % CDataMgr::GetUpgradeDatas().size();
	auto it = std::next(CDataMgr::GetUpgradeDatas().begin(), rand);
	
	int rarityRand = std::rand() % 100;
	int targetTier = 1;
	switch (tStageNum) {
	case 0:
	case 1:
		if (rarityRand < 80) {
			targetTier = 1;
		}
		else {
			targetTier = 2;
		}
		break;
	case 2:
		if (rarityRand < 80) {
			targetTier = 1;
		}
		else if (rarityRand < 95) {
			targetTier = 2;
		}
		else {
			targetTier = 3;
		}
		break;
	case 3:
		if (rarityRand < 70) {
			targetTier = 1;
		}
		else if (rarityRand < 90) {
			targetTier = 2;
		}
		else {
			targetTier = 3;
		}
		break;
	case 4:
		if (rarityRand < 60) {
			targetTier = 1;
		}
		else if (rarityRand < 70) {
			targetTier = 2;
		}
		else if (rarityRand < 98) {
			targetTier = 3;
		}
		else {
			targetTier = 4;
		}
		break;
	case 5:
		if (rarityRand < 55) {
			targetTier = 1;
		}
		else if (rarityRand < 65) {
			targetTier = 2;
		}
		else if (rarityRand < 90) {
			targetTier = 3;
		}
		else {
			targetTier = 4;
		}
		break;
	default:
		if (rarityRand < 30) {
			targetTier = 1;
		}
		else if (rarityRand < 55) {
			targetTier = 2;
		}
		else if (rarityRand < 80) {
			targetTier = 3;
		}
		else {
			targetTier = 4;
		}
		break;
	}
	return std::make_pair(targetTier, it->second);
}

void CLevelUpUI::ResetLevelUpPanels(int tStageNum)
{
	for (int i = 0; i < 4; ++i)
	{
		if (mStatUpPanels[i].second != nullptr)
		{
			mStatUpPanels[i].second->GetParent()->RemoveChild(mStatUpPanels[i].second);
			mStatUpPanels[i].second->OnDestroy();
			SAFE_DELETE(mStatUpPanels[i].second);
		}
	}

	mStatUpPanels.clear();

	float x = 5.0f;
	float offset = 5.0f;

	for (int i = 0; i < 4; i++) {
		std::pair<std::wstring, CUIPanel*> statUpPanel = MakeStatUpPanel(x, mLevelUpMainPanel->GetHeight() / 4, tStageNum);
		if (statUpPanel.first == L"") {
			i--;
			continue;
		}
		mStatUpPanels.push_back(statUpPanel);
		x += statUpPanel.second->GetWidth() + offset;
	}
}
