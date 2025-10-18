#pragma once

#include "CEntity.h"
#include "CAPIEngine.h"

#include "Enums.h"

class CUIBase : public CEntity
{
public:
	CUIBase() {}
	virtual ~CUIBase() {}

	void OnCreate();
	void Active();
	void InActive();
	void OnDestroy();
	void OnUpdate(float tDeltaTime);
	void OnLateUpdate(float tDeltaTime);
	void Render(HDC tHDC);
	void UIClear();

	eUIType GetType() {
		return this->mType;
	}

	void SetFullScreen(bool tEnable) {
		this->mbEnabled = tEnable;
	}

	bool IsFullScreen() {
		return this->mbEnabled;
	}

private:
	eUIType mType;
	bool mbFullScreen;
	bool mbEnabled;
};

