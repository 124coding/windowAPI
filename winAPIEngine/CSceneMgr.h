#pragma once
#include "CScene.h"

#include <map>

class CSceneMgr
{
public:
	template<typename T>
	static CScene* CreateScene(const std::wstring& tName) {
		T* scene = new T();
		scene->SetName(tName);
		mActiveScene = scene;
		scene->OnCreate();

		mScenes.insert(make_pair(tName, scene));

		return scene;
	}

	static CScene* LoadScene(const std::wstring& tName) {

		auto it = mScenes.find(tName);

		if (it == mScenes.end()) {
			return nullptr;
		}

		if (it->second != mActiveScene) {
			mActiveScene->OnExit();
		}

		mActiveScene = it->second;

		mActiveScene->OnEnter();

		return it->second;
	}

	static CScene* GetActiveScene() {
		return mActiveScene;
	}

	static CScene* GetDontDestroyOnLoadScene() { 
		return mDontDestroyOnLoadScene; 
	}

	static void SetDontDestroyOnLoad(bool tDontDestroyOnLoad) {
		mDontDestroyOnLoad = tDontDestroyOnLoad;
	}

	static std::vector<GameObject*> GetGameObjects(eLayerType tLayer);

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void RemoveDeadObjects();

private:
	static std::map<std::wstring, CScene*> mScenes;
	static CScene* mActiveScene;
	static CScene* mDontDestroyOnLoadScene;

	static bool mDontDestroyOnLoad;
};