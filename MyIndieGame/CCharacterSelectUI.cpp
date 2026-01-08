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
	// Click 이벤트: 타이틀 화면으로 씬 전환
	backButton->SetEventClick([=]() { 
		CPlayScene::GetPlayer()->GetComponent<CTransform>(eComponentType::Transform)->SetPos(SVector2D(mapWidth / 2, mapHeight / 2 + 55.0f));

		CSceneMgr::LoadScene(L"TitleScene");
		});
	this->AddChild(backButton);

	// 현재 창 텍스트
	CUIText* currentUITex = new CUIText(SVector2D(), this->GetWidth(), this->GetHeight() / 5, L"캐릭터 선택");

	currentUITex->SetFont(L"Noto Sans KR Medium");
	currentUITex->SetFontSize(40.0f);
	currentUITex->SetStrokeWidth(1.0f);
	currentUITex->SetOutline(3.0f, Gdiplus::Color::Black);
	currentUITex->SetColor(Gdiplus::Color::White);
	currentUITex->SetPos(SVector2D(0.0f, currentUITex->GetHeight() / 3));
	currentUITex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	this->AddChild(currentUITex);



	// ==========================================
	// Info Preview Panel (캐릭터 정보 표시 패널)
	// ==========================================
	// 캐릭터 버튼에 마우스를 올렸을 때(Hover), 해당 캐릭터의 상세 정보를 보여주는 패널
	// 중앙에 하나만 생성해두고, 내용은 이벤트에 따라 동적으로 교체함
	mCharDescriptionPanel = new CUIPanel(SVector2D(), 250.0f, 320.0f);

	mCharDescriptionPanel->SetWidth(250.0f);
	mCharDescriptionPanel->SetHeight(320.0f);
	mCharDescriptionPanel->SetPos(SVector2D(this->GetWidth() / 2 - mCharDescriptionPanel->GetWidth() / 2, this->GetHeight() / 7));
	mCharDescriptionPanel->SetBackColor(Gdiplus::Color::Black);
	mCharDescriptionPanel->SetCornerRadius(10.0f);

	this->AddChild(mCharDescriptionPanel);

	// 이미지, 이름, 직업, 설명 텍스트 UI 생성 및 배치
	CUIPanel* charDescriptionImgPanel = new CUIPanel(SVector2D(10.0f, 10.0f), 75.0f, 75.0f);
	charDescriptionImgPanel->SetBackColor(Gdiplus::Color::Black);
	charDescriptionImgPanel->SetCornerRadius(10.0f);

	mCharDescriptionPanel->AddChild(charDescriptionImgPanel);

	CUIImg* charDescriptionImg = new CUIImg(SVector2D(), charDescriptionImgPanel->GetWidth(), charDescriptionImgPanel->GetHeight());
	
	charDescriptionImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	charDescriptionImgPanel->AddChild(charDescriptionImg);

	CUIText* charNameTex = new CUIText(SVector2D(charDescriptionImgPanel->GetPos().mX + charDescriptionImgPanel->GetWidth() + 10.0f, charDescriptionImgPanel->GetPos().mY), 100.0f, 25.0f);

	charNameTex->SetFontSize(20.0f);
	charNameTex->SetColor(Gdiplus::Color::White);

	mCharDescriptionPanel->AddChild(charNameTex);

	CUIText* charTex = new CUIText(SVector2D(charDescriptionImgPanel->GetPos().mX + charDescriptionImgPanel->GetWidth() + 10.0f, charDescriptionImgPanel->GetPos().mY + 30.0f), 100.0f, 25.0f);

	charTex->SetFontSize(15.0f);
	charTex->SetColor(Gdiplus::Color::LightYellow);

	mCharDescriptionPanel->AddChild(charTex);

	CUIText* descriptionTex = new CUIText(SVector2D(charDescriptionImgPanel->GetPos().mX, charDescriptionImgPanel->GetPos().mY + charDescriptionImgPanel->GetHeight() + 10.0f), 200.0f, 250.0f);

	descriptionTex->SetFontSize(15.0f);
	descriptionTex->SetColor(Gdiplus::Color::White);

	mCharDescriptionPanel->AddChild(descriptionTex);

	// ==========================================
	// Dynamic Grid Generation (캐릭터 목록 생성)
	// ==========================================
	CUIPanel* charSelectPanel = new CUIPanel(SVector2D(this->GetPos().mX, this->GetPos().mY + this->GetHeight() / 2), this->GetWidth(), this->GetHeight() / 2);

	int x = 30;
	int y = 100;
	int i = 1;	// 줄바꿈 계산용 인덱스

	// 데이터 매니저의 캐릭터 목록을 순회하며 버튼 동적 생성
	for (auto& [id, character] : CDataMgr::GetCharacterDatas()) {
		// 1. 캐릭터 버튼 생성
		CUIButton* charButton = new CUIButton(SVector2D(x, y), 75.0f, 75.0f);
		charButton->SetBackColor(Gdiplus::Color::Gray);
		charButton->SetCornerRadius(10.0f);

		CUIImg* uiImg = new CUIImg(SVector2D(), charButton->GetWidth(), charButton->GetHeight(), CResourceMgr::Find<CTexture>(character.iconTexture));
		uiImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		charButton->AddChild(uiImg);

		charSelectPanel->AddChild(charButton);

		// 2. [Text Parsing] 상세 설명 텍스트 가공
		// 데이터 원본: "최대 체력 +{0}, 이동 속도 -{1}%"
		// 목표 형태: "<c=#00FF00>최대 체력 +10</c>, <c=#FF0000>이동 속도 -5%</c>"
		std::wstring finalDiscription = L"";

		for (auto& [effectID, args] : character.effects) {
			auto it = CDataMgr::GetEffectDatas().find(effectID);
			if (it == CDataMgr::GetEffectDatas().end()) {
				continue;
			}

			std::wstring rawDesc = it->second.description;	// 설명 템플릿 가져오기

			int index = 0;

			// 인자(Args) 순회하며 치환
			for (auto& arg : args) {
				std::wstring value = arg.value;

				std::wstring colorStr = L"#FFFFFF"; // 기본값
				if (arg.color != L"") {
					colorStr = arg.color;
				}

				// 양수 값 앞에 '+' 기호 붙이기 (Green 색상일 경우)
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

				// 색상 태그 입히기
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

		// 자동 줄바꿈 처리 (Word Wrapping)
		finalDiscription = CUIText::InsertLineBreaks(finalDiscription, mCharDescriptionPanel->GetWidth() - charDescriptionImgPanel->GetPos().mX * 2, L"Noto Sans KR Medium", 15.0f, false);

		// 3. 이벤트 바인딩
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
			CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

			plSc->SetStartingCharacterID(character.ID);
			this->InActive();
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

	this->AddChild(charSelectPanel);

	CUIBase::OnCreate();
	this->OnUpdate(0.0f);
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
