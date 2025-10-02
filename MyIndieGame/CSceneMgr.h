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
		scene->OnCreate();

		mScene.insert(make_pair(tName, scene));

		return scene;
	}

	static CScene* LoadScene(const std::wstring& tName) {
		if (mActiveScene) mActiveScene->OnExit();

		auto it = mScene.find(tName);

		if (it == mScene.end()) {
			return nullptr;
		}

		mActiveScene = it->second;

		mActiveScene->OnEnter();

		return it->second;
	}

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC hDC);

private:
	static std::map<std::wstring, CScene*> mScene;
	static CScene* mActiveScene;
};

