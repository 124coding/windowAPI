#include "CTitleSceneUI.h"

#include "CSceneMgr.h"

#include "CUIPanel.h"
#include "CUIButton.h"
#include "CUIText.h"

void CTitleSceneUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);

	CUIPanel* basePanel = new CUIPanel();

	basePanel->SetPos(SVector2D());
	basePanel->SetWidth(this->GetWidth());
	basePanel->SetHeight(this->GetHeight());

	CUIPanel* menuPanel = new CUIPanel();

	menuPanel->SetPos(SVector2D(30.0f, 3 * basePanel->GetHeight() / 5));
	menuPanel->SetWidth(basePanel->GetWidth() / 5);
	menuPanel->SetHeight(basePanel->GetHeight() / 3);

	basePanel->AddChild(menuPanel);

	CUIButton* startButton = new CUIButton();

	startButton->SetPos(SVector2D(50.0f, 0.0f));
	startButton->SetBackColor(Gdiplus::Color::Black);
	startButton->SetWidth(menuPanel->GetWidth() / 3);
	startButton->SetHeight(45.0f);
	startButton->SetCornerRadius(10.0f);

	CUIText* startButtonTex = new CUIText();
	
	startButtonTex->SetText(L"시작");
	startButtonTex->SetFont(L"Noto Sans KR Medium");
	startButtonTex->SetFontSize(28.0f);
	startButtonTex->SetStrokeWidth(1.0f);
	startButtonTex->SetColor(Gdiplus::Color::White);
	startButtonTex->SetWidth(startButton->GetWidth());
	startButtonTex->SetHeight(startButton->GetHeight());
	startButtonTex->SetIgnoreMouse(true);
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


	CUIButton* optionButton = new CUIButton();

	optionButton->SetPos(SVector2D(50.0f, startButton->GetHeight() + 10.0f));
	optionButton->SetBackColor(Gdiplus::Color::Black);
	optionButton->SetWidth(menuPanel->GetWidth() / 3);
	optionButton->SetHeight(45.0f);
	optionButton->SetCornerRadius(10.0f);

	CUIText* optionButtonTex = new CUIText();

	optionButtonTex->SetText(L"옵션");
	optionButtonTex->SetFont(L"Noto Sans KR Medium");
	optionButtonTex->SetFontSize(28.0f);
	optionButtonTex->SetStrokeWidth(1.0f);
	optionButtonTex->SetColor(Gdiplus::Color::White);
	optionButtonTex->SetWidth(optionButton->GetWidth());
	optionButtonTex->SetHeight(optionButton->GetHeight());
	optionButtonTex->SetIgnoreMouse(true);
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

	this->AddChild(basePanel);



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
