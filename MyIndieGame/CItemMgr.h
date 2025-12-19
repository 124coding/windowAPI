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
		for (auto& item : mItems) {
			if (item.first == tName) {
				item.second++;
				return;
			}
		}
		
		mItems.push_back(std::make_pair(tName, 1));
	};

	std::vector<std::pair<std::wstring, int>> GetItems() {
		return this->mItems;
	}
private:
	std::vector<std::pair<std::wstring, int>> mItems;
};

