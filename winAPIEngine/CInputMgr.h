#pragma once

#include <windows.h>
#include <unordered_map>
#include <string>
#include <typeinfo>

enum class eKeyState {
	Down,
	Pressed,
	Up,
	None
};

struct SKeyInfo {
	std::string mStrName = "";
	DWORD mKeyInput = 0;
	eKeyState mState = eKeyState::None;
	bool bPressed = false;
};

class CInputMgr
{
public:
	void Update();

	static CInputMgr* GetInst();
	static void ReleaseInst();

	template<typename T>
	bool AddKeyInfo(const std::string& tStrKey, const T& tData) {
		SKeyInfo* tKeyInfo = new SKeyInfo();

		const char* tTypeName = typeid(T).name();

		if (strcmp(tTypeName, "char") == 0 || strcmp(tTypeName, "int")) {
			tKeyInfo->mKeyInput = (DWORD)tData;
		}
		else return false;

		tKeyInfo->mStrName = tStrKey;

		this->mMapKeyInfos.insert(make_pair(tStrKey, tKeyInfo));

		return true;
	}

	bool GetKeyDown(const std::string& tStrKey);
	bool GetKeyPressed(const std::string& tStrKey);
	bool GetKeyUp(const std::string& tStrKey);

private:
	static CInputMgr* mInst;

	CInputMgr() {};
	~CInputMgr();

	SKeyInfo* FindKeyInfo(const std::string& tStrName) {
		auto t = mMapKeyInfos.find(tStrName);

		if (t == mMapKeyInfos.end()) {
			return nullptr;
		}

		return t->second;
	}

private:
	std::unordered_map<std::string, SKeyInfo*> mMapKeyInfos;


};

