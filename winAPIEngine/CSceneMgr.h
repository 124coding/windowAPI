#pragma once
#include "CScene.h"

#include <map>

class CSceneMgr
{
public:
	template<typename T>

	// 씬을 생성하고 매니저에 등록
	static CScene* CreateScene(const std::wstring& tName) {
		T* scene = new T();
		scene->SetName(tName);

		mActiveScene = scene;
		scene->OnCreate();

		mScenes.insert(make_pair(tName, scene));

		return scene;
	}

	// 씬 전환: [이전 씬 Exit] -> [포인터 교체] -> [새 씬 Enter]
	static CScene* LoadScene(const std::wstring& tName) {

		auto it = mScenes.find(tName);

		if (it == mScenes.end()) {
			return nullptr;
		}

		// 현재 실행 중인 씬이 있다면 종료 처리
		if (it->second != mActiveScene) {
			mActiveScene->OnExit();
		}

		// 씬 교체
		mActiveScene = it->second;

		// 새 씬 진입 처리 (초기화, 배치 등)
		mActiveScene->OnEnter();

		return it->second;
	}

	// 현재 활성화된 씬 반환
	static CScene* GetActiveScene() {
		return mActiveScene;
	}

	// 이름으로 특정 씬 검색 (전환하지 않고 포인터만 얻기 위함)
	static CScene* GetWantScene(const std::wstring& tName) {
		auto it = mScenes.find(tName);

		if (it == mScenes.end()) {
			return nullptr;
		}

		return it->second;
	}

	// 씬이 전환되어도 파괴되지 않는 오브젝트들이 모인 특수 씬 반환
	static CScene* GetDontDestroyOnLoadScene() { 
		return mDontDestroyOnLoadScene; 
	}

	static void SetDontDestroyOnLoad(bool tDontDestroyOnLoad) {
		mDontDestroyOnLoad = tDontDestroyOnLoad;
	}

	// 현재 활성 씬의 특정 레이어 오브젝트 목록 가져오기 헬퍼
	static std::vector<GameObject*> GetGameObjects(eLayerType tLayer);

	static void OnCreate();
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);

	// 각 씬의 사망한 오브젝트 제거 요청
	static void RemoveDeadObjects();

private:
	static std::map<std::wstring, CScene*> mScenes;	// 등록된 모든 씬
	static CScene* mActiveScene;					// 현재 실행 중인 씬
	static CScene* mDontDestroyOnLoadScene;			// 씬 전환 시에도 유지되는 가상의 씬

	static bool mDontDestroyOnLoad;
};