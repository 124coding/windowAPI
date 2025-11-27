#pragma once

#include "CUIBase.h"

#include "CResourceMgr.h"

#include "CTexture.h"

class CUIImg : public CUIBase
{
public:
	enum class eImageMode {
		Stretch,
		KeepAspect,
		OriginalCenter
	};

	CUIImg() : CUIBase(eUIType::None) {
		mIgnoreMouse = true;
	}
	virtual ~CUIImg() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;
	virtual void UIClear() override;
	
	void SetTexture(std::wstring tTexName) {
		mTex = CResourceMgr::Find<CTexture>(tTexName);
	}

	void SetImageMode(eImageMode mode) {
		this->mImgMode = mode; 
	}

private:
	CTexture* mTex = nullptr;

	eImageMode mImgMode = eImageMode::Stretch;
};

