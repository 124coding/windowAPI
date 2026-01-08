#include "CResourceMgr.h"
#include "winMacro.h"

std::map<std::wstring, CResource*> CResourceMgr::mResources = {};

void CResourceMgr::OnDestroy() {
	// 府家胶 老褒 秦力
	for (auto& resource : mResources) {
		if (resource.second != nullptr) {
			SAFE_DELETE(resource.second);
		}
	}

	mResources.clear();
}