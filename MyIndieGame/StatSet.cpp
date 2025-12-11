#include "StatSet.h";

CUIPanel* MakeStat(float tWidth, float tHeight, std::wstring tTexName, std::wstring tName, CUIText*& tSaveUITex) {
	CUIPanel* separateStatPanel = new CUIPanel();
	separateStatPanel->SetWidth(tWidth);
	separateStatPanel->SetHeight(tHeight);
	separateStatPanel->SetCornerRadius(10);

	CUIImg* statImg = new CUIImg();
	statImg->SetTexture(CResourceMgr::Find<CTexture>(tTexName));
	statImg->SetWidth(tWidth / 6);
	statImg->SetHeight(tHeight);
	statImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	statImg->SetPos(SVector2D(5.0f, 0.0f));

	separateStatPanel->AddChild(statImg);

	CUIText* statName = new CUIText();
	statName->SetFontSize(15.0f);
	statName->SetText(tName);
	statName->SetWidth((tWidth - tWidth / 6) / 2);
	statName->SetHeight(tHeight);
	statName->SetPos(SVector2D(statImg->GetWidth() + 5.0f, 0.0f));
	statName->SetColor(Gdiplus::Color::White);
	statName->SetAlign(Gdiplus::StringAlignmentNear, Gdiplus::StringAlignmentCenter);

	separateStatPanel->AddChild(statName);

	tSaveUITex = new CUIText();
	tSaveUITex->SetFontSize(15.0f);
	tSaveUITex->SetPos(SVector2D(statName->GetPos().mX + statName->GetHeight(), 0.0f));
	tSaveUITex->SetWidth(tWidth - statName->GetWidth());
	tSaveUITex->SetHeight(tHeight);
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
