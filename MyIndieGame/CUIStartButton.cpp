#include "CUIStartButton.h"

#include "CSceneMgr.h"

void CUIStartButton::OnCreate()
{
	CUIButton::OnCreate();

	this->SetName(L"StartButton");
	this->SetPos(SVector2D(windowWidth / 5, 2 * windowHeight / 3));
	this->SetSize(SVector2D(200.0f, 200.0f));
	this->SetMouseInTexture(L"StartInMouse");
	this->SetMouseOutTexture(L"StartOutMouse");
}

void CUIStartButton::Active()
{
}

void CUIStartButton::InActive()
{
}

void CUIStartButton::OnDestroy()
{
	CUIButton::OnDestroy();
}

void CUIStartButton::OnUpdate(float tDeltaTime)
{
	CUIButton::OnUpdate(tDeltaTime);
}

void CUIStartButton::OnLateUpdate(float tDeltaTime)
{
	CUIButton::OnLateUpdate(tDeltaTime);
}

void CUIStartButton::Render(HDC tHDC)
{
	CUIButton::Render(tHDC);
}

void CUIStartButton::UIClear()
{
	CUIButton::UIClear();
}

void CUIStartButton::ButtonClick()
{
	CSceneMgr::LoadScene(L"SettingScene");
}
