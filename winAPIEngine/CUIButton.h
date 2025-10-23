#pragma once

#include "CUIBase.h"

#include "CTexture.h"
#include "CResourceMgr.h"

class CUIButton : public CUIBase
{
public:
	CUIButton() : CUIBase(eUIType::Button) {}
	virtual ~CUIButton() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;

	virtual void ButtonClick();

	virtual void SetMouseOutTexture(const std::wstring& tKey) {
		this->mMouseOutTexture = CResourceMgr::Find<CTexture>(tKey);
	}
	
	virtual void SetMouseInTexture(const std::wstring& tKey) {
		this->mMouseInTexture = CResourceMgr::Find<CTexture>(tKey);
	}
	//
	//virtual void SetClickEvent(std::function<void()> tFunc);

private:
	CTexture* mMouseOutTexture;
	CTexture* mMouseInTexture;
	SEvent mOnClick;
};