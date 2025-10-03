#pragma once

#include "CResource.h"
#include "CAPIEngine.h"

#include <map>
#include <assert.h>

class CResourceMgr
{
public:
	template <typename T>
	static T* Find(const std::wstring& tKey) {
		auto iter = mResources.find(tKey);

		if (iter == mResources.end()) return nullptr;

		return dynamic_cast<T*>(iter->second);
	}

	template <typename T>
	static T* Load(CAPIEngine* tEngine, const std::wstring& tKey, const std::wstring& tPath) {
		T* resource = CResourceMgr::Find<T>(tKey);

		if (resource != nullptr) return resource;

		resource = new T();
		if ( FAILED(resource->Load(tEngine, tPath))) assert(false);

		resource->SetName(tKey);
		resource->SetPath(tPath);
		mResources.insert(std::make_pair(tKey, resource));

		return resource;
	}

private:
	static std::map<std::wstring, CResource*> mResources;
};

