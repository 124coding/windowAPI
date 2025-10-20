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
		void operator=(std::function<void()> func) {
			mEvent = std::move(func);
		}

		void operator()() {
			if (mEvent)
				mEvent();
		}
		std::function<void()> mEvent;
	};

	CUIBase(eUIType tType) : mType(tType), mbFullScreen(false), mbEnabled(false) {}
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

	SVector2D GetPos() {
		return this->mPos;
	}

	void SetSize(SVector2D tSize) {
		this->mSize = tSize;
	}

	SVector2D GetSize() {
		return this->mSize;
	}

protected:
	SVector2D mPos;
	SVector2D mSize;

private:
	eUIType mType;
	bool mbFullScreen;
	bool mbEnabled;
};