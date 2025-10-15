#pragma once
#include "CScene.h"

#include <map>

class CSceneMgr
{
public:
	template<typename T>
	static CScene* CreateScene(CAPIEngine* tEngine, const std::wstring& tName) {
		T* scene = new T();
		scene->SetName(tName);
		mActiveScene = scene;
		scene->OnCreate(tEngine);

		mScenes.insert(make_pair(tName, scene));

		return scene;
	}

	static CScene* LoadScene(const std::wstring& tName) {
		if (mActiveScene) mActiveScene->OnExit();

		auto it = mScenes.find(tName);

		if (it == mScenes.end()) {
			return nullptr;
		}

		mActiveScene = it->second;

		mActiveScene->OnEnter();

		return it->second;
	}

	static CScene* GetActiveScene() {
		return mActiveScene;
	}

	static CScene* GetDontDestroyOnLoad() { 
		return mDontDestroyOnLoadScene; 
	}

	static void OnCreate(CAPIEngine* tEngine);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	static void RemoveDeadObjects();

private:
	static std::map<std::wstring, CScene*> mScenes;
	static CScene* mActiveScene;
	static CScene* mDontDestroyOnLoadScene;
};