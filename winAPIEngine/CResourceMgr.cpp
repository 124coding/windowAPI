#include "CResourceMgr.h"
#include "winMacro.h"

std::map<std::wstring, CResource*> CResourceMgr::mResources = {};

void CResourceMgr::OnDestroy() {
	for (auto& resource : mResources) {
		if (resource.second != nullptr) {
			SAFE_DELETE(resource.second);
		}
	}

	mResources.clear();
}