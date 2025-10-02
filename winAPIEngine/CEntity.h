#pragma once

#include <string>

class CEntity
{
public:
	CEntity() :mName(L"") {}
	virtual ~CEntity() {}

	void SetName(const std::wstring& tName) {
		this->mName = tName;
	}

	std::wstring& GetName() {
		return this->mName;
	}

private:
	std::wstring mName;
};

