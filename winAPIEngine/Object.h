#pragma once

#include "CComponent.h"
#include "GameObject.h"
#include "CLayer.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "CCollider.h"

#include <Windows.h>

// 씬에 오브젝트를 생성하고 추가 (기본값: 현재 활성화된 씬)
template<typename T>
static T* Instantiate(eLayerType tType, CScene* targetScene = nullptr) {
	T* gameObject = new T();
	gameObject->SetLayerType(tType);

	CScene* scene = nullptr;

	// 타겟 씬이 지정되지 않았다면 Active Scene 사용
	if (targetScene != nullptr) {
		scene = targetScene;
	}
	else {
		scene = CSceneMgr::GetActiveScene();
	}

	// 해당 씬의 레이어에 오브젝트 등록
	CLayer* layer = scene->GetLayer(tType);

	layer->AddGameObject(gameObject);

	return gameObject;
}

// 위치를 지정하여 씬에 오브젝트를 생성하고 추가 (기본값: 현재 활성화된 씬)
template<typename T>
static T* Instantiate(eLayerType tType, SVector2D tPosition, CScene* targetScene = nullptr) {
	T* gameObject = new T();
	gameObject->SetLayerType(tType);

	CScene* scene = nullptr;

	if (targetScene != nullptr) {
		scene = targetScene;
	}
	else {
		scene = CSceneMgr::GetActiveScene();
	}

	CLayer* layer = scene->GetLayer(tType);
	layer->AddGameObject(gameObject);

	// Transform 컴포넌트를 통해 초기 위치 설정
	CTransform* tr = gameObject->GetComponent<CTransform>(eComponentType::Transform);
	tr->SetPos(tPosition);
	tr->SetName(L"TR");

	return gameObject;
}

// 이미 생성된 오브젝트 포인터를 받아 씬에 등록
template<typename T>
static void AddObjectToScene(T* tObj) {
	CScene* activeScene = CSceneMgr::GetActiveScene();

	CLayer* layer = activeScene->GetLayer(tObj->GetLayerType());
	layer->AddGameObject(tObj);
}

// 오브젝트 삭제 요청 (즉시 delete가 아닌 Death 상태로 전환)
static void ObjDestroy(GameObject* tObj) {
	tObj->Death();
}

// 씬 전환 시 오브젝트가 삭제되지 않도록 별도 씬(DontDestroyOnLoad)으로 이동
static void DontDestroyOnLoad(GameObject* tObj) {
	CScene* activeScene = CSceneMgr::GetActiveScene();
	activeScene->EraseGameObject(tObj); // 현재 씬 관리 목록에서 제거

	CScene* dontDestroyOnLoad = CSceneMgr::GetDontDestroyOnLoadScene();
	dontDestroyOnLoad->AddGameObject(tObj, tObj->GetLayerType()); // 유지되는 씬에 재등록

}

// 텍스처 크기와 오브젝트의 스케일을 곱해 실제 렌더링 크기 반환
static SVector2D ObjectSize(GameObject* tObj) {
	CTransform* tr = tObj->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* sr = tObj->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	return SVector2D(tObj->GetSize().mX * sr->GetTexture()->GetWidth(), tObj->GetSize().mY * sr->GetTexture()->GetHeight());
}

// 회전, 앵커 포인트, 콜라이더 오프셋을 모두 고려한 월드 상의 실제 중심 좌표 계산
static SVector2D ObjectCenterPos(GameObject* tObj) {
	CTransform* tr = tObj->GetComponent<CTransform>(eComponentType::Transform);
	CCollider* cl = tObj->GetComponent<CCollider>(eComponentType::Collider);

	// 회전 변환을 위한 삼각함수 값 미리 계산
	float fCos = cos(DegToRad(tr->GetRot()));
	float fSin = sin(DegToRad(tr->GetRot()));

	SVector2D vLocalLC, vRotC, centerPos;

	// 콜라이더 유무에 따라 중심점 기준이 달라짐
	if (cl == nullptr) {
		// 1. 콜라이더가 없을 때: 앵커 포인트를 기준으로 로컬 중심 계산
		vLocalLC.mX = -tObj->GetAnchorPoint().mX * tObj->GetSize().mX;
		vLocalLC.mY = -tObj->GetAnchorPoint().mY * tObj->GetSize().mY;

		vLocalLC = vLocalLC + (ObjectSize(tObj) / 2); // 텍스처 크기의 절반만큼 보정

		// 2. 회전 행렬 적용 (Rotation)
		vRotC = SVector2D(vLocalLC.mX * fCos - vLocalLC.mY * fSin, vLocalLC.mX * fSin + vLocalLC.mY * fCos);
	}
	else {
		// 1. 콜라이더가 있을 때: 콜라이더의 오프셋과 크기를 반영하여 계산
		vLocalLC.mX = -tObj->GetAnchorPoint().mX * tObj->GetSize().mX * cl->GetSize().mX + cl->GetOffset().mX;
		vLocalLC.mY = -tObj->GetAnchorPoint().mY * tObj->GetSize().mY * cl->GetSize().mY + cl->GetOffset().mY;

		vLocalLC = vLocalLC + (ObjectSize(tObj) * cl->GetSize() / 2);

		// 2. 회전 행렬 적용
		vRotC = SVector2D(vLocalLC.mX * fCos - vLocalLC.mY * fSin, vLocalLC.mX * fSin + vLocalLC.mY * fCos);
	}

	// 3. 최종 월드 좌표 (오브젝트 위치 + 회전된 중심 오프셋)
	centerPos = tr->GetPos() + vRotC;

	return centerPos;
}