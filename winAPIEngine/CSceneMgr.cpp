#include "CSceneMgr.h"
#include "CDontDestroyOnLoad.h"

#include "winMacro.h"

std::map< std::wstring, CScene*> CSceneMgr::mScenes = {};
CScene* CSceneMgr::mActiveScene = nullptr;
CScene* CSceneMgr::mDontDestroyOnLoadScene = nullptr;

void CSceneMgr::OnCreate(CAPIEngine* tEngine)
{
	mDontDestroyOnLoadScene = CreateScene<CDontDestroyOnLoad>(tEngine, L"DontDestroyOnLoad");
}

void CSceneMgr::OnDestroy()
{
	for (auto& it : mScenes) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}
}

void CSceneMgr::OnUpdate(float tDeltaTime)
{
	mActiveScene->OnUpdate(tDeltaTime);
	mDontDestroyOnLoadScene->OnUpdate(tDeltaTime);
}

void CSceneMgr::OnLateUpdate(float tDeltaTime)
{
	mActiveScene->OnLateUpdate(tDeltaTime);
	mDontDestroyOnLoadScene->OnLateUpdate(tDeltaTime);
}

void CSceneMgr::Render(HDC tHDC)
{
	mActiveScene->Render(tHDC);
	mDontDestroyOnLoadScene->Render(tHDC);
}

void CSceneMgr::RemoveDeadObjects()
{
	for (auto& scene : mScenes) {
		scene.second->RemoveDeadObjects();
	}
}