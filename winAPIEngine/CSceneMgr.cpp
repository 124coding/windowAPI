#include "CSceneMgr.h"

#include "winMacro.h"

std::map< std::wstring, CScene*> CSceneMgr::mScene = {};
CScene* CSceneMgr::mActiveScene = nullptr;

void CSceneMgr::OnCreate(CAPIEngine* tEngine)
{
}

void CSceneMgr::OnDestroy()
{
	for (auto& it : mScene) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}
}

void CSceneMgr::OnUpdate(float tDeltaTime)
{
	mActiveScene->OnUpdate(tDeltaTime);
}

void CSceneMgr::OnLateUpdate(float tDeltaTime)
{
	mActiveScene->OnLateUpdate(tDeltaTime);
}

void CSceneMgr::Render(HDC tHDC)
{
	mActiveScene->Render(tHDC);
}
