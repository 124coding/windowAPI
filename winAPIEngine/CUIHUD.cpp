#include "CUIHUD.h"

#include "CTexture.h"
#include "CResourceMgr.h"

void CUIHUD::OnCreate()
{
	mTexture = CResourceMgr::Find<CTexture>(L"HPBAR");
}

void CUIHUD::Active()
{
}

void CUIHUD::InActive()
{
}

void CUIHUD::OnDestroy()
{
}

void CUIHUD::OnUpdate(float tDeltaTime)
{
}

void CUIHUD::OnLateUpdate(float tDeltaTime)
{
}

void CUIHUD::Render(HDC tHDC)
{
	TransparentBlt(tHDC
		, 0, 0
		, mTexture->GetWidth()
		, mTexture->GetHeight()
		, mTexture->GetDCMem()
		, 0, 0
		, mTexture->GetWidth() / 2.0f
		, mTexture->GetHeight()
		, RGB(255, 0, 255));
}

void CUIHUD::UIClear()
{
}
