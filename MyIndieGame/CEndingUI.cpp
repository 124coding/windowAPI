#include "CEndingUI.h"

#include "Effect.h"

#include "CDataMgr.h"
#include "CUIMgr.h"

#include "CPlayScene.h"
#include "StatSet.h"

#include "CPlayer.h";
#include "CWeapon.h"

#include "CPlayerScript.h"
#include "CWeaponScript.h"
#include "CItemMgr.h"
#include "CWeaponMgr.h"

#include "CUIButton.h"

#include <regex>

void CEndingUI::OnCreate()
{
	// ==========================================
	// Data Retrieval (최종 데이터 수집)
	// ==========================================
	// 게임이 종료된 시점의 플레이어 스탯, 아이템, 무기 정보를 모두 가져옴
	CPlayer* pl = CPlayScene::GetPlayer();
	CPlayerScript* plSc = pl->GetComponent<CPlayerScript>(eComponentType::Script);
	CItemMgr* plItemMgr = pl->GetComponent<CItemMgr>(eComponentType::ItemMgr);
	CWeaponMgr* plWeaponMgr = pl->GetComponent<CWeaponMgr>(eComponentType::WeaponMgr);
	std::vector<std::pair<std::wstring, int>>* plItems = &plItemMgr->GetItems();
	std::vector<CWeapon*>* plWeapons = &plWeaponMgr->GetWeapons();



	SetWidth(windowWidth);
	SetHeight(windowHeight);

	// ==========================================
	// Win / Loss Condition Check
	// ==========================================
	// 5스테이지 클리어 & 체력이 0 초과인 경우 승리, 아니면 패배 처리
	CUIText* endingTex = new CUIText(SVector2D(), this->GetWidth(), 50.0f, L"Test");
	endingTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
	endingTex->SetColor(Gdiplus::Color::White);
	endingTex->SetFontSize(50.0f);
	endingTex->SetStrokeWidth(2.0f);
	endingTex->SetOutline(2.0f, Gdiplus::Color::Black);
	if (CPlayScene::GetStageNum() == 5 && CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script)->GetHP() > 0) {
		endingTex->SetText(L"달리기 성공!");
		endingTex->SetColor(Gdiplus::Color::Green);
	}
	else {
		endingTex->SetText(L"달리기 실패.");
		endingTex->SetColor(Gdiplus::Color::Red);
	}
	this->AddChild(endingTex);

	// 메인 배경 패널
	CUIPanel* endingMainPanel = new CUIPanel(SVector2D(100.0f, 100.0f), this->GetWidth() - 200.0f, this->GetHeight() - 200.0f);
	endingMainPanel->SetBackColor(0xFF222222);
	endingMainPanel->SetCornerRadius(10);

	this->AddChild(endingMainPanel);

	// ==========================================
	// Statistics Dashboard (최종 스탯 표시)
	// ==========================================
	// 좌측 패널에 레벨, 체력, 공격력 등 최종 스탯을 나열
	CUIPanel* statPanel = new CUIPanel(SVector2D(3.0f, 3.0f), endingMainPanel->GetWidth() / 4, endingMainPanel->GetHeight() - 6.0f);
	statPanel->SetBackColor(Gdiplus::Color::Black);
	statPanel->SetCornerRadius(10);

	endingMainPanel->AddChild(statPanel);

	CUIText* statTex = new CUIText(SVector2D(), statPanel->GetWidth(), 40.0f, L"능력치");
	statTex->SetFontSize(30.0f);
	statTex->SetColor(Gdiplus::Color::White);
	statTex->SetStrokeWidth(1.0f);
	statTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	statPanel->AddChild(statTex);

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

	// ==========================================
	// Inventory Visualization (최종 빌드 확인)
	// ==========================================
	// 우측 패널에 획득한 모든 무기와 아이템 아이콘을 나열
	CUIPanel* endingItemPanel = new CUIPanel(SVector2D(statPanel->GetPos().mX + statPanel->GetWidth() + 3.0f, 0.0f), endingMainPanel->GetWidth() - (statPanel->GetPos().mX + statPanel->GetWidth() + 3.0f), endingMainPanel->GetHeight());

	endingMainPanel->AddChild(endingItemPanel);

	// [Weapons Display]
	CUIPanel* weaponsPanel = new CUIPanel(SVector2D(), endingItemPanel->GetWidth(), endingItemPanel->GetHeight() / 3);

	CUIText* weaponTex = new CUIText(SVector2D(20.0f, 20.0f), 0.0f, 40.0f, L"무기");
	weaponTex->SetWidth(weaponTex->CalculateTextSize().Width);
	weaponTex->SetFontSize(30.0f);
	weaponTex->SetColor(Gdiplus::Color::White);
	weaponTex->SetStrokeWidth(1.0f);

	weaponsPanel->AddChild(weaponTex);

	// 무기 목록 순회 및 버튼 생성
	float x = 20.0f;
	float y = weaponTex->GetPos().mY + weaponTex->GetHeight() + 10.0f;
	float offset = 10.0f;

	for (int i = 0; i < plWeapons->size(); i++) {		
		// 화면 오른쪽 끝에 있는 아이템은 툴팁을 왼쪽으로 띄우기 위한 플래그(right) 설정
		bool right = false;
		if (i < 8) {
			right = true;
		}

		CUIButton* weaponButton = MakeWeaponButton(plWeapons, (*plWeapons)[i], x, y, right);
		weaponsPanel->AddChild(weaponButton);
		x += weaponButton->GetWidth() + offset;

		if (x + weaponButton->GetWidth() + offset > weaponsPanel->GetWidth()) {
			x = 20.0f;
			y = weaponButton->GetPos().mY + weaponButton->GetHeight() + offset;
		}
	}

	// [Items Display]
	CUIPanel* itemsPanel = new CUIPanel(SVector2D(0.0f, weaponsPanel->GetHeight()), endingItemPanel->GetWidth(), 2 * endingItemPanel->GetHeight() / 3);

	endingItemPanel->AddChild(itemsPanel);
	endingItemPanel->AddChild(weaponsPanel);

	CUIText* itemTex = new CUIText(SVector2D(20.0f, 20.0f), 0.0f, 40.0f, L"아이템");
	itemTex->SetWidth(itemTex->CalculateTextSize().Width);
	itemTex->SetFontSize(30.0f);
	itemTex->SetColor(Gdiplus::Color::White);
	itemTex->SetStrokeWidth(1.0f);

	itemsPanel->AddChild(itemTex);

	x = 20.0f;
	y = itemTex->GetPos().mY + itemTex->GetHeight() + 10.0f;

	for (int i = 0; i < plItems->size(); i++) {
		bool right = false;
		if (i < 8) {
			right = true;
		}

		// 화면 아래쪽에 있는 아이템은 툴팁을 위쪽으로 띄우기 위한 플래그(up) 설정
		bool up = false;
		if (y > 200.0f) {
			up = true;
		}

		CUIPanel* itemPanel = MakeItemPanel((*plItems)[i].first, x, y, right, up);
		itemsPanel->AddChild(itemPanel);
		x += itemPanel->GetWidth() + offset;

		if (x + itemPanel->GetWidth() + offset > itemsPanel->GetWidth()) {
			x = 20.0f;
			y = itemPanel->GetPos().mY + itemPanel->GetHeight() + offset;
		}
	}

	// ==========================================
	// Navigation Buttons (Game Loop Reset)
	// ==========================================
	CUIPanel* buttonPanel = new CUIPanel(SVector2D(), 3 * endingMainPanel->GetWidth() / 4, 40.0f);
	buttonPanel->SetPos(SVector2D(endingMainPanel->GetPos().mX + endingMainPanel->GetWidth() / 2 - buttonPanel->GetWidth() / 2, endingMainPanel->GetPos().mY + endingMainPanel->GetHeight() + 30.0f));

	this->AddChild(buttonPanel);

	// 1. [Restart Button] 같은 캐릭터/무기로 재시작
	CUIButton* restartButton = new CUIButton(SVector2D(10.0f, 0.0f), buttonPanel->GetWidth() / 4 - 20.0f, buttonPanel->GetHeight());
	restartButton->SetBackColor(Gdiplus::Color::Black);
	restartButton->SetCornerRadius(10);

	buttonPanel->AddChild(restartButton);

	CUIText* restartTex = new CUIText(SVector2D(), restartButton->GetWidth(), restartButton->GetHeight(), L"재시작");
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
		// 모든 진행 데이터 초기화
		plSc->ResetStats();
		plItemMgr->ResetItems();
		plWeaponMgr->ResetWeapons();

		// 기존 선택했던 캐릭터와 무기 정보 다시 로드 (Soft Reset)
		auto startChar = CDataMgr::GetCharacterDatas().find(plSc->GetStartingCharacterID());
		if (startChar == CDataMgr::GetCharacterDatas().end()) {
			return;
		}

		CDataMgr::SCharacter curChar = startChar->second;

		auto startWeapon = CDataMgr::GetWeaponDatas().find(plSc->GetStartWeaponID());
		if (startWeapon == CDataMgr::GetWeaponDatas().end()) {
			return;
		}
		CDataMgr::SWeapon curWeapon = startWeapon->second;


		// 외형 및 기본 무기 다시 지급
		plSc->SetClothTexture(CResourceMgr::Find<CTexture>(curChar.clothTexture));
		plSc->SetMouthTexture(CResourceMgr::Find<CTexture>(curChar.mouthTexture));
		plSc->SetEyesTexture(CResourceMgr::Find<CTexture>(curChar.eyesTexture));
		plSc->SetHairTexture(CResourceMgr::Find<CTexture>(curChar.hairTexture));

		if (curWeapon.ID[0] == L'M') {
			plWeaponMgr->PlusWeapon(eLayerType::MeleeWeapon, curWeapon.ID, 1);
		}
		else if (curWeapon.ID[0] == L'R') {
			plWeaponMgr->PlusWeapon(eLayerType::RangedWeapon, curWeapon.ID, 1);
		}

		// 캐릭터 고유 효과 재적용
		for (auto& [ID, args] : curChar.effects) {
			ApplyEffect(ID, args);
		}

		// 스테이지 초기화 후 플레이 씬으로 이동
		CPlayScene::ResetStageNum();
		CSceneMgr::LoadScene(L"PlayScene");
		});

	// 2. [New Run Button] 캐릭터 선택창으로 이동
	CUIButton* newStartButton = new CUIButton(SVector2D(restartButton->GetPos().mX + restartButton->GetWidth() + 20.0f, 0.0f), buttonPanel->GetWidth() / 4 - 20.0f, buttonPanel->GetHeight());
	newStartButton->SetBackColor(Gdiplus::Color::Black);
	newStartButton->SetCornerRadius(10);

	buttonPanel->AddChild(newStartButton);

	CUIText* newStartTex = new CUIText(SVector2D(), newStartButton->GetWidth(), newStartButton->GetHeight(), L"새로운 달리기");
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
		// 데이터 완전 초기화 (Hard Reset)
		plSc->ResetStats();
		plItemMgr->ResetItems();
		plWeaponMgr->ResetWeapons();

		// 선택 정보도 초기화
		plSc->SetStartingCharacterID(L"");
		plSc->SetStartWeaponID(L"");

		CPlayScene::ResetStageNum();
		CSceneMgr::LoadScene(L"SettingScene");		// 캐릭터 선택 씬으로 이동
		});


	// 3. [Menu Button] 타이틀 화면으로 이동
	CUIButton* returnMenuButton = new CUIButton(SVector2D(newStartButton->GetPos().mX + newStartButton->GetWidth() + 20.0f, 0.0f), buttonPanel->GetWidth() / 2 - 20.0f, buttonPanel->GetHeight());
	returnMenuButton->SetBackColor(Gdiplus::Color::Black);
	returnMenuButton->SetCornerRadius(10);

	buttonPanel->AddChild(returnMenuButton);

	CUIText* returnMenuTex = new CUIText(SVector2D(), returnMenuButton->GetWidth(), returnMenuButton->GetHeight(), L"메인 메뉴로 돌아가기");
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
		// 데이터 완전 초기화
		plSc->ResetStats();
		plItemMgr->ResetItems();
		plWeaponMgr->ResetWeapons();

		plSc->SetStartingCharacterID(L"");
		plSc->SetStartWeaponID(L"");

		CPlayScene::ResetStageNum();

		CSceneMgr::LoadScene(L"TitleScene");	// 타이틀 씬으로 이동

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

