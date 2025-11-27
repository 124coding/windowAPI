#pragma once

#include "CEntity.h"
#include "CAPIEngine.h"
#include "SVector2D.h"

#include "Enums.h"

#include <functional>

class CUIBase : public CEntity
{
public:
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

	CUIBase(eUIType tType) : mType(tType), mbFullScreen(false), mbEnabled(true) {}
	virtual ~CUIBase() {}

	virtual void OnCreate();
	virtual void Active();
	virtual void InActive();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);
	virtual void OnLateUpdate(float tDeltaTime);
	virtual void Render(HDC tHDC);
	virtual void UIClear();

	eUIType GetType() {
		return this->mType;
	}

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

	void SetPos(SVector2D tPos) {
		this->mPos = tPos;
	}

	SVector2D GetFinalPos() {
		SVector2D finalPos = mPos;
		if (mParent != nullptr) {
			SVector2D parentPos = mParent->GetFinalPos();
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

	void AddChild(CUIBase* tChild) {
		tChild->mParent = this;
		mChilds.push_back(tChild);
	}

	void SetHover(bool tIsHover) {
		this->mIsHover = tIsHover;
	}

	bool GetIsHover() {
		return this->mIsHover;
	}

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
	void SetEventHover(std::function<void()> tFunc) {
		this->mOnHover = std::move(tFunc);
	}

	void GetEventHover() {
		return this->mOnHover();
	}

	void SetEventOutHover(std::function<void()> tFunc) {
		this->mOutHover = std::move(tFunc);
	}

	void GetEventOutHover() {
		return this->mOutHover();
	}

	void AddRoundedRectToPath(Gdiplus::GraphicsPath* path, Gdiplus::Rect rect, int cornerRadius);
	bool IsOutsideRect(CUIBase* tUIBase);
	CUIBase* FindTargetUI(SVector2D mousePos);

protected:
	SVector2D mFinalPos;
	SVector2D mPos;
	float mWidth;
	float mHeight;

	bool mbUseClipping;

	CUIBase* mParent = nullptr;
	std::vector<CUIBase*> mChilds;

	SEvent mOnHover;
	SEvent mOutHover;
	bool mIsHover = false;

	bool mIgnoreMouse = false;

	Gdiplus::Color mBackColor = Gdiplus::Color(0, 0, 0, 0);

	int mCornerRadius = 0;

private:
	eUIType mType;
	bool mbFullScreen;
	bool mbEnabled;
};