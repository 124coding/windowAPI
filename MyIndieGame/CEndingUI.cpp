#include "CEndingUI.h"

#include "CPlayScene.h"
#include "StatSet.h"

#include "CPlayer.h";

#include "CPlayerScript.h"

#include "CUIButton.h"

void CEndingUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);


	CUIText* endingTex = new CUIText();
	endingTex->SetWidth(this->GetWidth());
	endingTex->SetHeight(50.0f);
	endingTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
	endingTex->SetText(L"Test");
	endingTex->SetColor(Gdiplus::Color::White);
	endingTex->SetFontSize(50.0f);
	endingTex->SetStrokeWidth(2.0f);
	endingTex->SetOutline(2.0f, Gdiplus::Color::Black);
	// 한 5라운드까지만 해서 갔는지 못갔는지 확인해서 Text 수정
	/*if () {

	}
	else {

	}*/
	this->AddChild(endingTex);

	CUIPanel* endingMainPanel = new CUIPanel();
	endingMainPanel->SetWidth(this->GetWidth() - 200.0f);
	endingMainPanel->SetHeight(this->GetHeight() - 200.0f);
	endingMainPanel->SetPos(SVector2D(100.0f, 100.0f));
	endingMainPanel->SetBackColor(0xFF222222);
	endingMainPanel->SetCornerRadius(10);

	this->AddChild(endingMainPanel);

	CUIPanel* statPanel = new CUIPanel();
	statPanel->SetWidth(endingMainPanel->GetWidth() / 4);
	statPanel->SetHeight(endingMainPanel->GetHeight() - 6.0f);
	statPanel->SetPos(SVector2D(3.0f, 3.0f));
	statPanel->SetBackColor(Gdiplus::Color::Black);
	statPanel->SetCornerRadius(10);

	endingMainPanel->AddChild(statPanel);

	CUIText* statTex = new CUIText();
	statTex->SetWidth(statPanel->GetWidth());
	statTex->SetText(L"능력치");
	statTex->SetHeight(40.0f);
	statTex->SetFontSize(30.0f);
	statTex->SetColor(Gdiplus::Color::White);
	statTex->SetStrokeWidth(1.0f);
	statTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	statPanel->AddChild(statTex);

	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

	CUIText* level = nullptr;
	CUIPanel* statLevel = MakeStat(statPanel->GetWidth(), 30.0f, L"UpgradeIcon", L"현재 레벨", level);
	statLevel->SetPos(SVector2D(20.0f, statTex->GetHeight() + statTex->GetPos().mY + 20.0f));
	statPanel->AddChild(statLevel);

	SettingStatTex(plSc->GetLevel(), level);

	float statPosY = (statPanel->GetHeight() - 30.0f - (statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f)) / 12;

	CUIText* maxHP = nullptr;
	CUIPanel* statMaxHP = MakeStat(statPanel->GetWidth(), 30.0f, L"MaxHP", L"최대 체력", maxHP);
	statMaxHP->SetPos(SVector2D(20.0f, statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f));
	statPanel->AddChild(statMaxHP);

	SettingStatTex(plSc->GetMaxHP(), maxHP);

	CUIText* HPRegen = nullptr;
	CUIPanel* statHPRegen = MakeStat(statPanel->GetWidth(), 30.0f, L"HPRegen", L"HP 재생", HPRegen);
	statHPRegen->SetPos(SVector2D(20.0f, statMaxHP->GetPos().mY + statPosY));
	statPanel->AddChild(statHPRegen);

	SettingStatTex(plSc->GetHPGeneration(), HPRegen);

	CUIText* HPSteal = nullptr;
	CUIPanel* statHPSteal = MakeStat(statPanel->GetWidth(), 30.0f, L"LifeSteal", L"% 생명 훔침", HPSteal);
	statHPSteal->SetPos(SVector2D(20.0f, statHPRegen->GetPos().mY + statPosY));
	statPanel->AddChild(statHPSteal);

	SettingStatTex(plSc->GetLifeSteal(), HPSteal);

	CUIText* damage = nullptr;
	CUIPanel* statDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"Damage", L"% 데미지", damage);
	statDamage->SetPos(SVector2D(20.0f, statHPSteal->GetPos().mY + statPosY));
	statPanel->AddChild(statDamage);

	SettingStatTex(plSc->GetDamagePercent(), damage);

	CUIText* meleeDamage = nullptr;
	CUIPanel* statMeleeDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"MeleeDamage", L"근거리 데미지", meleeDamage);
	statMeleeDamage->SetPos(SVector2D(20.0f, statDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statMeleeDamage);

	SettingStatTex(plSc->GetMeleeDamage(), meleeDamage);

	CUIText* rangedDamage = nullptr;
	CUIPanel* statRangedDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"RangedDamage", L"원거리 데미지", rangedDamage);
	statRangedDamage->SetPos(SVector2D(20.0f, statMeleeDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statRangedDamage);

	SettingStatTex(plSc->GetRangedDamage(), rangedDamage);

	CUIText* attackSpeed = nullptr;
	CUIPanel* statAttackSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"AttackSpeed", L"% 공격 속도", attackSpeed);
	statAttackSpeed->SetPos(SVector2D(20.0f, statRangedDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statAttackSpeed);

	SettingStatTex(plSc->GetAttackSpeedPercent(), attackSpeed);

	CUIText* critChance = nullptr;
	CUIPanel* statCritChance = MakeStat(statPanel->GetWidth(), 30.0f, L"CritChance", L"% 치명타율", critChance);
	statCritChance->SetPos(SVector2D(20.0f, statAttackSpeed->GetPos().mY + statPosY));
	statPanel->AddChild(statCritChance);

	SettingStatTex(plSc->GetCriticalChancePercent(), critChance);

	CUIText* range = nullptr;
	CUIPanel* statRange = MakeStat(statPanel->GetWidth(), 30.0f, L"Range", L"범위", range);
	statRange->SetPos(SVector2D(20.0f, statCritChance->GetPos().mY + statPosY));
	statPanel->AddChild(statRange);

	SettingStatTex(plSc->GetRange(), range);

	CUIText* armor = nullptr;
	CUIPanel* statArmor = MakeStat(statPanel->GetWidth(), 30.0f, L"Armor", L"방어구", armor);
	statArmor->SetPos(SVector2D(20.0f, statRange->GetPos().mY + statPosY));
	statPanel->AddChild(statArmor);

	SettingStatTex(plSc->GetArmor(), armor);

	CUIText* dodge = nullptr;
	CUIPanel* statDodge = MakeStat(statPanel->GetWidth(), 30.0f, L"Dodge", L"% 회피", dodge);
	statDodge->SetPos(SVector2D(20.0f, statArmor->GetPos().mY + statPosY));
	statPanel->AddChild(statDodge);

	SettingStatTex(plSc->GetDodge(), dodge);

	CUIText* speed = nullptr;
	CUIPanel* statSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"Speed", L"% 속도", speed);
	statSpeed->SetPos(SVector2D(20.0f, statDodge->GetPos().mY + statPosY));
	statPanel->AddChild(statSpeed);
	
	SettingStatTex(plSc->GetSpeedPercent(), speed);

	
	CUIPanel* endingItemPanel = new CUIPanel();
	endingItemPanel->SetWidth(endingMainPanel->GetWidth() - (statPanel->GetPos().mX + statPanel->GetWidth() + 3.0f));
	endingItemPanel->SetHeight(endingMainPanel->GetHeight());
	endingItemPanel->SetPos(SVector2D(statPanel->GetPos().mX + statPanel->GetWidth() + 3.0f, 0.0f));

	endingMainPanel->AddChild(endingItemPanel);

	CUIPanel* weaponPanel = new CUIPanel();
	weaponPanel->SetWidth(endingItemPanel->GetWidth());
	weaponPanel->SetHeight(endingItemPanel->GetHeight() / 3);
	weaponPanel->SetPos(SVector2D());

	endingItemPanel->AddChild(weaponPanel);

	CUIText* weaponTex = new CUIText();
	weaponTex->SetText(L"무기");
	weaponTex->SetWidth(weaponTex->CalculateTextSize().Width);
	weaponTex->SetHeight(40.0f);
	weaponTex->SetFontSize(30.0f);
	weaponTex->SetColor(Gdiplus::Color::White);
	weaponTex->SetStrokeWidth(1.0f);
	weaponTex->SetPos(SVector2D(20.0f, 20.0f));

	weaponPanel->AddChild(weaponTex);

	/* 가지고 있던 무기들 구현 (ShopUI랑 똑같이)*/

	CUIPanel* itemPanel = new CUIPanel();
	itemPanel->SetWidth(endingItemPanel->GetWidth());
	itemPanel->SetHeight(2 * endingItemPanel->GetHeight() / 3);
	itemPanel->SetPos(SVector2D(0.0f, weaponPanel->GetHeight()));

	endingItemPanel->AddChild(itemPanel);

	CUIText* itemTex = new CUIText();
	itemTex->SetText(L"아이템");
	itemTex->SetWidth(itemTex->CalculateTextSize().Width);
	itemTex->SetHeight(40.0f);
	itemTex->SetFontSize(30.0f);
	itemTex->SetColor(Gdiplus::Color::White);
	itemTex->SetStrokeWidth(1.0f);
	itemTex->SetPos(SVector2D(20.0f, 20.0f));

	itemPanel->AddChild(itemTex);

	/* 가지고 있던 아이템들 구현 (ShopUI랑 똑같이)*/


	CUIPanel* buttonPanel = new CUIPanel();
	buttonPanel->SetWidth(3 * endingMainPanel->GetWidth() / 4);
	buttonPanel->SetHeight(40.0f);
	buttonPanel->SetPos(SVector2D(endingMainPanel->GetPos().mX + endingMainPanel->GetWidth() / 2 - buttonPanel->GetWidth() / 2, endingMainPanel->GetPos().mY + endingMainPanel->GetHeight() + 30.0f));

	this->AddChild(buttonPanel);

	CUIButton* restartButton = new CUIButton();
	restartButton->SetWidth(buttonPanel->GetWidth() / 4 - 20.0f);
	restartButton->SetHeight(buttonPanel->GetHeight());
	restartButton->SetPos(SVector2D(10.0f, 0.0f));
	restartButton->SetBackColor(Gdiplus::Color::Black);
	restartButton->SetCornerRadius(10);

	buttonPanel->AddChild(restartButton);

	CUIText* restartTex = new CUIText();
	restartTex->SetText(L"재시작");
	restartTex->SetWidth(restartButton->GetWidth());
	restartTex->SetHeight(restartButton->GetHeight());
	restartTex->SetFontSize(30.0f);
	restartTex->SetColor(Gdiplus::Color::White);
	restartTex->SetStrokeWidth(1.0f);
	restartTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	restartButton->AddChild(restartTex);

	restartButton->SetEventHover([=]() {
		restartTex->SetColor(Gdiplus::Color::Black);
		restartButton->SetBackColor(Gdiplus::Color::White);
		});
	restartButton->SetEventOutHover([=]() {
		restartTex->SetColor(Gdiplus::Color::White);
		restartButton->SetBackColor(Gdiplus::Color::Black);
		});
	restartButton->SetEventClick([=]() {
		/*버튼 클릭 구현*/
		});


	CUIButton* newStartButton = new CUIButton();
	newStartButton->SetWidth(buttonPanel->GetWidth() / 4 - 20.0f);
	newStartButton->SetHeight(buttonPanel->GetHeight());
	newStartButton->SetPos(SVector2D(restartButton->GetPos().mX + restartButton->GetWidth() + 20.0f, 0.0f));
	newStartButton->SetBackColor(Gdiplus::Color::Black);
	newStartButton->SetCornerRadius(10);

	buttonPanel->AddChild(newStartButton);

	CUIText* newStartTex = new CUIText();
	newStartTex->SetText(L"새로운 달리기");
	newStartTex->SetWidth(newStartButton->GetWidth());
	newStartTex->SetHeight(newStartButton->GetHeight());
	newStartTex->SetFontSize(30.0f);
	newStartTex->SetColor(Gdiplus::Color::White);
	newStartTex->SetStrokeWidth(1.0f);
	newStartTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	newStartButton->AddChild(newStartTex);

	newStartButton->SetEventHover([=]() {
		newStartTex->SetColor(Gdiplus::Color::Black);
		newStartButton->SetBackColor(Gdiplus::Color::White);
		});
	newStartButton->SetEventOutHover([=]() {
		newStartTex->SetColor(Gdiplus::Color::White);
		newStartButton->SetBackColor(Gdiplus::Color::Black);
		});
	newStartButton->SetEventClick([=]() {
		/*버튼 클릭 구현*/

		});


	CUIButton* returnMenuButton = new CUIButton();
	returnMenuButton->SetWidth(buttonPanel->GetWidth() / 2 - 20.0f);
	returnMenuButton->SetHeight(buttonPanel->GetHeight());
	returnMenuButton->SetPos(SVector2D(newStartButton->GetPos().mX + newStartButton->GetWidth() + 20.0f, 0.0f));
	returnMenuButton->SetBackColor(Gdiplus::Color::Black);
	returnMenuButton->SetCornerRadius(10);

	buttonPanel->AddChild(returnMenuButton);

	CUIText* returnMenuTex = new CUIText();
	returnMenuTex->SetText(L"메인 메뉴로 돌아가기");
	returnMenuTex->SetWidth(returnMenuButton->GetWidth());
	returnMenuTex->SetHeight(returnMenuButton->GetHeight());
	returnMenuTex->SetFontSize(30.0f);
	returnMenuTex->SetColor(Gdiplus::Color::White);
	returnMenuTex->SetStrokeWidth(1.0f);
	returnMenuTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	returnMenuButton->AddChild(returnMenuTex);

	returnMenuButton->SetEventHover([=]() {
		returnMenuTex->SetColor(Gdiplus::Color::Black);
		returnMenuButton->SetBackColor(Gdiplus::Color::White);
		});
	returnMenuButton->SetEventOutHover([=]() {
		returnMenuTex->SetColor(Gdiplus::Color::White);
		returnMenuButton->SetBackColor(Gdiplus::Color::Black);
		});
	returnMenuButton->SetEventClick([=]() {
		/*버튼 클릭 구현*/

		});

	CUIBase::OnCreate();
}

void CEndingUI::Active()
{
	CUIBase::Active();
}

void CEndingUI::InActive()
{
	CUIBase::InActive();
}

void CEndingUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CEndingUI::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CEndingUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CEndingUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CEndingUI::UIClear()
{
	CUIBase::UIClear();
}
