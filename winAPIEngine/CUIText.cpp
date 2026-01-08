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

void CUIText::Render(HDC tHDC)
{
    if (!this->GetEnabled()) return;

    if (mText == L"" && mFragments.empty()) return;

	Gdiplus::Graphics graphics(tHDC);

    // 텍스트 계단 현상 제거 (안티앨리어싱)
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::FontFamily fontFamily(mFont.c_str());
	int style = mbBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
    Gdiplus::Font font(&fontFamily, mFontSize, style, Gdiplus::UnitPixel);

    // 텍스트 포맷 설정 (영역 밖 클리핑 방지 등)
    Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoClip);
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    // 측정 정확도를 높이기 위한 플래그 (공백 포함 계산)
    INT flags = Gdiplus::StringFormat::GenericTypographic()->GetFormatFlags();
    flags |= Gdiplus::StringFormatFlagsMeasureTrailingSpaces;
    format.SetFormatFlags(flags);

    // 1. 기준 줄 높이(Line Height) 측정
    Gdiplus::RectF measureBox;
    Gdiplus::PointF zero(0.0f, 0.0f);
    graphics.MeasureString(L"TestHeight", -1, &font, zero, Gdiplus::StringFormat::GenericTypographic(), &measureBox);

    float lineHeight = measureBox.Height;

    // 2. 수직 정렬(Vertical Alignment) 계산
    float totalTextHeight = mCachedLines.size() * lineHeight;
    float startY = (float)mFinalPos.mY;

    if (mAlignV == Gdiplus::StringAlignmentCenter) {
        startY += (mHeight - totalTextHeight) / 2.0f;
    }
    else if (mAlignV == Gdiplus::StringAlignmentFar) {
        startY += (mHeight - totalTextHeight);
    }

    float currentY = startY;

    // 3. 미리 계산된 라인(Cached Lines)들을 순회하며 렌더링
    for (const auto& line : mCachedLines)
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

        // 한 줄 내의 텍스트 조각(Fragment)들을 순차적으로 그리기
        for (const auto& frag : line.frags)
        {
            float fragWidth = GetTextWidth(&graphics, &font, frag.text);

            // 외곽선(Outline) 효과를 위해 GraphicsPath 사용
            // (DrawString은 외곽선을 지원하지 않음)
            Gdiplus::RectF layoutRect(currentX, currentY, 1000.0f, 1000.0f);
            Gdiplus::GraphicsPath path(Gdiplus::FillModeWinding);

            path.AddString(frag.text.c_str(), -1, &fontFamily, style,
                mFontSize, layoutRect, &format);

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

// 텍스트 변경 시 파싱 및 레이아웃 재계산 수행
void CUIText::SetText(const std::wstring& tText)
{
    if (mText == tText) return;
    mText = tText;
    ParseRichText(mText);
    UpdateTextLayout();
}

void CUIText::SetColor(Gdiplus::Color tColor)
{
    this->mFontColor = tColor;
    if (!mText.empty()) {
        ParseRichText(mText);   // 기본 색상이 바뀌었으니 다시 파싱
        UpdateTextLayout();
    }
}

// 텍스트가 차지하는 실제 크기(너비, 높이) 계산 (UI 자동 크기 조절용)
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

// 문자열 내의 색상 태그(<c=#RRGGBB>...</c>)를 파싱하여 Fragment로 분리
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

        // 태그 앞부분의 일반 텍스트 추가
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

// GDI+ MeasureString을 이용한 정밀한 문자열 폭 계산
float CUIText::GetTextWidth(Gdiplus::Graphics* tGraphics, Gdiplus::Font* tFont, const std::wstring& tText)
{
    if (tText.empty()) return 0.0f;
    Gdiplus::RectF boundingBox;
    Gdiplus::PointF origin(0.0f, 0.0f);

    Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoClip);

    INT flags = Gdiplus::StringFormat::GenericTypographic()->GetFormatFlags();

    flags |= Gdiplus::StringFormatFlagsMeasureTrailingSpaces;

    format.SetFormatFlags(flags);

    format.SetAlignment(Gdiplus::StringAlignmentNear);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    tGraphics->MeasureString(tText.c_str(), -1, tFont, origin, &format, &boundingBox);

    return boundingBox.Width;
}

