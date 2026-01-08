#include "CSceneMgr.h"
#include "CDontDestroyOnLoad.h"

#include "winMacro.h"

std::map< std::wstring, CScene*> CSceneMgr::mScenes = {};
CScene* CSceneMgr::mActiveScene = nullptr;
CScene* CSceneMgr::mDontDestroyOnLoadScene = nullptr;
bool CSceneMgr::mDontDestroyOnLoad = false;

// 특정 레이어에 있는 모든 오브젝트(현재 씬 + 유지 씬)를 가져옴
std::vector<GameObject*> CSceneMgr::GetGameObjects(eLayerType tLayer)
{
	// 1. 현재 활성 씬의 해당 레이어 오브젝트들 가져오기
	std::vector<GameObject*> gameObjects = mActiveScene->GetLayer(tLayer)->GetGameObjects();

	// 2. 유지(DDOL) 씬의 해당 레이어 오브젝트들 가져오기
	std::vector<GameObject*> dontDestroyOnLoadGameObjs = mDontDestroyOnLoadScene->GetLayer(tLayer)->GetGameObjects();

	// 3. 두 벡터 합치기 (Active Scene 뒤에 DDOL 오브젝트 추가)
	// 충돌 처리나 렌더링 시, 씬 구분 없이 해당 레이어의 모든 물체를 검사해야 하기 때문
	gameObjects.insert(gameObjects.end(), dontDestroyOnLoadGameObjs.begin(), dontDestroyOnLoadGameObjs.end());

	return gameObjects;
}

void CSceneMgr::OnCreate()
{
	// 씬 전환 시에도 파괴되지 않는 특수 씬 생성
	mDontDestroyOnLoadScene = CreateScene<CDontDestroyOnLoad>(L"DontDestroyOnLoad");
	SetDontDestroyOnLoad(true);
}

void CSceneMgr::OnDestroy()
{
	// 현재 씬 종료 처리
	if (mActiveScene) mActiveScene->OnExit();

	// 등록된 모든 씬 메모리 해제
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
	// 1. 현재 활성 씬 업데이트 (게임 로직)
	mActiveScene->OnUpdate(tDeltaTime);

	// 2. 유지 씬 업데이트
	// 두 씬이 병렬로 돌아가는 구조
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

	// 유지 씬에 있는 오브젝트(예: 전역 UI 등)도 그려줌
	if (mDontDestroyOnLoad) {
		mDontDestroyOnLoadScene->Render(tHDC);
	}
}

void CSceneMgr::RemoveDeadObjects()
{
	// 등록된 모든 씬(Active, Inactive, DDOL 포함)을 순회하며 삭제 대기 오브젝트 정리
	for (auto& scene : mScenes) {
		scene.second->RemoveDeadObjects();
	}
}