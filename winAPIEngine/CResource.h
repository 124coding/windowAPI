#pragma once

#include "CEntity.h"
#include "Enums.h"
#include "CAPIEngine.h"

#include <string>
#include <Windows.h>

class CResource : public CEntity
{
public:
	CResource(eResourceType tType) : mType(tType) {}
	virtual ~CResource() {}

	virtual HRESULT Load(const std::wstring& tPath) = 0;

	void SetPath(const std::wstring& tPath) {
		this->mPath = tPath;
	}

	const std::wstring& GetPath() {
		return this->mPath;
	}

private:
	const eResourceType mType;
	std::wstring mPath; 
};

