#include "CCharacterSelectUI.h"

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

void CCharacterSelectUI::OnCreate()
{

	SetWidth(windowWidth);
	SetHeight(windowHeight);

	CUIPanel* basePanel = new CUIPanel();

	basePanel->SetPos(SVector2D());
	basePanel->SetWidth(this->GetWidth());
	basePanel->SetHeight(this->GetHeight());

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
		CSceneMgr::LoadScene(L"TitleScene");
		});
	basePanel->AddChild(backButton);

	// 현재 창 텍스트
	CUIText* currentUITex = new CUIText();

	currentUITex->SetText(L"캐릭터 선택");
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
	CUIPanel* descriptionPanel = new CUIPanel();

	descriptionPanel->SetWidth(250.0f);
	descriptionPanel->SetHeight(320.0f);
	descriptionPanel->SetPos(SVector2D(basePanel->GetWidth() / 2 - descriptionPanel->GetWidth() / 2, this->GetHeight() / 7));
	descriptionPanel->SetBackColor(Gdiplus::Color::Black);
	descriptionPanel->SetCornerRadius(10.0f);

	basePanel->AddChild(descriptionPanel);

	CUIPanel* charDescriptionImgPanel = new CUIPanel();

	charDescriptionImgPanel->SetWidth(75.0f);
	charDescriptionImgPanel->SetHeight(75.0f);
	charDescriptionImgPanel->SetPos(SVector2D(10.0f, 10.0f));
	charDescriptionImgPanel->SetBackColor(Gdiplus::Color::Black);
	charDescriptionImgPanel->SetCornerRadius(10.0f);

	descriptionPanel->AddChild(charDescriptionImgPanel);

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

	descriptionPanel->AddChild(charNameTex);

	CUIText* charTex = new CUIText();

	charTex->SetPos(SVector2D(charDescriptionImgPanel->GetPos().mX + charDescriptionImgPanel->GetWidth() + 10.0f, charDescriptionImgPanel->GetPos().mY + 30.0f));
	charTex->SetWidth(100.0f);
	charTex->SetHeight(25.0f);

	charTex->SetText(L"");
	charTex->SetFont(L"Noto Sans KR Medium");
	charTex->SetFontSize(15.0f);
	charTex->SetColor(Gdiplus::Color::LightYellow);

	descriptionPanel->AddChild(charTex);

	CUIText* descriptionTex = new CUIText();

	descriptionTex->SetPos(SVector2D(charDescriptionImgPanel->GetPos().mX, charDescriptionImgPanel->GetPos().mY + charDescriptionImgPanel->GetHeight() + 10.0f));
	descriptionTex->SetWidth(200.0f);
	descriptionTex->SetHeight(250.0f);

	descriptionTex->SetFont(L"Noto Sans KR Medium");
	descriptionTex->SetFontSize(15.0f);
	descriptionTex->SetColor(Gdiplus::Color::White);

	descriptionPanel->AddChild(descriptionTex);

	CUIPanel* charSelectPanel = new CUIPanel;

	charSelectPanel->SetPos(SVector2D(basePanel->GetPos().mX, basePanel->GetPos().mY + basePanel->GetHeight() / 2));
	charSelectPanel->SetWidth(basePanel->GetWidth());
	charSelectPanel->SetHeight(basePanel->GetHeight() / 2);

	int x = 30;
	int y = 100;
	int i = 1;

	// 캐릭터 데이터 가져와서 버튼 만들기
	for (auto& [id, character] : CDataMgr::GetCharacterDatas()) {
		CUIButton* charButton = new CUIButton();
		charButton->SetPos(SVector2D(x, y));
		charButton->SetWidth(75.0f);
		charButton->SetHeight(75.0f);
		charButton->SetBackColor(Gdiplus::Color::Gray);
		charButton->SetCornerRadius(10.0f);

		CUIImg* uiImg = new CUIImg();
		uiImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
		uiImg->SetTexture(CResourceMgr::Find<CTexture>(character.iconTexture));
		uiImg->SetWidth(charButton->GetWidth());
		uiImg->SetHeight(charButton->GetHeight());

		charButton->AddChild(uiImg);

		charSelectPanel->AddChild(charButton);

		std::wstring finalDiscription = L"";

		for (auto& [effectID, args] : character.effects) {
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

		charButton->SetEventHover([=]() {
			charButton->SetBackColor(Gdiplus::Color::White);
			charDescriptionImgPanel->SetBackColor(Gdiplus::Color::Gray);
			charDescriptionImg->SetTexture(uiImg->GetTexture());
			charNameTex->SetText(character.name);
			charTex->SetText(L"캐릭터");

			descriptionTex->SetText(finalDiscription);
			});

		charButton->SetEventOutHover([=]() {
			charButton->SetBackColor(Gdiplus::Color::Gray);
			});

		charButton->SetEventClick([=]() {
			CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
			plSc->SetCharacter(character.ID);
			CUIMgr::Pop(eUIType::CharacterSelectUI);
			CUIMgr::Push(eUIType::WeaponSelectUI);
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

	basePanel->AddChild(charSelectPanel);
	this->AddChild(basePanel);

	CUIBase::OnCreate();
}

void CCharacterSelectUI::Active()
{
	CUIBase::Active();
}

void CCharacterSelectUI::InActive()
{
	CUIBase::InActive();
}

void CCharacterSelectUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CCharacterSelectUI::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CCharacterSelectUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CCharacterSelectUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CCharacterSelectUI::UIClear()
{
	CUIBase::UIClear();
}
