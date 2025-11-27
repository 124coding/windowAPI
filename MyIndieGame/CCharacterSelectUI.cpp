#include "CCharacterSelectUI.h"

#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CDataMgr.h"

#include "CUIButton.h"
#include "CUIPanel.h"
#include "CUIText.h"
#include "CUIImg.h"

void CCharacterSelectUI::OnCreate()
{

	CUIPanel* basePanel = new CUIPanel();

	basePanel->SetPos(SVector2D());
	basePanel->SetWidth(this->GetWidth());
	basePanel->SetHeight(this->GetHeight());

	/*CUIText* text = new CUIText();

	text->SetPos(SVector2D(30.0f, 30.0f));
	text->SetText(L"Test");

	basePanel->AddChild(text);*/

	CUIButton* backButton = new CUIButton();
	backButton->SetPos(SVector2D(windowWidth / 20, windowHeight / 20));
	backButton->SetWidth(200.0f);
	backButton->SetHeight(50.0f);
	backButton->SetBackColor(Gdiplus::Color(255, 0, 0, 0));
	backButton->SetCornerRadius(10.0f);

	CUIText* backButtonTex = new CUIText();

	backButtonTex->SetText(L"뒤로");
	backButtonTex->SetFont(L"Noto Sans KR Medium");
	backButtonTex->SetFontSize(24.0f);
	backButtonTex->SetStrokeWidth(1.0f);
	// backButtonTex->SetOutline(1.0f, Gdiplus::Color::Red);
	backButtonTex->SetColor(Gdiplus::Color(255, 255, 255, 255));
	backButtonTex->SetPos(SVector2D());
	backButtonTex->SetWidth(backButton->GetWidth());
	backButtonTex->SetHeight(backButton->GetHeight());
	backButtonTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	backButton->AddChild(backButtonTex);

	backButton->SetEventHover([=]() {
		backButtonTex->SetColor(Gdiplus::Color(255, 0, 0, 0));
		backButton->SetBackColor(Gdiplus::Color(255, 255, 255, 255));
		});

	backButton->SetEventOutHover([=]() {
		backButtonTex->SetColor(Gdiplus::Color(255, 255, 255, 255));
		backButton->SetBackColor(Gdiplus::Color(255, 0, 0, 0));
		});

	backButton->SetEventClick([=]() { CSceneMgr::LoadScene(L"TitleScene"); });
	basePanel->AddChild(backButton);

	CUIText* currentUITex = new CUIText();

	currentUITex->SetText(L"캐릭터 선택");
	currentUITex->SetFont(L"Noto Sans KR Medium");
	currentUITex->SetFontSize(70.0f);
	currentUITex->SetStrokeWidth(1.0f);
	currentUITex->SetOutline(3.0f, Gdiplus::Color::Black);
	currentUITex->SetColor(Gdiplus::Color(255, 255, 255, 255));
	currentUITex->SetPos(SVector2D(0.0f, this->GetHeight() / 15));
	currentUITex->SetWidth(this->GetWidth());
	currentUITex->SetHeight(this->GetHeight());
	currentUITex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	basePanel->AddChild(currentUITex);

	int x = 30;
	int y = windowHeight / 2 + 30;
	int i = 1;

	// 캐릭터 데이터 가져와서 버튼 만들기
	for (auto& character : CDataMgr::GetCharacterDatas()["Characters"]) {
		std::string name = character["Name"];
		std::string iconImage = character["IconTexture"];

		CUIButton* charButton = new CUIButton();
		charButton->SetPos(SVector2D(x, y));
		charButton->SetWidth(75.0f);
		charButton->SetHeight(75.0f);
		charButton->SetBackColor(Gdiplus::Color(255, 10, 10, 10));
		charButton->SetCornerRadius(10.0f);

		CUIImg* uiImg = new CUIImg();
		uiImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
		uiImg->SetTexture(std::wstring(iconImage.begin(), iconImage.end()));
		uiImg->SetWidth(charButton->GetWidth());
		uiImg->SetHeight(charButton->GetHeight());

		charButton->AddChild(uiImg);
			/*::CreateInvertButton(
			std::wstring(name.begin(), name.end())
			, L""
			, std::wstring(iconImage.begin(), iconImage.end())
			, x, y
			, 0.0f, 0.0f
			, Gdiplus::Color(255, 20, 20, 20), Gdiplus::Color(255, 255, 255, 255)
			, Gdiplus::Color(255, 255, 255, 255), Gdiplus::Color(255, 255, 255, 255)
			, 0.8f
			, 0.8f
		);*/

		charButton->SetEventHover([=]() {
			charButton->SetBackColor(Gdiplus::Color(255, 255, 255, 255));
			});
		charButton->SetEventOutHover([=]() {
			charButton->SetBackColor(Gdiplus::Color(255, 10, 10, 10));
			});
		// charButton->SetEventClick();
		basePanel->AddChild(charButton);
		
		if (i == 11) {
			y += 85;
			i = 0;
		}
		else {
			x += 85;
		}
	}

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
