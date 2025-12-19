#include "CTitleSceneUI.h"

#include "CSceneMgr.h"

#include "CUIPanel.h"
#include "CUIButton.h"
#include "CUIText.h"

void CTitleSceneUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);


	CUIPanel* menuPanel = new CUIPanel(SVector2D(30.0f, 3 * this->GetHeight() / 5), this->GetWidth() / 5, this->GetHeight() / 3);

	this->AddChild(menuPanel);

	CUIButton* startButton = new CUIButton(SVector2D(50.0f, 0.0f), menuPanel->GetWidth() / 3, 45.0f);

	startButton->SetBackColor(Gdiplus::Color::Black);
	startButton->SetCornerRadius(10.0f);

	CUIText* startButtonTex = new CUIText(SVector2D(), startButton->GetWidth(), startButton->GetHeight(), L"시작");
	
	startButtonTex->SetFontSize(28.0f);
	startButtonTex->SetStrokeWidth(1.0f);
	startButtonTex->SetColor(Gdiplus::Color::White);
	startButtonTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	startButton->SetEventHover([=]() {
		startButton->SetBackColor(Gdiplus::Color::White);
		startButtonTex->SetColor(Gdiplus::Color::Black);
		});
	startButton->SetEventOutHover([=]() {
		startButton->SetBackColor(Gdiplus::Color::Black);
		startButtonTex->SetColor(Gdiplus::Color::White);
		});
	startButton->SetEventClick([=]() {
		CSceneMgr::LoadScene(L"SettingScene");
		});

	startButton->AddChild(startButtonTex);

	menuPanel->AddChild(startButton);


	CUIButton* optionButton = new CUIButton(SVector2D(50.0f, startButton->GetHeight() + 10.0f), menuPanel->GetWidth() / 3, 45.0f);

	optionButton->SetBackColor(Gdiplus::Color::Black);
	optionButton->SetCornerRadius(10.0f);

	CUIText* optionButtonTex = new CUIText(SVector2D(), optionButton->GetWidth(), optionButton->GetHeight(), L"옵션");

	optionButtonTex->SetFont(L"Noto Sans KR Medium");
	optionButtonTex->SetFontSize(28.0f);
	optionButtonTex->SetStrokeWidth(1.0f);
	optionButtonTex->SetColor(Gdiplus::Color::White);
	optionButtonTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentNear);

	optionButton->SetEventHover([=]() {
		optionButton->SetBackColor(Gdiplus::Color::White);
		optionButtonTex->SetColor(Gdiplus::Color::Black);
		});
	optionButton->SetEventOutHover([=]() {
		optionButton->SetBackColor(Gdiplus::Color::Black);
		optionButtonTex->SetColor(Gdiplus::Color::White);
		});
	optionButton->SetEventClick([=]() {
		});

	optionButton->AddChild(optionButtonTex);

	menuPanel->AddChild(optionButton);



	CUIBase::OnCreate();

	this->OnUpdate(0.0f);
}

void CTitleSceneUI::Active()
{
	CUIBase::Active();
}

void CTitleSceneUI::InActive()
{
	CUIBase::InActive();
}

void CTitleSceneUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CTitleSceneUI::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CTitleSceneUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CTitleSceneUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CTitleSceneUI::UIClear()
{
	CUIBase::UIClear();
}
