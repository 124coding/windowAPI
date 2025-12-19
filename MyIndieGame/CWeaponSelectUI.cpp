#include "CWeaponSelectUI.h"

#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CDataMgr.h"

#include "CCharacterSelectUI.h"
#include "CUIButton.h"
#include "CUIPanel.h"
#include "CUIText.h"
#include "CUIImg.h"

#include "CPlayer.h"

#include "CPlayScene.h"
#include "CSettingScene.h"

#include "CPlayerScript.h"
#include "CWeaponMgr.h"

#include "Effect.h"

#include <regex>

void CWeaponSelectUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);


	auto it = CDataMgr::GetCharacterDatas().find(CPlayScene::GetPlayer()->GetComponent<CPlayerScript>()->GetStartingCharacterID());
	if (it == CDataMgr::GetCharacterDatas().end()) {
		return;
	}

	CDataMgr::SCharacter curChar = it->second;

	// 뒤로가기 버튼
	CUIButton* backButton = new CUIButton(SVector2D(windowWidth / 20, windowHeight / 20), 200.0f, 50.0f);
	backButton->SetBackColor(Gdiplus::Color::Black);
	backButton->SetCornerRadius(10.0f);

	CUIText* backButtonTex = new CUIText(SVector2D(), backButton->GetWidth(), backButton->GetHeight(), L"뒤로");

	backButtonTex->SetFontSize(24.0f);
	backButtonTex->SetStrokeWidth(1.0f);
	backButtonTex->SetColor(Gdiplus::Color::White);
	backButtonTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	backButton->AddChild(backButtonTex);

	backButton->SetEventHover([=]() {
		backButtonTex->SetColor(Gdiplus::Color::Black);
		backButton->SetBackColor(Gdiplus::Color::White);
		});

	backButton->SetEventOutHover([=]() {
		backButtonTex->SetColor(Gdiplus::Color::White);
		backButton->SetBackColor(Gdiplus::Color::Black);
		});

	backButton->SetEventClick([=]() {
		this->InActive();
		CUIMgr::FindUI(eUIType::CharacterSelectUI)->Active();
		mCharDescriptionPanel->Reparent(CUIMgr::FindUI(eUIType::CharacterSelectUI), true);
		CUIMgr::ClearUI(eUIType::WeaponSelectUI);
		});
	this->AddChild(backButton);

	// 현재 창 텍스트
	CUIText* currentUITex = new CUIText(SVector2D(), this->GetWidth(), this->GetHeight() / 5, L"무기 선택");

	currentUITex->SetFontSize(40.0f);
	currentUITex->SetStrokeWidth(1.0f);
	currentUITex->SetOutline(3.0f, Gdiplus::Color::Black);
	currentUITex->SetColor(Gdiplus::Color::White);
	currentUITex->SetPos(SVector2D(0.0f, currentUITex->GetHeight() / 3));
	currentUITex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	this->AddChild(currentUITex);

	mCharDescriptionPanel = dynamic_cast<CUIPanel*>(dynamic_cast<CCharacterSelectUI*>(CUIMgr::FindUI(eUIType::CharacterSelectUI))->GetCharDescPanel()->Reparent(this, true));

	// 무기 설명 패널
	CUIPanel* descriptionWeaponPanel = new CUIPanel(SVector2D(), 250.0f, 320.0f);

	descriptionWeaponPanel->SetBackColor(Gdiplus::Color::Black);
	descriptionWeaponPanel->SetCornerRadius(10.0f);
	descriptionWeaponPanel->InActive();

	this->AddChild(descriptionWeaponPanel);

	CUIPanel* weaponDescriptionImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);

	weaponDescriptionImgPanel->SetBackColor(Gdiplus::Color::Black);
	weaponDescriptionImgPanel->SetCornerRadius(10.0f);

	descriptionWeaponPanel->AddChild(weaponDescriptionImgPanel);

	CUIImg* weaponDescriptionImg = new CUIImg(SVector2D(), weaponDescriptionImgPanel->GetWidth(), weaponDescriptionImgPanel->GetHeight());

	weaponDescriptionImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	weaponDescriptionImgPanel->AddChild(weaponDescriptionImg);

	CUIText* weaponNameTex = new CUIText(SVector2D(weaponDescriptionImgPanel->GetPos().mX + weaponDescriptionImgPanel->GetWidth() + 10.0f, weaponDescriptionImgPanel->GetPos().mY), 100.0f, 25.0f);
	weaponNameTex->SetFontSize(20.0f);
	weaponNameTex->SetColor(Gdiplus::Color::White);

	descriptionWeaponPanel->AddChild(weaponNameTex);

	CUIText* weaponTex = new CUIText(SVector2D(weaponDescriptionImgPanel->GetPos().mX + weaponDescriptionImgPanel->GetWidth() + 10.0f, weaponDescriptionImgPanel->GetPos().mY + 30.0f), 100.0f, 25.0f);
	weaponTex->SetFontSize(15.0f);
	weaponTex->SetColor(Gdiplus::Color::LightYellow);

	descriptionWeaponPanel->AddChild(weaponTex);

	CUIText* descriptionWeaponTex = new CUIText(SVector2D(weaponDescriptionImgPanel->GetPos().mX, weaponDescriptionImgPanel->GetPos().mY + weaponDescriptionImgPanel->GetHeight() + 10.0f), 200.0f, 250.0f);

	descriptionWeaponTex->SetFontSize(15.0f);
	descriptionWeaponTex->SetColor(Gdiplus::Color::White);

	descriptionWeaponPanel->AddChild(descriptionWeaponTex);

	CUIPanel* weaponSelectPanel = new CUIPanel(SVector2D(this->GetPos().mX, this->GetPos().mY + this->GetHeight() / 2), this->GetWidth(), this->GetHeight() / 2);

	this->AddChild(weaponSelectPanel);

	int x = 30;
	int y = 100;
	int i = 1;

	// 캐릭터 무기 데이터 가져와서 버튼 만들기
	for (auto& weapon : curChar.weapons) {
		auto it = CDataMgr::GetWeaponDatas().find(weapon);
		if (it == CDataMgr::GetWeaponDatas().end()) {
			continue;
		}
		CDataMgr::SWeapon curWeapon = it->second;

		CUIButton* weaponButton = new CUIButton(SVector2D(x, y), 75.0f, 75.0f);
		weaponButton->SetBackColor(Gdiplus::Color::Gray);
		weaponButton->SetCornerRadius(10.0f);

		CUIImg* uiImg = new CUIImg(SVector2D(), weaponButton->GetWidth(), weaponButton->GetHeight(), CResourceMgr::Find<CTexture>(curWeapon.iconTexture));
		uiImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		weaponButton->AddChild(uiImg);

		weaponSelectPanel->AddChild(weaponButton);

		std::wstring weaponDiscription = L"";

		auto& data = curWeapon.tier[0];

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


		weaponButton->SetEventHover([=]() {
			weaponButton->SetBackColor(Gdiplus::Color::White);
			descriptionWeaponPanel->Active();
			mCharDescriptionPanel->SetPos(SVector2D(this->GetWidth() / 2 - mCharDescriptionPanel->GetWidth() - 10, mCharDescriptionPanel->GetPos().mY));
			descriptionWeaponPanel->SetPos(SVector2D(this->GetWidth() / 2 + 10, mCharDescriptionPanel->GetPos().mY));
			weaponDescriptionImgPanel->SetBackColor(Gdiplus::Color::Gray);
			weaponDescriptionImg->SetTexture(uiImg->GetTexture());
			weaponNameTex->SetText(curWeapon.name);
			weaponTex->SetText(curWeapon.classType);

			descriptionWeaponTex->SetText(weaponDiscription);
			});

		weaponButton->SetEventOutHover([=]() {
			weaponButton->SetBackColor(Gdiplus::Color::Gray);
			});

		weaponButton->SetEventClick([=]() {
			CPlayer* pl = CPlayScene::GetPlayer();
			CPlayerScript* plSc = pl->GetComponent<CPlayerScript>();
			CWeaponMgr* plWeaponMgr = pl->GetComponent<CWeaponMgr>();

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

			for (auto& [ID, args] : curChar.effects) {
				ApplyEffect(ID, args);
			}

			CSceneMgr::LoadScene(L"PlayScene");
			});

		if (i == 11) {
			y += 85;
			x = 30;
			i = 0;
		}
		else {
			x += 85;
		}

	}

	CUIBase::OnCreate();
	this->OnUpdate(0.0f);
}

void CWeaponSelectUI::Active()
{
	CUIBase::Active();
}

void CWeaponSelectUI::InActive()
{
	CUIBase::InActive();
}

void CWeaponSelectUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CWeaponSelectUI::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CWeaponSelectUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CWeaponSelectUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CWeaponSelectUI::UIClear()
{
	CUIBase::UIClear();
}
