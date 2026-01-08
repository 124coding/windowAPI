#pragma once

#include <windows.h>
#include <unordered_map>
#include <string>
#include <typeinfo>

#include "SVector2D.h"

// 키의 상태를 4단계로 구분 (게임 로직 정교화에 필수)
enum class eKeyState {
	Down,
	Pressed,
	Up,
	None
};

// 개별 키에 대한 정보를 담는 구조체
struct SKeyInfo {
	std::string mStrName = "";
	DWORD mKeyInput = 0;
	eKeyState mState = eKeyState::None;
	bool bPressed = false;
};

class CInputMgr
{
public:
	// 매 프레임 호출되어 키 상태를 갱신하고 마우스 위치를 계산
	void Update(HWND tHWND);

	static CInputMgr* GetInst();
	static void ReleaseInst();

	// 키 이름과 실제 키 코드(char, int, VK_*)를 매핑하여 등록
	template<typename T>
	bool AddKeyInfo(const std::string& tStrKey, const T& tData) {
		SKeyInfo* tKeyInfo = new SKeyInfo();

		const char* tTypeName = typeid(T).name();

		// 입력된 데이터 타입에 따라 가상 키 코드로 변환 저장
		if (strcmp(tTypeName, "char") == 0 || strcmp(tTypeName, "int") == 0) {
			tKeyInfo->mKeyInput = (DWORD)tData;
		}
		else return false;

		tKeyInfo->mStrName = tStrKey;

		// 맵에 등록 (O(1) 접근을 위해 unordered_map 사용)
		mMapKeyInfos.insert(make_pair(tStrKey, tKeyInfo));

		return true;
	}

	// 런타임 중에 키 설정을 변경 (예: 옵션에서 키 바꿀 때)
	template<typename T>
	bool ChangeKeyInfo(const std::string& tStrKey, const T& tData) {
		SKeyInfo* tKeyInfo = new SKeyInfo();

		const char* tTypeName = typeid(T).name();

		if (strcmp(tTypeName, "char") == 0 || strcmp(tTypeName, "int")) {
			auto it = mMapKeyInfos.find(tStrKey);
			if (it == mMapKeyInfos.end()) {
				return false;
			}
			else {
				it->second->mKeyInput = (DWORD)tData;
			}
		}
		else return false;

		return true;
	}

	// ==========================================
	// State Checking (키 상태 확인)
	// ==========================================
	bool GetKeyDown(const std::string& tStrKey);
	bool GetKeyPressed(const std::string& tStrKey);
	bool GetKeyUp(const std::string& tStrKey);
	bool GetKeyNone(const std::string& tStrKey);

	// 현재 마우스 좌표 반환 (ScreenToClient 적용된 좌표)
	static SVector2D GetMousePosition() {
		return mMousePosition;
	}

private:
	// 씬 전환이나 포커스 이동 시 키 상태 초기화
	void KeyClear();

	// 윈도우 핸들을 기준으로 마우스 좌표 계산
	void GetMousePositionByWindow(HWND tHWND);

private:
	static CInputMgr* mInst;
	static SVector2D mMousePosition;

	CInputMgr() {};
	~CInputMgr();

	// 맵에서 키 정보 검색 헬퍼 함수
	SKeyInfo* FindKeyInfo(const std::string& tStrName) {
		auto t = mMapKeyInfos.find(tStrName);

		if (t == mMapKeyInfos.end()) {
			return nullptr;
		}

		return t->second;
	}

private:
	// 빠른 검색을 위한 해시 맵
	std::unordered_map<std::string, SKeyInfo*> mMapKeyInfos;
};