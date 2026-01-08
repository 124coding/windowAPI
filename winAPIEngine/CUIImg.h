#pragma once

#include "CUIBase.h"

#include "CResourceMgr.h"

#include "CTexture.h"

class CUIImg : public CUIBase
{
public:
	// 이미지 렌더링 방식 정의
	enum class eImageMode {
		Stretch,		// UI 영역 크기에 맞춰 이미지를 강제로 늘림 (비율 깨질 수 있음)
		KeepAspect,		// 원본 비율을 유지하면서 UI 영역 안에 꽉 차게 맞춤 (Letterbox 발생 가능)
		OriginalCenter	// 크기 변환 없이 원본 사이즈 그대로 중앙 정렬 (UI보다 크면 잘림)
	};

	CUIImg(SVector2D tPos,
		float tWidth, float tHeight,
		CTexture* tTex = nullptr)
		: CUIBase(tPos,
			tWidth, tHeight, 
			eUIType::None),
		mTex(tTex) {

		// 이미지는 보통 배경이나 아이콘 등 단순 장식용이 많으므로
		// 기본적으로 마우스 이벤트를 무시(통과)하도록 설정
		mIgnoreMouse = true;
	}
	virtual ~CUIImg() {}

	virtual void OnCreate() override;
	virtual void Active() override;
	virtual void InActive() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;		// 설정된 모드에 따라 그리기 수행
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

	eImageMode mImgMode = eImageMode::Stretch;	// 기본값: 꽉 채우기
};

