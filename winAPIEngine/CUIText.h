#pragma once

#include "CUIBase.h"

class CUIText : public CUIBase
{
public:
	CUIText() : CUIBase(eUIType::None)
		, mText(L"")
		, mFont(L"Noto Sans KR Medium")
		, mFontSize(20.0f)
		, mFontColor(Gdiplus::Color::Black)
		, mStrokeWidth(0.0f)
		, mbBold(false)
		, mOutlineWidth(0.0f)
		, mOutlineColor(Gdiplus::Color::Black)
		, mAlignH(Gdiplus::StringAlignmentNear)
		, mAlignV(Gdiplus::StringAlignmentNear) {
		mIgnoreMouse = true;
	}
	virtual ~CUIText() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

public:
	void SetText(const std::wstring& tText) {
		this->mText = tText;
	}

	void SetFont(const std::wstring& tFont) {
		this->mFont = tFont;
	}

	void SetFontSize(float tFontSize) {
		this->mFontSize = tFontSize;
	}

	void SetColor(Gdiplus::Color tColor) {
		this->mFontColor = tColor;
	}

	void SetStrokeWidth(float tStrokeWidth) {
		this->mStrokeWidth = tStrokeWidth;
	}

	void SetBold(bool tbBold) {
		this->mbBold = tbBold;
	}

	void SetOutline(float tWidth, Gdiplus::Color tColor) {
		this->mOutlineWidth = tWidth;
		this->mOutlineColor = tColor;
	}

	void SetAlign(Gdiplus::StringAlignment hAlign, Gdiplus::StringAlignment vAlign) {
		mAlignH = hAlign;
		mAlignV = vAlign;
	}

private:
	std::wstring mText;
	std::wstring mFont;
	
	float mFontSize;
	Gdiplus::Color mFontColor;
	float mStrokeWidth;
	bool mbBold;

	float mOutlineWidth;
	Gdiplus::Color mOutlineColor;

	Gdiplus::StringAlignment mAlignH;
	Gdiplus::StringAlignment mAlignV;
};

