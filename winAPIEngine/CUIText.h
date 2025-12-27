#pragma once

#include "CUIBase.h"

class CUIText : public CUIBase
{
public:
	struct STextFragment {
		std::wstring text;
		Gdiplus::Color color;
	};

	// 줄바꿈 전용 구조체
	struct RenderLineInfo {
		float width = 0.0f; // 이 줄의 총 가로 길이
		std::vector<CUIText::STextFragment> frags; // 이 줄에 포함된 텍스트 조각들
	};

	struct SAutoColor {
		std::wstring text;
		int refValue;

		SAutoColor(std::wstring t, int v) : text(t), refValue(v) {}
	};

	CUIText(SVector2D tPos,
		float tWidth, float tHeight,
		std::wstring tText = L"")
		: CUIBase(tPos, tWidth, tHeight, eUIType::None)
		, mText(tText)
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
	void SetText(const std::wstring& tText);

	void SetFont(const std::wstring& tFontName)
	{
		if (mFont == tFontName) return;
		mFont = tFontName;

		UpdateTextLayout();
	}

	void SetFontSize(float tFontSize) {
		if (mFontSize == tFontSize) return;
		mFontSize = tFontSize;

		UpdateTextLayout();
	}

	void SetColor(Gdiplus::Color tColor);

	void SetStrokeWidth(float tStrokeWidth) {
		this->mStrokeWidth = tStrokeWidth;
	}

	void SetBold(bool tBold)
	{
		if (mbBold == tBold) return;
		mbBold = tBold;

		UpdateTextLayout();
	}

	void SetOutline(float tWidth, Gdiplus::Color tColor) {
		this->mOutlineWidth = tWidth;
		this->mOutlineColor = tColor;
	}

	void SetAlign(Gdiplus::StringAlignment hAlign, Gdiplus::StringAlignment vAlign) {
		mAlignH = hAlign;
		mAlignV = vAlign;
	}

	Gdiplus::SizeF CalculateTextSize();

public:
	static std::wstring InsertLineBreaks(const std::wstring& text, float maxWidth, const std::wstring& fontName, float fontSize, bool isBold);

private:
	void ParseRichText(const std::wstring& text);

	// Hex 문자열을 Gdiplus::Color로 변환하는 함수
	Gdiplus::Color HexToColor(const std::wstring& hex);

	// 텍스트 폭 계산 함수
	float GetTextWidth(Gdiplus::Graphics* graphics, Gdiplus::Font* font, const std::wstring& text);

	void UpdateTextLayout();

private:
	std::vector<RenderLineInfo> mCachedLines;

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

	std::vector<STextFragment> mFragments;
};

