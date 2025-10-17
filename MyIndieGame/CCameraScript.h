#pragma once

#include "CScript.h"
#include "CInputMgr.h"

class CCameraScript : public CScript
{
public:
	CCameraScript() {}
	~CCameraScript() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

private:
	CInputMgr* mInputMgr = CInputMgr::GetInst();
};

