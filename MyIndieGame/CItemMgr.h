#pragma once

#include "CComponent.h"

#include <vector>

class CItemMgr : public CComponent
{
public:
	CItemMgr() : CComponent(eComponentType::ItemMgr) {};
	virtual ~CItemMgr() {};

	virtual CComponent* Clone() override {
		return new CItemMgr(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void PlusItem(std::wstring tName) {
		mItems.push_back(tName);
	};
private:
	std::vector<std::wstring> mItems;
};

