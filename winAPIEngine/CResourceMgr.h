#pragma once

#include "CResource.h"
#include "CAPIEngine.h"

#include "winMacro.h"

#include <map>
#include <assert.h>

class CResourceMgr
{
public:

	// 키 값으로 리소스를 찾아, 원하는 타입(T)으로 캐스팅하여 반환
	template <typename T>
	static T* Find(const std::wstring& tKey) {
		auto iter = mResources.find(tKey);

		if (iter == mResources.end()) return nullptr;

		// dynamic_cast를 사용하여 안전하게 다운캐스팅 (실패 시 nullptr)
		return dynamic_cast<T*>(iter->second);
	}

	// 리소스를 로드하되, 이미 로드되어 있다면 기존 것을 반환 (중복 로딩 방지)
	template <typename T>
	static T* Load(const std::wstring& tKey, const std::wstring& tPath) {
		// 1. 먼저 캐시(Map)에 있는지 확인
		T* resource = CResourceMgr::Find<T>(tKey);

		if (resource != nullptr) return resource;

		// 2. 없으면 새로 생성하고 파일 로딩 시도
		resource = new T();

		// 로딩 실패 시 assert를 터뜨려 개발자에게 알림 (디버깅 용이성)
		if ( FAILED(resource->Load(tPath))) assert(false);

		// 3. 리소스 정보 설정 및 맵에 등록
		resource->SetName(tKey);
		resource->SetPath(tPath);
		mResources.insert(std::make_pair(tKey, resource));

		return resource;
	}

	// 파일 로딩이 아니라, 코드 상에서 동적으로 생성한 리소스를 등록할 때 사용
	static void Insert(const std::wstring& tKey, CResource* tResource) {
		if (tKey == L"") {
			return;
		}

		if (tResource == nullptr) {
			return;
		}

		mResources.insert(std::make_pair(tKey, tResource));
	}

	// 특정 리소스 삭제 및 메모리 해제
	static void Delete(const std::wstring& tKey, CResource* tResource) {
		if (tKey == L"") {
			return;
		}

		if (tResource == nullptr) {
			return;
		}

		for (auto& resource : mResources) {
			if (resource.first == tKey) {
				SAFE_DELETE(resource.second);
				mResources.erase(tKey);
				break;
			}
		}
	}

	static void OnDestroy();

private:
	// 리소스 저장소
	static std::map<std::wstring, CResource*> mResources;
};

