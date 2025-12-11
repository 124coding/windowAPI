#include "CUIText.h"


void CUIText::OnCreate()
{
	CUIBase::OnCreate();
}

void CUIText::Active()
{
	CUIBase::Active();
}

void CUIText::InActive()
{
	CUIBase::InActive();
}

void CUIText::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CUIText::OnUpdate(float tDeltaTime)
{
	CUIBase::OnUpdate(tDeltaTime);
}

void CUIText::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

// 줄바꿈 전용 구조체
struct RenderLineInfo {
    float width = 0.0f; // 이 줄의 총 가로 길이
    std::vector<CUIText::STextFragment> frags; // 이 줄에 포함된 텍스트 조각들
};

void CUIText::Render(HDC tHDC)
{
    if (mText == L"" && mFragments.empty()) return;

	Gdiplus::Graphics graphics(tHDC);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::FontFamily fontFamily(mFont.c_str());
	int style = mbBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
    Gdiplus::Font font(&fontFamily, mFontSize, style, Gdiplus::UnitPixel);

    // 텍스트 높이
    Gdiplus::RectF measureBox;
    Gdiplus::PointF zero(0.0f, 0.0f);
    graphics.MeasureString(L"TestHeight", -1, &font, zero,
        Gdiplus::StringFormat::GenericTypographic(), &measureBox);

    float lineHeight = measureBox.Height;

    std::vector<RenderLineInfo> lines;

    lines.push_back(RenderLineInfo());

    std::vector<STextFragment> sources;
    if (!mFragments.empty()) sources = mFragments;
    else sources.push_back({ mText, mFontColor });

    for (const auto& frag : sources)
    {
        std::wstring remaining = frag.text;

        while (true)
        {
            size_t pos = remaining.find(L'\n');

            // 현재 처리할 문자열
            std::wstring part;
            bool isLineEnd = (pos != std::wstring::npos);

            if (isLineEnd) part = remaining.substr(0, pos);
            else part = remaining;

            if (!part.empty()) {
                // 각 부분마다의 넓이
                float partWidth = GetTextWidth(&graphics, &font, part);

                // 줄 정보 갱신
                lines.back().frags.push_back({ part, frag.color });
                lines.back().width += partWidth;
            }

            if (isLineEnd) {
                // 개행이 있으면 새로운 줄 생성
                lines.push_back(RenderLineInfo());
                remaining = remaining.substr(pos + 1);
            }
            else {
                break;
            }
        }
    }

    float totalTextHeight = lines.size() * lineHeight;
    float startY = (float)mFinalPos.mY;

    if (mAlignV == Gdiplus::StringAlignmentCenter) {
        startY += (mHeight - totalTextHeight) / 2.0f;
    }
    else if (mAlignV == Gdiplus::StringAlignmentFar) {
        startY += (mHeight - totalTextHeight);
    }

    float currentY = startY;

    for (const auto& line : lines)
    {
        if (line.frags.empty()) {
            currentY += lineHeight;
            continue;
        }

        // 수평(Horizontal) 정렬 계산
        float startX = (float)mFinalPos.mX;

        if (mAlignH == Gdiplus::StringAlignmentCenter) {
            startX += (mWidth - line.width) / 2.0f;
        }
        else if (mAlignH == Gdiplus::StringAlignmentFar) {
            startX += (mWidth - line.width);
        }

        float currentX = startX;
        for (const auto& frag : line.frags)
        {
            float fragWidth = GetTextWidth(&graphics, &font, frag.text);

            Gdiplus::RectF layoutRect(currentX, currentY, 1000.0f, 1000.0f);
            Gdiplus::GraphicsPath path(Gdiplus::FillModeWinding);

            path.AddString(frag.text.c_str(), -1, &fontFamily, style,
                mFontSize, layoutRect, Gdiplus::StringFormat::GenericTypographic());

            // 외곽선
            if (mOutlineWidth > 0.0f) {
                float totalWidth = mStrokeWidth + (mOutlineWidth * 2.0f);
                Gdiplus::Pen outlinePen(mOutlineColor, totalWidth);
                outlinePen.SetLineJoin(Gdiplus::LineJoinRound);
                graphics.DrawPath(&outlinePen, &path);
            }

            // 두께
            if (mStrokeWidth > 0.0f) {
                Gdiplus::Pen bodyPen(frag.color, mStrokeWidth);
                bodyPen.SetLineJoin(Gdiplus::LineJoinRound);
                graphics.DrawPath(&bodyPen, &path);
            }

            // 내부 채우기
            Gdiplus::SolidBrush brush(frag.color);
            graphics.FillPath(&brush, &path);

            currentX += fragWidth;
        }

        // 다음 줄로 이동
        currentY += lineHeight;
    }

    CUIBase::Render(tHDC);
}

void CUIText::UIClear()
{
	CUIBase::UIClear();
}

void CUIText::SetText(const std::wstring& tText)
{
    mText = tText;
    ParseRichText(mText);
}

void CUIText::SetColor(Gdiplus::Color tColor)
{
    this->mFontColor = tColor;
    if (!mText.empty()) {
        ParseRichText(mText);
    }
}

