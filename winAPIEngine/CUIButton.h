#pragma once

#include "CUIBase.h"

#include "CTexture.h"
#include "CResourceMgr.h"

#include <functional>

class CUIButton : public CUIBase
{
public:
	CUIButton() : CUIBase(eUIType::None) {}
	virtual ~CUIButton() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	void SetMouseOutTexture(const std::wstring& tKey) {
		this->mMouseOutTexture = CResourceMgr::Find<CTexture>(tKey);
	}

	CTexture* GetMouseOutTexture() {
		return this->mMouseOutTexture;
	}
	
	void SetMouseInTexture(const std::wstring& tKey) {
		this->mMouseInTexture = CResourceMgr::Find<CTexture>(tKey);
	}

	CTexture* GetMouseInTexture() {
		return this->mMouseInTexture;
	}
	
	virtual void SetButtonClick(std::function<void()> tFunc) {
		mOnClick = std::move(tFunc);
	}

public:
	static CUIButton* CreateInvertButton(
		const std::wstring& tName
		, const std::wstring& tText
		, const std::wstring& tImagePath
		, float tX, float tY
		, float tPaddingX, float tPaddingY
		, Gdiplus::Color tNormalBgColor, Gdiplus::Color tNormalFgColor
		, Gdiplus::Color tHoverBgColor, Gdiplus::Color tHoverFgColor
		, float tFontSize = 24.0f
		, float tBold = 0.0f);


private:
	CTexture* mMouseOutTexture = nullptr;
	CTexture* mMouseInTexture = nullptr;
	SEvent mOnClick;
};