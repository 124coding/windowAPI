#pragma once

#include "CUIBase.h"
#include "CAPIEngine.h"

#include "Enums.h"

#include <stack>
#include <queue>
#include <unordered_map>

class CUIMgr
{
public:
	static void OnCreate();
	static void OnLoad(eUIType tType);
	static void OnDestroy();
	static void OnUpdate(float tDeltaTime);
	static void OnLateUpdate(float tDeltaTime);
	static void Render(HDC tHDC);
	static void OnComplete(CUIBase* tAddUI);

	static CUIBase* GetUI(eUIType tType);

	static CUIBase* CreateUI(eUIType tType);

	static void ClearUI(eUIType tType);

	// Update 루프 도중 컨테이너를 수정하면 이터레이터가 무효화되어 크래시가 날 수 있음.
	// 따라서 요청(Queue)만 받아두고 프레임 끝에서 실제 Pop/Delete를 수행함.
	static void ProcessPop();
	static void ProcessDelete();

	// UI를 화면에 띄움 (활성 목록에 추가)
	static void Push(eUIType tType);

	// UI를 화면에서 닫음 (비활성화 대기열에 추가)
	static void Pop(eUIType tType);

	// 현재 마우스 위치에 있는 UI를 찾아 호버(Hover) 이벤트를 처리
	// (가장 위에 있는 UI부터 역순으로 검사)
	static void CheckMouseHover();

	// 호버 상태 강제 초기화 (씬 전환 등에서 사용)
	static void CurHoverNull();

	// 특정 타입의 UI 포인터 직접 검색 (Unordered Map 조회)
	static CUIBase* FindUI(eUIType tType) {
		return mUIs.find(tType)->second;
	}

private:
	// 로드된 모든 UI를 보관하는 저장소 (캐싱용)
	static std::unordered_map<eUIType, CUIBase*> mUIs;
	
	// 현재 화면에 활성화되어 업데이트/렌더링 중인 UI 목록 (순서 중요: 뒤쪽이 화면 앞쪽)
	static std::vector<CUIBase*> mActiveUIs;

	// 요청 대기열 (프레임워크 안전성을 위한 버퍼)
	static std::queue<eUIType> mRequestUIQueue;	// 활성화 요청
	static std::queue<eUIType> mPopQueue;		// 닫기 요청
	static std::queue<eUIType> mDeleteQueue;	// 메모리 해제 요청

	// 마우스 호버 상태 추적 (이전 프레임에 호버된 UI)
	// OnMouseLeave(OutHover) 이벤트를 트리거하기 위해 필요
	static CUIBase* mPrevHoverUI;
};

