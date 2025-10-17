#include "CSceneMgr.h"
#include "CDontDestroyOnLoad.h"

#include "winMacro.h"

std::map< std::wstring, CScene*> CSceneMgr::mScenes = {};
CScene* CSceneMgr::mActiveScene = nullptr;
CScene* CSceneMgr::mDontDestroyOnLoadScene = nullptr;

std::vector<GameObject*> CSceneMgr::GetGameObjects(eLayerType tLayer)
{
	std::vector<GameObject*> gameObjects = mActiveScene->GetLayer(tLayer)->GetGameObjects();
	std::vector<GameObject*> dontDestroyOnLoadGameObjs = mDontDestroyOnLoadScene->GetLayer(tLayer)->GetGameObjects();

	gameObjects.insert(gameObjects.end(), dontDestroyOnLoadGameObjs.begin(), dontDestroyOnLoadGameObjs.end());

	return gameObjects;
}

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