Gdiplus::SizeF CUIText::CalculateTextSize()
{
    if (mText.empty() && mFragments.empty()) return Gdiplus::SizeF(0.0f, 0.0f);

    // 임시 Graphics 객체 생성
    HDC hDC = GetDC(NULL);
    Gdiplus::Graphics graphics(hDC);

    // 폰트 설정
    Gdiplus::FontFamily fontFamily(mFont.c_str());
    int style = mbBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
    Gdiplus::Font font(&fontFamily, mFontSize, style, Gdiplus::UnitPixel);

    // 줄 높이 측정
    Gdiplus::RectF measureBox;
    Gdiplus::PointF zero(0.0f, 0.0f);
    graphics.MeasureString(L"TestHeight", -1, &font, zero,
        Gdiplus::StringFormat::GenericTypographic(), &measureBox);
    float lineHeight = measureBox.Height;

    // 텍스트 프래그먼트 순회하며 너비/높이 계산
    float maxLineWidth = 0.0f;  // 가장 긴 줄의 너비
    float currentLineWidth = 0.0f; // 현재 줄의 너비
    int lineCount = 1;          // 총 줄 수

    std::vector<STextFragment> sources;
    if (!mFragments.empty()) sources = mFragments;
    else sources.push_back({ mText, mFontColor });

    for (const auto& frag : sources)
    {
        std::wstring remaining = frag.text;

        while (true)
        {
            size_t pos = remaining.find(L'\n');
            std::wstring part;
            bool isLineEnd = (pos != std::wstring::npos);

            if (isLineEnd) part = remaining.substr(0, pos);
            else part = remaining;

            if (!part.empty()) {
                float partWidth = GetTextWidth(&graphics, &font, part);
                currentLineWidth += partWidth;
            }

            if (isLineEnd) {
                if (currentLineWidth > maxLineWidth) maxLineWidth = currentLineWidth;

                currentLineWidth = 0.0f;
                lineCount++;
                remaining = remaining.substr(pos + 1);
            }
            else {
                break;
            }
        }
    }

    if (currentLineWidth > maxLineWidth) maxLineWidth = currentLineWidth;

    ReleaseDC(NULL, hDC);

    return Gdiplus::SizeF(maxLineWidth, lineCount * lineHeight);
}

void CUIText::ParseRichText(const std::wstring& tText)
{
    mFragments.clear();
    std::wstring remaining = tText;

    std::wstring startTag = L"<c=";
    std::wstring endTag = L"</c>";

    while (!remaining.empty())
    {
        size_t startPos = remaining.find(startTag);

        // 태그가 없으면 남은 문자열 전부 기본색으로 추가 후 종료
        if (startPos == std::wstring::npos) {
            mFragments.push_back({ remaining, mFontColor });
            break;
        }

        if (startPos > 0) {
            mFragments.push_back({ remaining.substr(0, startPos), mFontColor });
        }

        // 태그 분석 시작
        remaining = remaining.substr(startPos);

        size_t closeBracketPos = remaining.find(L">");
        size_t endTagPos = remaining.find(endTag);

        if (closeBracketPos != std::wstring::npos && endTagPos != std::wstring::npos)
        {
            // "<c=" 길이가 3이므로 인덱스 3부터 '>' 앞까지
            std::wstring colorCode = remaining.substr(3, closeBracketPos - 3);
            Gdiplus::Color color = HexToColor(colorCode);

            // 실제 내용 추출
            std::wstring content = remaining.substr(closeBracketPos + 1, endTagPos - closeBracketPos - 1);

            mFragments.push_back({ content, color });

            // 처리한 부분 잘라내기
            remaining = remaining.substr(endTagPos + endTag.length());
        }
        else {
            // 태그 형식이 잘못되었으면 루프 탈출
            mFragments.push_back({ remaining, mFontColor });
            break;
        }
    }
}

Gdiplus::Color CUIText::HexToColor(const std::wstring& tHex)
{
    if (tHex.empty() || tHex[0] != L'#') return mFontColor;

    std::wstring hexCode = tHex.substr(1);

    // 포맷 맞추기 (RRGGBB -> AARRGGBB)
    if (hexCode.length() == 6) {
        hexCode = L"FF" + hexCode;
    }

    // 문자열을 unsigned long(정수)으로 변환
    unsigned int colorValue = 0;
    try {
        colorValue = std::stoul(hexCode, nullptr, 16);
    }
    catch (...) {
        return mFontColor; // 변환 실패 시 기본색 반환
    }

    return Gdiplus::Color(colorValue);
}

float CUIText::GetTextWidth(Gdiplus::Graphics* tGraphics, Gdiplus::Font* tFont, const std::wstring& tText)
{
    if (tText.empty()) return 0.0f;
    Gdiplus::RectF boundingBox;
    Gdiplus::PointF origin(0.0f, 0.0f);

    // GenericTypographic을 써야 여백 없이 정확한 너비가 나옴
    tGraphics->MeasureString(tText.c_str(), -1, tFont, origin,
        Gdiplus::StringFormat::GenericTypographic(), &boundingBox);

    return boundingBox.Width;
}