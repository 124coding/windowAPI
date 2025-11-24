#include "CSceneMgr.h"
#include "CDontDestroyOnLoad.h"

#include "winMacro.h"

std::map< std::wstring, CScene*> CSceneMgr::mScenes = {};
CScene* CSceneMgr::mActiveScene = nullptr;
CScene* CSceneMgr::mDontDestroyOnLoadScene = nullptr;
bool CSceneMgr::mDontDestroyOnLoad = false;

std::vector<GameObject*> CSceneMgr::GetGameObjects(eLayerType tLayer)
{
	std::vector<GameObject*> gameObjects = mActiveScene->GetLayer(tLayer)->GetGameObjects();
	std::vector<GameObject*> dontDestroyOnLoadGameObjs = mDontDestroyOnLoadScene->GetLayer(tLayer)->GetGameObjects();

	gameObjects.insert(gameObjects.end(), dontDestroyOnLoadGameObjs.begin(), dontDestroyOnLoadGameObjs.end());

	return gameObjects;
}

void CSceneMgr::OnCreate()
{
	mDontDestroyOnLoadScene = CreateScene<CDontDestroyOnLoad>(L"DontDestroyOnLoad");
	SetDontDestroyOnLoad(true);
}

void CSceneMgr::OnDestroy()
{
	mActiveScene->OnExit();

	for (auto& it : mScenes) {
		it.second->OnDestroy();
		SAFE_DELETE(it.second);
	}

	mActiveScene = nullptr;
	mDontDestroyOnLoadScene = nullptr;
	mScenes.clear();
}

void CSceneMgr::OnUpdate(float tDeltaTime)
{
	mActiveScene->OnUpdate(tDeltaTime);
	if (mDontDestroyOnLoad) {
		mDontDestroyOnLoadScene->OnUpdate(tDeltaTime);
	}
}

void CSceneMgr::OnLateUpdate(float tDeltaTime)
{
	mActiveScene->OnLateUpdate(tDeltaTime);
	if (mDontDestroyOnLoad) {
		mDontDestroyOnLoadScene->OnLateUpdate(tDeltaTime);
	}
}

void CSceneMgr::Render(HDC tHDC)
{
	mActiveScene->Render(tHDC);
	if (mDontDestroyOnLoad) {
		mDontDestroyOnLoadScene->Render(tHDC);
	}
}

void CSceneMgr::RemoveDeadObjects()
{
	for (auto& scene : mScenes) {
		scene.second->RemoveDeadObjects();
	}
}