#pragma once

#include "CEntity.h"
#include "CAPIEngine.h"
#include "SVector2D.h"

#include "Enums.h"

#include <functional>

class CUIBase : public CEntity
{
public:
	// UI 이벤트를 처리하기 위한 함수 래퍼 (std::function 사용)
	struct SEvent {
		std::function<void()> mEvent;

		void operator=(std::function<void()> func) {
			mEvent = std::move(func);
		}

		void operator()() {
			if (mEvent)
				mEvent();
		}
	};

	CUIBase(SVector2D tPos,
		float tWidth, float tHeight, 
		eUIType tType) 
		: mPos(tPos), 
		mWidth(tWidth), mHeight(tHeight), 
		mType(tType), 
		mbFullScreen(false), 
		mbEnabled(true) {
	}
	virtual ~CUIBase() {}

	virtual void OnCreate();
	virtual void Active();				// UI 활성화 (보이기)
	virtual void InActive();			// UI 비활성화 (숨기기)
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);
	virtual void UIClear();				// UI 관련 리소스나 상태 초기화

	eUIType GetType() {
		return this->mType;
	}

	// 클리핑(부모 영역 밖 자르기) 사용 여부
	void SetUsedClipping(bool tUseClipping) {
		this->mbUseClipping = tUseClipping;
	}

	void SetFullScreen(bool tEnable) {
		this->mbFullScreen = tEnable;
	}

	bool IsFullScreen() {
		return this->mbFullScreen;
	}

	bool GetEnabled() {
		return this->mbEnabled;
	}

	// 로컬 좌표(부모 기준 상대 좌표) 설정
	void SetPos(SVector2D tPos) {
		this->mPos = tPos;
	}

	// 부모 계층을 순회하여 월드(스크린) 기준의 최종 좌표 계산
	SVector2D GetFinalPos() {
		SVector2D finalPos = mPos;
		if (mParent != nullptr) {
			SVector2D parentPos = mParent->GetFinalPos();	// 재귀적으로 부모 좌표 계산
			finalPos.mX += parentPos.mX;
			finalPos.mY += parentPos.mY;
		}

		this->mFinalPos = finalPos;
		return this->mFinalPos;
	}

	SVector2D GetPos() {
		return this->mPos;
	}

	void SetWidth(float tWidth) {
		this->mWidth = tWidth;
	}

	float GetWidth() {
		return this->mWidth;
	}

	void SetHeight(float tHeight) {
		this->mHeight = tHeight;
	}

	float GetHeight() {
		return this->mHeight;
	}

	// ==========================================
	// Hierarchy (계층 구조)
	// ==========================================

	CUIBase* GetParent() {
		return this->mParent;
	}

	// 자식 UI 추가 및 부모 설정
	void AddChild(CUIBase* tChild) {
		tChild->mParent = this;
		mChilds.push_back(tChild);
	}

	// 자식 UI 제거 (부모 연결 끊기)
	void RemoveChild(CUIBase* tChild) {
		if (tChild == nullptr) return;

		for (auto it = mChilds.begin(); it != mChilds.end(); it++) {
			if (tChild == *it) {
				tChild->mParent = nullptr;
				mChilds.erase(it);
				return;
			}
		}
	}

	void SetHover(bool tIsHover) {
		this->mIsHover = tIsHover;
	}

	bool GetIsHover() {
		return this->mIsHover;
	}

	// 마우스 이벤트 무시 여부 (Raycast 타겟 제외)
	void SetIgnoreMouse(bool tIgnore) {
		this->mIgnoreMouse = tIgnore;
	}

	bool GetIgnoreMouse() {
		return this->mIgnoreMouse;
	}

	void SetBackColor(Gdiplus::Color tColor) {
		this->mBackColor = tColor;
	}

	void SetCornerRadius(int tRadius) {
		this->mCornerRadius = tRadius;
	}

	int GetCornerRadius() {
		return this->mCornerRadius;
	}

public:
	// 마우스 호버 시작 이벤트 등록
	void SetEventHover(std::function<void()> tFunc) {
		this->mOnHover = std::move(tFunc);
	}

	// 호버 이벤트 실행
	void GetEventHover() {
		return this->mOnHover();
	}

	// 마우스 호버 종료(Leave) 이벤트 등록
	void SetEventOutHover(std::function<void()> tFunc) {
		this->mOutHover = std::move(tFunc);
	}

	// 호버 종료 이벤트 실행
	void GetEventOutHover() {
		return this->mOutHover();
	}

	// GDI+: 둥근 모서리 사각형 경로 생성 헬퍼
	void AddRoundedRectToPath(Gdiplus::GraphicsPath* path, Gdiplus::Rect rect, int cornerRadius);
	// 특정 UI 영역 밖에 있는지 검사 (충돌 체크)
	bool IsOutsideRect(CUIBase* tUIBase);
	// 마우스 좌표에 위치한 최상위(Leaf) UI 탐색 (Hit Test)
	CUIBase* FindTargetUI(SVector2D mousePos);
	// 부모를 변경하며 좌표 유지 여부 등을 처리
	CUIBase* Reparent(CUIBase* tNewParent, bool keepWorldPosition, SVector2D tWantPos = SVector2D());

protected:
	SVector2D mFinalPos;		// 스크린 기준 최종 좌표 (캐싱용)
	SVector2D mPos;				// 부모 기준 로컬 좌표
	float mWidth;
	float mHeight;

	bool mbUseClipping = false;

	CUIBase* mParent = nullptr;
	std::vector<CUIBase*> mChilds;

	SEvent mOnHover;			// 마우스 진입 시 실행될 콜백
	SEvent mOutHover;			// 마우스 이탈 시 실행될 콜백
	bool mIsHover = false;

	bool mIgnoreMouse = false;	// true일 경우 클릭/호버 감지 안 함

	Gdiplus::Color mBackColor = Gdiplus::Color(0, 0, 0, 0);

	int mCornerRadius = 0;		// 0보다 크면 둥근 사각형으로 렌더링

private:
	eUIType mType;
	bool mbFullScreen;
	bool mbEnabled;
};