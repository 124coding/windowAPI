#pragma once

#include "CUIBase.h"

class CUIText : public CUIBase
{
public:
	// 색상 등 서식이 적용된 최소 텍스트 단위 (예: "Hello"는 빨강, "World"는 파랑이면 2개의 Fragment)
	struct STextFragment {
		std::wstring text;
		Gdiplus::Color color;
	};

	/// 한 줄(Line)에 대한 렌더링 캐시 정보
	// 정렬(Alignment) 처리를 위해 줄 단위의 폭과 포함된 조각들을 미리 계산해둠
	struct RenderLineInfo {
		float width = 0.0f; // 이 줄의 총 가로 길이
		std::vector<CUIText::STextFragment> frags; // 이 줄에 포함된 텍스트 조각들
	};

	// (참고용) 특정 값에 따라 텍스트 색상을 자동 지정하기 위한 구조체
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

		// 텍스트 UI는 기본적으로 마우스 이벤트를 가로채지 않음 (클릭 투과)
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
	// 텍스트 변경: 내용을 파싱하고 레이아웃을 갱신함
	void SetText(const std::wstring& tText);

	void SetFont(const std::wstring& tFontName)
	{
		if (mFont == tFontName) return;
		mFont = tFontName;

		// 폰트가 바뀌면 글자 크기가 달라지므로 레이아웃 재계산 필수
		UpdateTextLayout();
	}

	void SetFontSize(float tFontSize) {
		if (mFontSize == tFontSize) return;
		mFontSize = tFontSize;

		UpdateTextLayout();
	}

	// 기본 텍스트 색상 설정
	void SetColor(Gdiplus::Color tColor);

	// 글자 획 굵기 (GDI+ Pen width)
	void SetStrokeWidth(float tStrokeWidth) {
		this->mStrokeWidth = tStrokeWidth;
	}

	void SetBold(bool tBold)
	{
		if (mbBold == tBold) return;
		mbBold = tBold;

		UpdateTextLayout();
	}

	// 텍스트 외곽선 설정
	void SetOutline(float tWidth, Gdiplus::Color tColor) {
		this->mOutlineWidth = tWidth;
		this->mOutlineColor = tColor;
	}

	// 가로/세로 정렬 설정 (Gdiplus::StringAlignment 사용)
	void SetAlign(Gdiplus::StringAlignment hAlign, Gdiplus::StringAlignment vAlign) {
		mAlignH = hAlign;
		mAlignV = vAlign;
	}

	// 현재 설정된 텍스트가 차지하는 실제 크기 계산 (배경 박스 크기 조정 등에 사용)
	Gdiplus::SizeF CalculateTextSize();

public:
	// 지정된 너비(maxWidth)를 넘어가면 줄바꿈 문자를 삽입하여 반환하는 정적 헬퍼 함수
	static std::wstring InsertLineBreaks(const std::wstring& text, float maxWidth, const std::wstring& fontName, float fontSize, bool isBold);

private:
	// 리치 텍스트 파싱: 문자열 내의 태그나 규칙을 분석하여 mFragments로 분리
	void ParseRichText(const std::wstring& text);

	// Hex 문자열을 Gdiplus::Color로 변환하는 함수
	Gdiplus::Color HexToColor(const std::wstring& hex);

	// GDI+를 이용한 실제 텍스트 폭 측정
	float GetTextWidth(Gdiplus::Graphics* graphics, Gdiplus::Font* font, const std::wstring& text);

	// 텍스트 조각들을 분석하여 실제 렌더링될 라인(mCachedLines)을 구축
	// 매 프레임 계산하지 않고, 속성이 변할 때만 호출됨
	void UpdateTextLayout();

private:
	// 렌더링 최적화를 위한 캐시 데이터
	std::vector<RenderLineInfo> mCachedLines;
	std::vector<STextFragment> mFragments;

	std::wstring mText;
	std::wstring mFont;

	float mFontSize;
	Gdiplus::Color mFontColor;
	float mStrokeWidth;
	bool mbBold;

	// 외곽선 효과
	float mOutlineWidth;
	Gdiplus::Color mOutlineColor;

	// 정렬 옵션
	Gdiplus::StringAlignment mAlignH;
	Gdiplus::StringAlignment mAlignV;
};

