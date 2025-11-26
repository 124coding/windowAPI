#include "CCharacterSelectUI.h"

#include "CUIMgr.h"
#include "CSceneMgr.h"

#include "CUIButton.h"

void CCharacterSelectUI::OnCreate()
{


	CUIButton* backButton = CUIButton::CreateInvertButton(
		L"BackButton"
		, L"뒤로"
		, L""
		, windowWidth / 20, windowHeight / 20
		, 100.0f, 10.0f
		, Gdiplus::Color(255, 0, 0, 0), Gdiplus::Color(255, 255, 255, 255)
		, Gdiplus::Color(255, 255, 255, 255), Gdiplus::Color(255, 0, 0, 0)
		, 24.0f
		, 2.0f);

	//CTexture* backMouseInTex = CTexture::Create(L"BackMouseInButton", 100, 50);
	//CTexture* backMouseOutTex = CTexture::Create(L"BackMouseOutButton", 100, 50);

	//if (backMouseInTex != nullptr && backMouseOutTex != nullptr) {
	//	// Gdiplus를 이용한 텍스트 작성 및 텍스처 내부 색 채우기

	//	Gdiplus::Graphics mouseInGraphics(backMouseInTex->GetImage());
	//	mouseInGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	//	mouseInGraphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	//	mouseInGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	//	Gdiplus::Graphics mouseOutGraphics(backMouseOutTex->GetImage());
	//	mouseOutGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	//	mouseOutGraphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	//	mouseOutGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);


	//	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	//	Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));

	//	Gdiplus::Pen boldWhitePen(Gdiplus::Color(255, 255, 255, 255), 2);
	//	Gdiplus::Pen boldBlackPen(Gdiplus::Color(255, 0, 0, 0), 2);


	//	Gdiplus::GraphicsPath bgPath;
	//	Gdiplus::Rect rect(0, 0, backMouseInTex->GetWidth(), backMouseInTex->GetHeight());
	//	CTexture::AddRoundedRectToPath(&bgPath, rect, backMouseInTex->GetHeight() / 5);

	//	mouseInGraphics.FillPath(&whiteBrush, &bgPath);
	//	mouseOutGraphics.FillPath(&blackBrush, &bgPath);

	//	Gdiplus::GraphicsPath textPath;
	//	Gdiplus::FontFamily fontFamily(L"Noto Sans KR Medium");
	//	Gdiplus::StringFormat format;
	//	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	//	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	//	Gdiplus::RectF textRect(0.0f, 0.0f, (float)backMouseInTex->GetWidth(), (float)backMouseInTex->GetHeight());

	//	textPath.AddString(
	//		L"뒤로", -1,
	//		&fontFamily, Gdiplus::FontStyleBold,
	//		24, textRect, &format
	//	);

	//	mouseInGraphics.FillPath(&blackBrush, &textPath); 
	//	mouseInGraphics.DrawPath(&boldBlackPen, &textPath);

	//	mouseOutGraphics.FillPath(&whiteBrush, &textPath);
	//	mouseOutGraphics.DrawPath(&boldWhitePen, &textPath);

	//	// 텍스처 키 지정
	//	backButton->SetMouseInTexture(L"BackMouseInButton");
	//	backButton->SetMouseOutTexture(L"BackMouseOutButton");
	//}

	//backButton->SetWidth(backMouseInTex->GetWidth());
	//backButton->SetHeight(backMouseInTex->GetHeight());
	//backButton->SetPos(SVector2D(windowWidth / 20, windowHeight / 20));
	backButton->SetButtonClick([=]() { CSceneMgr::LoadScene(L"TitleScene"); });
	backButton->Active();
	this->AddChild(backButton);

	// CUIMgr::Push(eUIType::CharacterSelectUI);
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
