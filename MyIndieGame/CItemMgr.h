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

	int PlusItem(std::wstring tName) {
		int index = 0;
		for (auto& item : mItems) {
			if (item.first == tName) {
				item.second++;
				return index;
			}
			index++;
		}
		
		mItems.push_back(std::make_pair(tName, 1));
		return index;
	};

	std::vector<std::pair<std::wstring, int>>& GetItems() {
		return this->mItems;
	}

	void ResetItems() {
		mItems.clear();
	}
private:
	std::vector<std::pair<std::wstring, int>> mItems;
};

