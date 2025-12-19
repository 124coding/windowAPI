#pragma once

#include "CUIBase.h"

#include "CTexture.h"
#include "CResourceMgr.h"

#include <functional>

class CUIButton : public CUIBase
{
public:
	CUIButton(SVector2D tPos,
		float tWidth, float tHeight) 
		: CUIBase(tPos,
			tWidth, tHeight, eUIType::None) {}
	virtual ~CUIButton() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	void SetSelected(bool tBool) {
		this->mIsSelected = tBool;
	}

	bool IsSelected() {
		return this->mIsSelected;
	}

	void SetToolTipText(const std::wstring& text) {
		mToolTipText = text;
	}

	std::wstring GetToolTipText() {
		return mToolTipText;
	}

public:

	virtual void SetEventClick(std::function<void()> tFunc) {
		mOnClick = std::move(tFunc);
	}


private:
	SEvent mOnClick;

	bool mIsSelected = false;

	std::wstring mToolTipText;
};