// 텍스트 조각들을 분석하여 '그릴 준비가 된' 라인 정보(mCachedLines)를 생성
// 매 프레임 호출되지 않고 데이터 변경 시에만 호출됨
void CUIText::UpdateTextLayout()
{
    mCachedLines.clear();

    if (mText.empty() && mFragments.empty()) return;

    HDC hDC = GetDC(NULL);
    {
        Gdiplus::Graphics graphics(hDC);

        // 폰트 및 포맷 설정 (측정용)
        Gdiplus::FontFamily fontFamily(mFont.c_str());
        int style = mbBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
        Gdiplus::Font font(&fontFamily, mFontSize, style, Gdiplus::UnitPixel);

        Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoClip);
        format.SetAlignment(Gdiplus::StringAlignmentNear);
        format.SetLineAlignment(Gdiplus::StringAlignmentNear);

        INT flags = Gdiplus::StringFormat::GenericTypographic()->GetFormatFlags();
        flags |= Gdiplus::StringFormatFlagsMeasureTrailingSpaces;
        format.SetFormatFlags(flags);

        mCachedLines.push_back(RenderLineInfo());

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

                    // 현재 줄에 조각 추가 및 길이 누적
                    mCachedLines.back().frags.push_back({ part, frag.color });
                    mCachedLines.back().width += partWidth;
                }

                if (isLineEnd) {
                    // 개행이 있으면 새로운 줄 생성
                    mCachedLines.push_back(RenderLineInfo());
                    remaining = remaining.substr(pos + 1);
                }
                else {
                    break;
                }
            }
        }
    }

    ReleaseDC(NULL, hDC);
}

// 자동 줄바꿈(Word Wrap) 처리를 위한 정적 헬퍼 함수
std::wstring CUIText::InsertLineBreaks(const std::wstring& tText, float tMaxWidth, const std::wstring& tFontName, float tFontSize, bool tIsBold) {
    if (tText.empty()) return L"";

    HDC hdc = GetDC(NULL);
    std::wstring result = L"";

    {
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::FontFamily fontFamily(tFontName.c_str());
        int style = tIsBold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
        Gdiplus::Font font(&fontFamily, tFontSize, style, Gdiplus::UnitPixel);

        Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoClip);
        INT flags = Gdiplus::StringFormat::GenericTypographic()->GetFormatFlags();
        flags |= Gdiplus::StringFormatFlagsMeasureTrailingSpaces;
        format.SetFormatFlags(flags);

        std::wstring currentLine = L"";
        std::wstring visibleLine = L"";     // 태그를 제외한 실제 보이는 문자열 (길이 측정용)

        float currentWidth = 0.0f;

        // 문자열을 순회
        for (size_t i = 0; i < tText.length(); ++i) {
            wchar_t c = tText[i];

            // 1. 태그(<...>)는 길이 계산에서 제외하고 그대로 통과
            if (c == L'<') {
                // 태그가 끝날 때(>)까지 그대로 currentLine에만 추가
                size_t endTag = tText.find(L'>', i);
                if (endTag != std::wstring::npos) {
                    std::wstring tag = tText.substr(i, endTag - i + 1);
                    currentLine += tag;
                    i = endTag;
                    continue;
                }
            }

            // 2. 강제 개행(\n) 처리
            if (c == L'\n') {
                result += currentLine + c;
                currentLine = L"";
                visibleLine = L"";
                continue;
            }

            currentLine += c;
            visibleLine += c;

            // 3. 현재 줄의 너비 측정
            Gdiplus::RectF box;
            Gdiplus::PointF origin(0, 0);
            graphics.MeasureString(visibleLine.c_str(), -1, &font, origin, &format, &box);

            // 4. 최대 너비 초과 시 줄바꿈 처리
            if (box.Width > tMaxWidth) {
                // 단어 중간이 끊기지 않도록 마지막 공백(띄어쓰기)을 찾음
                size_t lastSpace = currentLine.find_last_of(L' ');

                if (lastSpace != std::wstring::npos && lastSpace > 0) {
                    currentLine[lastSpace] = L'\n';     // 공백을 개행으로 교체

                    result += currentLine.substr(0, lastSpace + 1);

                    std::wstring remainder = currentLine.substr(lastSpace + 1);

                    // 단순히 남은 부분으로 다시 시작한다고 가정 (약간의 오차 허용)
                    currentLine = remainder;

                    // visibleLine도 태그 제거하고 다시 갱신해야 함 (약식 처리)
                    visibleLine = L"";

                    for (auto wc : remainder) {
                        if (wc != L'<' && wc != L'>') visibleLine += wc;
                    }
                }
                else {
                    // 공백이 하나도 없는 긴 단어라면 그냥 강제로 자름
                    std::wstring temp = currentLine.substr(0, currentLine.length() - 1); // 현재 글자 빼고
                    result += temp + L"\n";
                    currentLine = std::wstring(1, c);
                    visibleLine = std::wstring(1, c);
                }
            }
        }

        result += currentLine;
    }
    ReleaseDC(NULL, hdc);

    return result;
}