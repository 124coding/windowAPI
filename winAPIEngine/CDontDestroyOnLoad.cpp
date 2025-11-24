#include "CDontDestroyOnLoad.h"

void CDontDestroyOnLoad::OnCreate()
{
	CScene::OnCreate();
}

void CDontDestroyOnLoad::OnDestroy()
{
	CScene::OnDestroy();
}

void CDontDestroyOnLoad::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);

}

void CDontDestroyOnLoad::OnLateUpdate(float tDeltaTime)
{
	CScene::OnLateUpdate(tDeltaTime);
}

void CDontDestroyOnLoad::Render(HDC tHDC)
{
	CScene::Render(tHDC);
}

void CDontDestroyOnLoad::OnEnter()
{
	CScene::OnEnter();
}

void CDontDestroyOnLoad::OnExit()
{
	CScene::OnExit();
}
