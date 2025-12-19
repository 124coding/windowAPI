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

	CUIImg(SVector2D tPos,
		float tWidth, float tHeight,
		CTexture* tTex = nullptr)
		: CUIBase(tPos,
			tWidth, tHeight, 
			eUIType::None),
		mTex(tTex) {
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
	
	void SetTexture(CTexture* tTex) {
		mTex = tTex;
	}

	CTexture* GetTexture() {
		return this->mTex;
	}

	void SetImageMode(eImageMode mode) {
		this->mImgMode = mode; 
	}

private:
	CTexture* mTex = nullptr;

	eImageMode mImgMode = eImageMode::Stretch;
};

