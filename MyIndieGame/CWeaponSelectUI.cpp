#include "CWeaponSelectUI.h"

#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CDataMgr.h"

#include "CUIButton.h"
#include "CUIPanel.h"
#include "CUIText.h"
#include "CUIImg.h"

#include "CPlayer.h"

#include "CPlayScene.h"
#include "CSettingScene.h"

#include "CPlayerScript.h"

#include <regex>

void CWeaponSelectUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);

	CUIPanel* basePanel = new CUIPanel();

	basePanel->SetPos(SVector2D());
	basePanel->SetWidth(this->GetWidth());
	basePanel->SetHeight(this->GetHeight());

	auto it = CDataMgr::GetCharacterDatas().find(CPlayScene::GetPlayer()->GetComponent<CPlayerScript>()->GetCharacter());
	if (it == CDataMgr::GetCharacterDatas().end()) {
		return;
	}

	CDataMgr::SCharacter curChar = it->second;

	// 뒤로가기 버튼
	CUIButton* backButton = new CUIButton();
	backButton->SetPos(SVector2D(windowWidth / 20, windowHeight / 20));
	backButton->SetWidth(200.0f);
	backButton->SetHeight(50.0f);
	backButton->SetBackColor(Gdiplus::Color::Black);
	backButton->SetCornerRadius(10.0f);

	CUIText* backButtonTex = new CUIText();

	backButtonTex->SetText(L"뒤로");
	backButtonTex->SetFont(L"Noto Sans KR Medium");
	backButtonTex->SetFontSize(24.0f);
	backButtonTex->SetStrokeWidth(1.0f);
	backButtonTex->SetColor(Gdiplus::Color::White);
	backButtonTex->SetPos(SVector2D());
	backButtonTex->SetWidth(backButton->GetWidth());
	backButtonTex->SetHeight(backButton->GetHeight());
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
		CUIMgr::Pop(eUIType::WeaponSelectUI);
		CUIMgr::Push(eUIType::CharacterSelectUI);
		});
	basePanel->AddChild(backButton);

	// 현재 창 텍스트
	CUIText* currentUITex = new CUIText();

	currentUITex->SetText(L"무기 선택");
	currentUITex->SetFont(L"Noto Sans KR Medium");
	currentUITex->SetFontSize(40.0f);
	currentUITex->SetStrokeWidth(1.0f);
	currentUITex->SetOutline(3.0f, Gdiplus::Color::Black);
	currentUITex->SetColor(Gdiplus::Color::White);
	currentUITex->SetWidth(this->GetWidth());
	currentUITex->SetHeight(this->GetHeight() / 5);
	currentUITex->SetPos(SVector2D(0.0f, currentUITex->GetHeight() / 3));
	currentUITex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	basePanel->AddChild(currentUITex);

	// 설명 패널
	CUIPanel* descriptionCharPanel = new CUIPanel();

	descriptionCharPanel->SetWidth(250.0f);
	descriptionCharPanel->SetHeight(320.0f);
	descriptionCharPanel->SetPos(SVector2D(basePanel->GetWidth() / 2 - descriptionCharPanel->GetWidth() / 2, this->GetHeight() / 7));
	descriptionCharPanel->SetBackColor(Gdiplus::Color::Black);
	descriptionCharPanel->SetCornerRadius(10.0f);

	basePanel->AddChild(descriptionCharPanel);

	CUIPanel* charDescriptionImgPanel = new CUIPanel();

	charDescriptionImgPanel->SetWidth(75.0f);
	charDescriptionImgPanel->SetHeight(75.0f);
	charDescriptionImgPanel->SetPos(SVector2D(10.0f, 10.0f));
	charDescriptionImgPanel->SetBackColor(Gdiplus::Color::Black);
	charDescriptionImgPanel->SetCornerRadius(10.0f);

	descriptionCharPanel->AddChild(charDescriptionImgPanel);

	CUIImg* charDescriptionImg = new CUIImg();

	charDescriptionImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	charDescriptionImg->SetWidth(charDescriptionImgPanel->GetWidth());
	charDescriptionImg->SetHeight(charDescriptionImgPanel->GetHeight());

	charDescriptionImgPanel->AddChild(charDescriptionImg);

	CUIText* charNameTex = new CUIText();

	charNameTex->SetPos(SVector2D(charDescriptionImgPanel->GetPos().mX + charDescriptionImgPanel->GetWidth() + 10.0f, charDescriptionImgPanel->GetPos().mY));
	charNameTex->SetWidth(100.0f);
	charNameTex->SetHeight(25.0f);

	charNameTex->SetText(L"");
	charNameTex->SetFont(L"Noto Sans KR Medium");
	charNameTex->SetFontSize(20.0f);
	charNameTex->SetColor(Gdiplus::Color::White);

	descriptionCharPanel->AddChild(charNameTex);

	CUIText* charTex = new CUIText();

	charTex->SetPos(SVector2D(charDescriptionImgPanel->GetPos().mX + charDescriptionImgPanel->GetWidth() + 10.0f, charDescriptionImgPanel->GetPos().mY + 30.0f));
	charTex->SetWidth(100.0f);
	charTex->SetHeight(25.0f);

	charTex->SetText(L"");
	charTex->SetFont(L"Noto Sans KR Medium");
	charTex->SetFontSize(15.0f);
	charTex->SetColor(Gdiplus::Color::LightYellow);

	descriptionCharPanel->AddChild(charTex);

	CUIText* descriptionCharTex = new CUIText();

	descriptionCharTex->SetPos(SVector2D(charDescriptionImgPanel->GetPos().mX, charDescriptionImgPanel->GetPos().mY + charDescriptionImgPanel->GetHeight() + 10.0f));
	descriptionCharTex->SetWidth(200.0f);
	descriptionCharTex->SetHeight(250.0f);

	descriptionCharTex->SetFont(L"Noto Sans KR Medium");
	descriptionCharTex->SetFontSize(15.0f);
	descriptionCharTex->SetColor(Gdiplus::Color::White);

	descriptionCharPanel->AddChild(descriptionCharTex);

	charDescriptionImgPanel->SetBackColor(Gdiplus::Color::Gray);
	charDescriptionImg->SetTexture(CResourceMgr::Find<CTexture>(curChar.iconTexture));
	charNameTex->SetText(curChar.name);
	charTex->SetText(L"캐릭터");

	std::wstring charDiscription = L"";

	for (auto& [effectID, args] : curChar.effects) {
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

		charDiscription += rawDesc + L"\n";
	}

	descriptionCharTex->SetText(charDiscription);


	// 무기 설명 패널
	CUIPanel* descriptionWeaponPanel = new CUIPanel();

	descriptionWeaponPanel->SetWidth(250.0f);
	descriptionWeaponPanel->SetHeight(320.0f);
	descriptionWeaponPanel->SetPos(SVector2D());
	descriptionWeaponPanel->SetBackColor(Gdiplus::Color::Black);
	descriptionWeaponPanel->SetCornerRadius(10.0f);
	descriptionWeaponPanel->InActive();

	basePanel->AddChild(descriptionWeaponPanel);

	CUIPanel* weaponDescriptionImgPanel = new CUIPanel();

	weaponDescriptionImgPanel->SetWidth(75.0f);
	weaponDescriptionImgPanel->SetHeight(75.0f);
	weaponDescriptionImgPanel->SetPos(SVector2D(10.0f, 10.0f));
	weaponDescriptionImgPanel->SetBackColor(Gdiplus::Color::Black);
	weaponDescriptionImgPanel->SetCornerRadius(10.0f);

	descriptionWeaponPanel->AddChild(weaponDescriptionImgPanel);

	CUIImg* weaponDescriptionImg = new CUIImg();

	weaponDescriptionImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	weaponDescriptionImg->SetWidth(weaponDescriptionImgPanel->GetWidth());
	weaponDescriptionImg->SetHeight(weaponDescriptionImgPanel->GetHeight());

	weaponDescriptionImgPanel->AddChild(weaponDescriptionImg);

	CUIText* weaponNameTex = new CUIText();

	weaponNameTex->SetPos(SVector2D(weaponDescriptionImgPanel->GetPos().mX + weaponDescriptionImgPanel->GetWidth() + 10.0f, weaponDescriptionImgPanel->GetPos().mY));
	weaponNameTex->SetWidth(100.0f);
	weaponNameTex->SetHeight(25.0f);

	weaponNameTex->SetText(L"");
	weaponNameTex->SetFont(L"Noto Sans KR Medium");
	weaponNameTex->SetFontSize(20.0f);
	weaponNameTex->SetColor(Gdiplus::Color::White);

	descriptionWeaponPanel->AddChild(weaponNameTex);

	CUIText* weaponTex = new CUIText();

	weaponTex->SetPos(SVector2D(weaponDescriptionImgPanel->GetPos().mX + weaponDescriptionImgPanel->GetWidth() + 10.0f, weaponDescriptionImgPanel->GetPos().mY + 30.0f));
	weaponTex->SetWidth(100.0f);
	weaponTex->SetHeight(25.0f);

	weaponTex->SetText(L"");
	weaponTex->SetFont(L"Noto Sans KR Medium");
	weaponTex->SetFontSize(15.0f);
	weaponTex->SetColor(Gdiplus::Color::LightYellow);

	descriptionWeaponPanel->AddChild(weaponTex);

	CUIText* descriptionWeaponTex = new CUIText();

	descriptionWeaponTex->SetPos(SVector2D(weaponDescriptionImgPanel->GetPos().mX, weaponDescriptionImgPanel->GetPos().mY + weaponDescriptionImgPanel->GetHeight() + 10.0f));
	descriptionWeaponTex->SetWidth(200.0f);
	descriptionWeaponTex->SetHeight(250.0f);

	descriptionWeaponTex->SetFont(L"Noto Sans KR Medium");
	descriptionWeaponTex->SetFontSize(15.0f);
	descriptionWeaponTex->SetColor(Gdiplus::Color::White);

	descriptionWeaponPanel->AddChild(descriptionWeaponTex);

	CUIPanel* weaponSelectPanel = new CUIPanel;

	weaponSelectPanel->SetPos(SVector2D(basePanel->GetPos().mX, basePanel->GetPos().mY + basePanel->GetHeight() / 2));
	weaponSelectPanel->SetWidth(basePanel->GetWidth());
	weaponSelectPanel->SetHeight(basePanel->GetHeight() / 2);


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

		CUIButton* weaponButton = new CUIButton();
		weaponButton->SetPos(SVector2D(x, y));
		weaponButton->SetWidth(75.0f);
		weaponButton->SetHeight(75.0f);
		weaponButton->SetBackColor(Gdiplus::Color::Gray);
		weaponButton->SetCornerRadius(10.0f);

		CUIImg* uiImg = new CUIImg();
		uiImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
		uiImg->SetTexture(CResourceMgr::Find<CTexture>(curWeapon.iconTexture));
		uiImg->SetWidth(weaponButton->GetWidth());
		uiImg->SetHeight(weaponButton->GetHeight());

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
			weaponDiscription += L"<c=#FFFFE0>치명타:</c> X" + GetCleanVal(data.critDamagePer);

			if (data.critChancePer > 0.0f) {
				weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
			}
			weaponDiscription += L"\n";
		}

		descriptionStat(L"쿨다운", data.delay, L"s");
		descriptionStat(L"범위", data.range);


		// 특별한 능력에서 따로 설명 가져오는거 필요

		weaponButton->SetEventHover([=]() {
			weaponButton->SetBackColor(Gdiplus::Color::White);
			descriptionWeaponPanel->Active();
			descriptionCharPanel->SetPos(SVector2D(basePanel->GetWidth() / 2 - descriptionCharPanel->GetWidth() - 10, descriptionCharPanel->GetPos().mY));
			descriptionWeaponPanel->SetPos(SVector2D(basePanel->GetWidth() / 2 + 10, descriptionCharPanel->GetPos().mY));
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
			CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

			plSc->SetClothTexture(CResourceMgr::Find<CTexture>(curChar.clothTexture));
			plSc->SetMouthTexture(CResourceMgr::Find<CTexture>(curChar.mouthTexture));
			plSc->SetEyesTexture(CResourceMgr::Find<CTexture>(curChar.eyesTexture));
			plSc->SetHairTexture(CResourceMgr::Find<CTexture>(curChar.hairTexture));
			plSc->SetStartingWeaponID(curWeapon.ID);

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

	basePanel->AddChild(weaponSelectPanel);
	this->AddChild(basePanel);

	CUIBase::OnCreate();
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
