#include "StatSet.h";

CUIPanel* MakeStat(float tWidth, float tHeight, std::wstring tTexName, std::wstring tName, CUIText*& tSaveUITex) {
	CUIPanel* separateStatPanel = new CUIPanel(SVector2D(), tWidth, tHeight);
	separateStatPanel->SetCornerRadius(10);

	CUIImg* statImg = new CUIImg(SVector2D(5.0f, 0.0f), tWidth / 6, tHeight, CResourceMgr::Find<CTexture>(tTexName));
	statImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	separateStatPanel->AddChild(statImg);

	CUIText* statName = new CUIText(SVector2D(statImg->GetWidth() + 5.0f, 0.0f), (tWidth - tWidth / 6) / 2, tHeight, tName);
	statName->SetFontSize(15.0f);
	statName->SetColor(Gdiplus::Color::White);
	statName->SetAlign(Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentCenter);

	separateStatPanel->AddChild(statName);

	tSaveUITex = new CUIText(SVector2D(statName->GetPos().mX + statName->GetHeight(), 0.0f), tWidth - statName->GetWidth(), tHeight);
	tSaveUITex->SetFontSize(15.0f);
	tSaveUITex->SetAlign(Gdiplus::StringAlignmentFar, Gdiplus::StringAlignmentCenter);

	separateStatPanel->AddChild(tSaveUITex);

	return separateStatPanel;
}

void SettingStatTex(int tValue, CUIText* tTexUI) {
	if (tValue < 0) {
		tTexUI->SetColor(Gdiplus::Color::Red);
	}
	else if (tValue > 0) {
		tTexUI->SetColor(Gdiplus::Color::Green);
	}
	else {
		tTexUI->SetColor(Gdiplus::Color::White);
	}

	tTexUI->SetText(std::to_wstring(tValue));
}
