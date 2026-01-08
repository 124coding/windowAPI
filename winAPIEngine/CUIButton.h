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

	// 버튼 상태(Normal, Hover, Pressed, Selected)에 따른 렌더링 처리
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

	// 클릭 시 실행될 콜백 함수 등록 (람다 함수 등을 매개변수로 받음)
	virtual void SetEventClick(std::function<void()> tFunc) {
		mOnClick = std::move(tFunc);
	}


private:
	// 클릭 이벤트 처리기 (함수 포인터 래퍼)
	SEvent mOnClick;

	bool mIsSelected = false;	// 선택된 상태인지 여부 (하이라이트 표시 등에 활용)

	std::wstring mToolTipText;
};