// ==========================================
// Dynamic UI Factory Methods
// ==========================================
CUIButton* CEndingUI::MakeWeaponButton(std::vector<CWeapon*>* tWeapons, CWeapon* tWeapon, float tX, float tY, bool tRight)
{
	CWeaponScript* curSc = tWeapon->GetComponent<CWeaponScript>(eComponentType::Script);
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
	// weaponButton->SetUsedClipping(true);

	CUIImg* weaponImg = new CUIImg(SVector2D(), weaponButton->GetWidth(), weaponButton->GetHeight(), CResourceMgr::Find<CTexture>(currentWeapon->second.iconTexture));
	weaponImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	weaponButton->AddChild(weaponImg);

	CUIPanel* weaponDescPanel = new CUIPanel(SVector2D(), 245.0f, 0.0f);
	weaponDescPanel->SetCornerRadius(10);
	weaponDescPanel->InActive();
	weaponDescPanel->SetBackColor(0xDD000000);

	weaponButton->AddChild(weaponDescPanel);

	CUIPanel* weaponImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	weaponImgPanel->SetCornerRadius(10);
	weaponImgPanel->SetBackColor(color);

	weaponDescPanel->AddChild(weaponImgPanel);

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

	weaponDescPanel->SetHeight(weaponDescTex->GetPos().mY + weaponDescTex->GetHeight() + 10.0f);

	float posX = 0.0f;
	float posY = 0.0f;

	if (!tRight) {
		posX = -weaponDescPanel->GetWidth() + weaponButton->GetWidth();
	}

	posY = weaponButton->GetHeight() + 10.0f;

	weaponDescPanel->SetPos(SVector2D(posX, posY));

	weaponButton->SetEventHover([=]() {
		weaponButton->SetBackColor(Gdiplus::Color::White);
		weaponDescPanel->Active();
		});
	weaponButton->SetEventOutHover([=]() {
		weaponButton->SetBackColor(color);
		weaponDescPanel->InActive();
		});

	return weaponButton;
}

CUIPanel* CEndingUI::MakeItemPanel(std::wstring tItemID, float tX, float tY, bool tRight, bool tUp) {
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




	CUIPanel* itemDescPanel = new CUIPanel(SVector2D(), 245.0f, 0.0f);
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

	itemImgPanel->AddChild(itemDescImg);

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

	float posX = 0.0f;
	float posY = 0.0f;

	if (!tRight) {
		posX = -itemDescPanel->GetWidth() + itemPanel->GetWidth();
	}

	if (tUp) {
		posY = (10.0f + itemDescPanel->GetHeight());
	}
	else {
		posY = itemPanel->GetHeight() + 10.0f;
	}

	itemDescPanel->SetPos(SVector2D(posX, posY));

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