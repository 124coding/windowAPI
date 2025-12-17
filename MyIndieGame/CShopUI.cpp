#include "CShopUI.h"

#include "StatSet.h"

#include "CDataMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CPlayerScript.h"
#include "CItemMgr.h"
#include "CWeaponMgr.h"
#include "CWeaponScript.h"

#include "CUIButton.h"

#include <regex>
#include <algorithm>

void CShopUI::OnCreate()
{
	SetWidth(windowWidth);
	SetHeight(windowHeight);

	// 기본적으로 필요한 데이터 가져오기
	CPlayer* pl = CPlayScene::GetPlayer();
	CPlayerScript* plSc = pl->GetComponent<CPlayerScript>();
	std::vector<std::wstring> plItems = pl->GetComponent<CItemMgr>()->GetItems();
	std::vector<CWeapon*> plWeapons = pl->GetComponent<CWeaponMgr>()->GetWeapons();

	// 상점의 메인이 될 패널
	mShopMainPanel = new CUIPanel();
	mShopMainPanel->SetWidth(3 * this->GetWidth() / 4);
	mShopMainPanel->SetHeight(this->GetHeight());

	this->AddChild(mShopMainPanel);


	CUIText* shopTex = new CUIText();

	shopTex->SetPos(SVector2D(20.0f, 20.0f));
	shopTex->SetWidth(200.0f);
	shopTex->SetHeight(40.0f);
	shopTex->SetText(L"상점");
	shopTex->SetColor(Gdiplus::Color::White);
	shopTex->SetFontSize(35.0f);
	shopTex->SetStrokeWidth(1.0f);
	shopTex->SetOutline(2.0f, Gdiplus::Color::Black);

	mShopMainPanel->AddChild(shopTex);


	CUIPanel* shopMoneyPanel = new CUIPanel();
	shopMoneyPanel->SetWidth(300.0f);
	shopMoneyPanel->SetHeight(40.0f);

	shopMoneyPanel->SetPos(SVector2D(mShopMainPanel->GetWidth() / 2 - shopMoneyPanel->GetWidth() / 2, 20.0f));

	mShopMainPanel->AddChild(shopMoneyPanel);


	CUIImg* shopMoneyIcon = new CUIImg();
	shopMoneyIcon->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	shopMoneyIcon->SetPos(SVector2D(0.0f, 5.0f));
	shopMoneyIcon->SetWidth(50.0f);
	shopMoneyIcon->SetHeight(40.0f);
	shopMoneyIcon->SetImageMode(CUIImg::eImageMode::KeepAspect);

	shopMoneyPanel->AddChild(shopMoneyIcon);

	mMoneyTex = new CUIText();
	mMoneyTex->SetPos(SVector2D(55.0f, 0.0f));
	mMoneyTex->SetWidth(mShopMainPanel->GetWidth() - shopMoneyIcon->GetWidth() - 5.0f);
	mMoneyTex->SetHeight(40.0f);
	mMoneyTex->SetFontSize(30.0f);
	mMoneyTex->SetColor(Gdiplus::Color::White);
	mMoneyTex->SetStrokeWidth(1.0f);
	mMoneyTex->SetOutline(2.0f, Gdiplus::Color::Black);

	shopMoneyPanel->AddChild(mMoneyTex);

	

	mResetButton = new CUIButton();
	mResetButton->SetHeight(50.0f);
	mResetButton->SetCornerRadius(10.0f);
	mResetButton->SetBackColor(Gdiplus::Color::Black);

	mResetTex = new CUIText();
	mResetTex->SetPos(SVector2D(10.0f, 0.0f));
	mResetTex->SetHeight(40.0f);
	mResetTex->SetFontSize(30.0f);
	mResetTex->SetStrokeWidth(1.0f);
	mResetTex->SetColor(Gdiplus::Color::White);

	mResetButton->AddChild(mResetTex);


	mResetImg = new CUIImg();
	mResetImg->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	mResetImg->SetWidth(50.0f);
	mResetImg->SetHeight(40.0f);
	mResetImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	mResetButton->AddChild(mResetImg);


	mResetButton->SetEventHover([=]() {
		mResetButton->SetBackColor(Gdiplus::Color::White);
		mResetTex->SetColor(Gdiplus::Color::Black);
		});
	mResetButton->SetEventOutHover([=]() {
		mResetButton->SetBackColor(Gdiplus::Color::Black);
		mResetTex->SetColor(Gdiplus::Color::White);
		});
	mResetButton->SetEventClick([=]() {

		});

	mShopMainPanel->AddChild(mResetButton);


	// 상점에서 구매할 수 있는 아이템들을 보여주는 패널
	CUIPanel* goodsPanel = new CUIPanel();
	goodsPanel->SetPos(SVector2D(0.0f, 100.0f));
	goodsPanel->SetWidth(mShopMainPanel->GetWidth());
	goodsPanel->SetHeight(mShopMainPanel->GetHeight() / 2);

	mShopMainPanel->AddChild(goodsPanel);


	for (int i = 0; i < 4; i++) {
		int itemCount = 4;
		float panelW = goodsPanel->GetWidth();
		float panelH = goodsPanel->GetHeight();
		float margin = 30.0f;

		float cellWidth = panelW / (float)itemCount;
		float itemWidth = cellWidth - margin;       
		float itemHeight = 4.0f * panelH / 5.0f;    

		mGoods[i] = MakeGoods(itemWidth, itemHeight);

		float xPos = (i * cellWidth) + (margin / 2.0f);
		float yPos = 5.0f;

		mGoods[i]->SetPos(SVector2D(xPos, yPos));

		goodsPanel->AddChild(mGoods[i]);
	}

	// 현재 가지고 있는 아이템들을 표시해주는 패널
	CUIPanel* haveItemPanel = new CUIPanel();
	haveItemPanel->SetPos(SVector2D(0.0f, goodsPanel->GetPos().mY + goodsPanel->GetHeight()));
	haveItemPanel->SetWidth(2 * goodsPanel->GetWidth() / 3);
	haveItemPanel->SetHeight(windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	mShopMainPanel->AddChild(haveItemPanel);

	CUIText* haveItemText = new CUIText();
	haveItemText->SetText(L"아이템");
	haveItemText->SetFontSize(30.0f);
	haveItemText->SetPos(SVector2D(20.0f, 20.0f));
	haveItemText->SetColor(Gdiplus::Color::White);
	haveItemText->SetOutline(2.0f, Gdiplus::Color::Black);

	haveItemText->SetWidth(haveItemText->CalculateTextSize().Width);
	haveItemText->SetHeight(haveItemText->CalculateTextSize().Height);

	haveItemPanel->AddChild(haveItemText);

	float x = 20.0f;
	float y = haveItemText->GetPos().mY + haveItemText->GetHeight() + 20.0f;
	float offset = 10.0f;

	// 플레이어 아이템 정보에서 가져옴
	for (auto& item : plItems) {
		auto currentItem = CDataMgr::GetItemDatas().find(item);

		if (currentItem == CDataMgr::GetItemDatas().end()) continue;

		UINT color = 0;

		switch(currentItem->second.tier) {
		case 2:
			color = Gdiplus::Color::SteelBlue;
			break;
		case 3:
			color = Gdiplus::Color::BlueViolet;
			break;
		case 4:
			color = Gdiplus::Color::DarkRed;
			break;
		default:
			color = 0xFF111111;
			break;
		}

		CUIPanel* itemPanel = new CUIPanel();
		itemPanel->SetWidth(75.0f);
		itemPanel->SetHeight(75.0f);
		itemPanel->SetPos(SVector2D(x, y));
		x += itemPanel->GetWidth() + offset;
		itemPanel->SetCornerRadius(10);
		itemPanel->SetBackColor(color);
		itemPanel->SetUsedClipping(true);

		haveItemPanel->AddChild(itemPanel);

		CUIImg* itemImg = new CUIImg();
		itemImg->SetTexture(CResourceMgr::Find<CTexture>(currentItem->second.name + L"Icon"));
		itemImg->SetWidth(itemPanel->GetWidth());
		itemImg->SetHeight(itemPanel->GetHeight());
		itemImg->SetIgnoreMouse(true);
		itemImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		itemPanel->AddChild(itemImg);




		CUIPanel* itemDescPanel = new CUIPanel();
		itemDescPanel->SetWidth(goodsPanel->GetWidth() / 4.0f - 30.0f);
		itemDescPanel->SetCornerRadius(10);
		itemDescPanel->InActive();
		itemDescPanel->SetBackColor(0xDD000000);

		haveItemPanel->AddChild(itemDescPanel);

		CUIPanel* itemImgPanel = new CUIPanel();
		itemImgPanel->SetWidth(75.0f);
		itemImgPanel->SetHeight(75.0f);
		itemImgPanel->SetPos(SVector2D(10.0f, 10.0f));
		itemImgPanel->SetCornerRadius(10);
		itemImgPanel->SetBackColor(color);

		itemDescPanel->AddChild(itemImgPanel);

		CUIImg* itemDescImg = new CUIImg();
		itemDescImg->SetTexture(CResourceMgr::Find<CTexture>(currentItem->second.name + L"Icon"));
		itemDescImg->SetWidth(itemImgPanel->GetWidth());
		itemDescImg->SetHeight(itemImgPanel->GetHeight());
		itemDescImg->SetIgnoreMouse(true);
		itemDescImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		itemDescPanel->AddChild(itemDescImg);

		CUIText* itemNameTex = new CUIText();

		itemNameTex->SetPos(SVector2D(itemImgPanel->GetPos().mX + itemImgPanel->GetWidth() + 10.0f, itemDescPanel->GetPos().mY));
		itemNameTex->SetWidth(100.0f);
		itemNameTex->SetHeight(25.0f);

		itemNameTex->SetText(currentItem->second.name);
		itemNameTex->SetFont(L"Noto Sans KR Medium");
		itemNameTex->SetFontSize(20.0f);
		itemNameTex->SetColor(Gdiplus::Color::White);

		itemDescPanel->AddChild(itemNameTex);

		CUIText* itemCategoryTex = new CUIText();

		itemCategoryTex->SetPos(SVector2D(itemImgPanel->GetPos().mX + itemImgPanel->GetWidth() + 10.0f, itemDescPanel->GetPos().mY + 30.0f));
		itemCategoryTex->SetWidth(100.0f);
		itemCategoryTex->SetHeight(25.0f);

		itemCategoryTex->SetText(L"아이템");
		itemCategoryTex->SetFont(L"Noto Sans KR Medium");
		itemCategoryTex->SetFontSize(15.0f);
		itemCategoryTex->SetColor(Gdiplus::Color::LightYellow);

		itemDescPanel->AddChild(itemCategoryTex);

		CUIText* itemDescTex = new CUIText();

		itemDescTex->SetPos(SVector2D(itemImgPanel->GetPos().mX, itemImgPanel->GetPos().mY + itemImgPanel->GetHeight() + 10.0f));
		itemDescTex->SetWidth(200.0f);

		itemDescTex->SetFont(L"Noto Sans KR Medium");
		itemDescTex->SetFontSize(15.0f);
		itemDescTex->SetColor(Gdiplus::Color::White);

		itemDescPanel->AddChild(itemDescTex);

		std::wstring finalDiscription = L"";

		for (auto& [effectID, args] : currentItem->second.effects) {
			auto it = CDataMgr::GetEffectDatas().find(effectID);
			if (it == CDataMgr::GetEffectDatas().end()) {
				continue;
			}

			std::wstring rawDesc = it->second.description;

			int index = 0;

			for (auto& arg : args) {
				std::wstring value = arg.value;

				std::wstring colorStr = L"#FFFFFF"; // 기본값
				if (arg.color != L"") {
					colorStr = arg.color;
				}

				if (colorStr == L"#00FF00")
				{
					try {
						int iVal = std::stoi(arg.value);

						if (iVal > 0) {
							value = L"+" + value;
						}
					}
					catch (...) {
						// 변환 실패
					}
				}

				std::wstring taggedStr = L"<c=" + colorStr + L">" + value + L"</c>";

				// 설명글 치환 ({0} -> 태그 문자열)
				// L"\\{" + 숫자 + L"\\}" 형태의 정규식 패턴 생성
				std::wstring pattern = L"\\{" + std::to_wstring(index) + L"\\}";

				try {
					// rawDesc 안에 있는 {i}를 taggedStr로 교체
					rawDesc = std::regex_replace(rawDesc, std::wregex(pattern), taggedStr);
				}
				catch (...) {
					// 정규식 에러 예외처리 (원본 유지)
				}
				index++;
			}

			finalDiscription += rawDesc + L"\n";
		}

		finalDiscription = CUIText::InsertLineBreaks(finalDiscription, itemDescPanel->GetWidth() - itemDescPanel->GetPos().mX * 2, L"Noto Sans KR Medium", 15.0f, false);

		itemDescTex->SetText(finalDiscription);
		itemDescTex->SetHeight(itemDescTex->CalculateTextSize().Height);
		itemDescPanel->SetHeight(itemImgPanel->GetPos().mY * 2 + itemImgPanel->GetHeight() + itemDescTex->GetHeight());
		itemDescPanel->SetPos(SVector2D(itemPanel->GetPos().mX, itemPanel->GetPos().mY - (10.0f + itemDescPanel->GetHeight())));

		itemPanel->SetEventHover([=]() {
			itemPanel->SetBackColor(Gdiplus::Color::White);
			itemDescPanel->Active();
			});
		itemPanel->SetEventOutHover([=]() {
			itemPanel->SetBackColor(color);
			itemDescPanel->InActive();
			});

		if (x > haveItemPanel->GetWidth()) {
			x = 20.0f;
			y = y + itemPanel->GetPos().mY + itemPanel->GetHeight() + offset;
		}
	}


	// 현재 가지고 있는 무기들을 보여주는 패널
	CUIPanel* haveWeaponPanel = new CUIPanel();
	haveWeaponPanel->SetPos(SVector2D(haveItemPanel->GetWidth(), goodsPanel->GetPos().mY + goodsPanel->GetHeight()));
	haveWeaponPanel->SetWidth(goodsPanel->GetWidth() / 3);
	haveWeaponPanel->SetHeight(windowHeight - (goodsPanel->GetHeight() + goodsPanel->GetPos().mY));

	mShopMainPanel->AddChild(haveWeaponPanel);

	CUIText* haveWeaponText = new CUIText();

	haveWeaponText->SetText(L"무기");
	haveWeaponText->SetFontSize(30.0f);
	haveWeaponText->SetPos(SVector2D(20.0f, 20.0f));
	haveWeaponText->SetColor(Gdiplus::Color::White);
	haveWeaponText->SetOutline(2.0f, Gdiplus::Color::Black);

	haveWeaponText->SetWidth(haveWeaponText->CalculateTextSize().Width);
	haveWeaponText->SetHeight(haveWeaponText->CalculateTextSize().Height);

	haveWeaponPanel->AddChild(haveWeaponText);





	x = 20.0f;
	y = haveWeaponText->GetPos().mY + haveWeaponText->GetHeight() + 20.0f;
	offset = 10.0f;


	// 가지고 있는 플레이어 무기 데이터에서 가져옴
	for (auto& weapon : plWeapons) {
		CWeaponScript* curSc = weapon->GetComponent<CWeaponScript>();
		auto currentWeapon = CDataMgr::GetWeaponDatas().find(weapon->GetID());

		if (currentWeapon == CDataMgr::GetWeaponDatas().end()) continue;

		UINT color = 0;

		switch (curSc->GetTier()) {
		case 2:
			color = Gdiplus::Color::SteelBlue;
			break;
		case 3:
			color = Gdiplus::Color::BlueViolet;
			break;
		case 4:
			color = Gdiplus::Color::DarkRed;
			break;
		default:
			color = 0xFF111111;
			break;
		}

		CUIButton* weaponButton = new CUIButton();
		weaponButton->SetWidth(75.0f);
		weaponButton->SetHeight(75.0f);
		weaponButton->SetPos(SVector2D(x, y));
		x += weaponButton->GetWidth() + offset;
		weaponButton->SetCornerRadius(10);
		weaponButton->SetBackColor(color);
		weaponButton->SetUsedClipping(true);

		mWeapons.push_back(weaponButton);

		haveWeaponPanel->AddChild(weaponButton);

		CUIImg* weaponImg = new CUIImg();
		weaponImg->SetTexture(CResourceMgr::Find<CTexture>(currentWeapon->second.iconTexture));
		weaponImg->SetWidth(weaponButton->GetWidth());
		weaponImg->SetHeight(weaponButton->GetHeight());
		weaponImg->SetIgnoreMouse(true);
		weaponImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		weaponButton->AddChild(weaponImg);




		CUIPanel* weaponDescPanel = new CUIPanel();
		weaponDescPanel->SetWidth(goodsPanel->GetWidth() / 4.0f - 30.0f);
		weaponDescPanel->SetCornerRadius(10);
		weaponDescPanel->InActive();
		weaponDescPanel->SetBackColor(0xDD000000);
		mWeaponsDescPanel.push_back(weaponDescPanel);

		haveWeaponPanel->AddChild(weaponDescPanel);

		CUIPanel* weaponImgPanel = new CUIPanel();
		weaponImgPanel->SetWidth(75.0f);
		weaponImgPanel->SetHeight(75.0f);
		weaponImgPanel->SetPos(SVector2D(10.0f, 10.0f));
		weaponImgPanel->SetCornerRadius(10);
		weaponImgPanel->SetBackColor(color);

		weaponDescPanel->AddChild(weaponImgPanel);

		CUIImg* weaponDescImg = new CUIImg();
		weaponDescImg->SetTexture(CResourceMgr::Find<CTexture>(currentWeapon->second.iconTexture));
		weaponDescImg->SetWidth(weaponImgPanel->GetWidth());
		weaponDescImg->SetHeight(weaponImgPanel->GetHeight());
		weaponDescImg->SetIgnoreMouse(true);
		weaponDescImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

		weaponImgPanel->AddChild(weaponDescImg);

		CUIText* weaponNameTex = new CUIText();

		weaponNameTex->SetPos(SVector2D(weaponImgPanel->GetPos().mX + weaponImgPanel->GetWidth() + 10.0f, weaponDescPanel->GetPos().mY));
		weaponNameTex->SetWidth(100.0f);
		weaponNameTex->SetHeight(25.0f);

		weaponNameTex->SetText(currentWeapon->second.name);
		weaponNameTex->SetFont(L"Noto Sans KR Medium");
		weaponNameTex->SetFontSize(20.0f);
		weaponNameTex->SetColor(Gdiplus::Color::White);

		weaponDescPanel->AddChild(weaponNameTex);

		CUIText* weaponCategoryTex = new CUIText();

		weaponCategoryTex->SetPos(SVector2D(weaponImgPanel->GetPos().mX + weaponImgPanel->GetWidth() + 10.0f, weaponImgPanel->GetPos().mY + 30.0f));
		weaponCategoryTex->SetWidth(100.0f);
		weaponCategoryTex->SetHeight(25.0f);

		weaponCategoryTex->SetText(currentWeapon->second.classType);
		weaponCategoryTex->SetFont(L"Noto Sans KR Medium");
		weaponCategoryTex->SetFontSize(15.0f);
		weaponCategoryTex->SetColor(Gdiplus::Color::LightYellow);

		weaponDescPanel->AddChild(weaponCategoryTex);

		CUIText* weaponDescTex = new CUIText();

		weaponDescTex->SetPos(SVector2D(weaponImgPanel->GetPos().mX, weaponImgPanel->GetPos().mY + weaponImgPanel->GetHeight() + 10.0f));
		weaponDescTex->SetWidth(200.0f);

		weaponDescTex->SetFont(L"Noto Sans KR Medium");
		weaponDescTex->SetFontSize(15.0f);
		weaponDescTex->SetColor(Gdiplus::Color::White);

		weaponDescPanel->AddChild(weaponDescTex);

		std::wstring weaponDiscription = L"";

		auto& data = currentWeapon->second.tier[curSc->GetTier() - 1];

		// 깔끔한 wstring으로 바꿔주는 도우미 람다
		auto GetCleanVal = [](float value) -> std::wstring {
			std::wstring valStr = std::to_wstring(value);
			if (valStr.find(L'.') != std::wstring::npos) {
				valStr.erase(valStr.find_last_not_of(L'0') + 1);
				if (valStr.back() == L'.') valStr.pop_back();
			}
			return valStr;
			};

		// 텍스트를 붙여주는 도우미 람다
		auto descriptionStat = [&](std::wstring label, float value, std::wstring suffix = L"") {
			if (value <= 0.0f) return;
			weaponDiscription += L"<c=#FFFFE0>" + label + L":</c> " + GetCleanVal(value) + suffix + L"\n";
			};

		descriptionStat(L"데미지", data.damage);

		if (data.critDamagePer > 0.0f) {
			weaponDiscription += L"<c=#FFFFE0>치명타:</c> x" + GetCleanVal(data.critDamagePer);

			if (data.critChancePer > 0.0f) {
				weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
			}
			weaponDiscription += L"\n";
		}

		descriptionStat(L"쿨다운", data.delay, L"s");
		descriptionStat(L"범위", data.range);

		if (data.lifeSteal != 0.0f) {
			descriptionStat(L"생명 훔침", data.lifeSteal, L"%");
		}

		weaponDescTex->SetText(weaponDiscription);
		weaponDescTex->SetHeight(weaponDescTex->CalculateTextSize().Height);



		CUIButton* combinationButton = new CUIButton();
		combinationButton->InActive();
		combinationButton->SetWidth(weaponDescPanel->GetWidth() - 20.0f);
		combinationButton->SetHeight(20.0f);
		combinationButton->SetPos(SVector2D(weaponImgPanel->GetPos().mX, weaponDescTex->GetPos().mY + weaponDescTex->GetHeight() + 10.0f));
		combinationButton->SetBackColor(Gdiplus::Color::Gray);
		combinationButton->SetCornerRadius(10);
		mCombinationButtons.push_back(combinationButton);

		weaponDescPanel->AddChild(combinationButton);

		CUIText* combinationTex = new CUIText();
		combinationTex->SetWidth(combinationButton->GetWidth());
		combinationTex->SetHeight(combinationButton->GetHeight());
		combinationTex->SetPos(SVector2D());
		combinationTex->SetIgnoreMouse(true);
		combinationTex->SetColor(Gdiplus::Color::White);
		combinationTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
		combinationTex->SetText(L"결합");

		combinationButton->AddChild(combinationTex);

		combinationButton->SetEventHover([=]() {
			combinationButton->SetBackColor(Gdiplus::Color::White);
			combinationTex->SetColor(Gdiplus::Color::Black);
			});
		combinationButton->SetEventOutHover([=]() {
			combinationButton->SetBackColor(Gdiplus::Color::Gray);
			combinationTex->SetColor(Gdiplus::Color::White);
			});

		// 결합은 같은 아이디의 같은 티어의 무기가 2개 이상이라면 활성화 시켜줌
		int count = 0;
		for (int i = 0; i < plWeapons.size(); i++) {
			if (plWeapons[i]->GetID() == weapon->GetID() && plWeapons[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) count++;

			if (count >= 2) {
				combinationButton->Active();
				count = 0;
				break;
			}
		}

		// 재활용 버튼(무기 판매)
		CUIButton* recycleButton = new CUIButton();
		recycleButton->SetWidth(weaponDescPanel->GetWidth() - 20.0f);
		recycleButton->SetHeight(20.0f);
		recycleButton->SetPos(SVector2D(combinationButton->GetPos().mX, combinationButton->GetPos().mY + combinationButton->GetHeight() + 10.0f));
		recycleButton->SetBackColor(Gdiplus::Color::Gray);
		recycleButton->SetCornerRadius(10);

		weaponDescPanel->AddChild(recycleButton);

		int recycleCost = data.basePrice / 3.0f;
		
		CUIText* recycleTex = new CUIText();
		recycleTex->SetWidth(recycleButton->GetWidth());
		recycleTex->SetHeight(recycleButton->GetHeight());
		recycleTex->SetPos(SVector2D());
		recycleTex->SetIgnoreMouse(true);
		recycleTex->SetColor(Gdiplus::Color::White);
		recycleTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
		recycleTex->SetText(L"재활용 (+" + std::to_wstring(recycleCost) + L")");

		recycleButton->AddChild(recycleTex);

		recycleButton->SetEventHover([=]() {
			recycleButton->SetBackColor(Gdiplus::Color::White);
			recycleTex->SetColor(Gdiplus::Color::Black);
			});
		recycleButton->SetEventOutHover([=]() {
			recycleButton->SetBackColor(Gdiplus::Color::Gray);
			recycleTex->SetColor(Gdiplus::Color::White);
			});
		// 재활용 버튼 클릭 시 무기를 현재 지니고 있는 무기에서 삭제하는게 필요함
		recycleButton->SetEventClick([=]() {
			plSc->ChangeMoney(recycleCost);
			weaponDescPanel->InActive();

			for (int i = 0; i < plWeapons.size(); i++) {
				if (plWeapons[i]->GetID() == weapon->GetID() && plWeapons[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) {
					pl->GetComponent<CWeaponMgr>()->RemoveWeapon(i);

					if (mWeapons[i] != nullptr) {
						haveWeaponPanel->RemoveChild(mWeapons[i]);
						mWeapons[i]->OnDestroy();
						SAFE_DELETE(mWeapons[i]);
					}

					mWeapons.erase(mWeapons.begin() + i);
					mWeaponsDescPanel.erase(mWeaponsDescPanel.begin() + i);
					mCombinationButtons.erase(mCombinationButtons.begin() + i);
					break;
				}
			}

			if (count < 2) {
				combinationButton->InActive();
			}
			WeaponButtonsReSetting(20.0f, haveWeaponText->GetPos().mY + haveWeaponText->GetHeight() + 20.0f, 10.0f, haveItemPanel->GetWidth());
			});

		// 결합 버튼 또한 재활용 버튼과 마찬가지, 다른 점은 결합이 클릭된 무기를 업그레이드 시켜야 함.
		combinationButton->SetEventClick([=]() {
			curSc->SetTier(curSc->GetTier() + 1);
			for (int i = 0; i < plWeapons.size(); i++) {
				if (plWeapons[i]->GetID() == weapon->GetID() && plWeapons[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier() - 1) {
					pl->GetComponent<CWeaponMgr>()->RemoveWeapon(i);

					if (mWeapons[i] != nullptr) {
						haveWeaponPanel->RemoveChild(mWeapons[i]);
						mWeapons[i]->OnDestroy();
						SAFE_DELETE(mWeapons[i]);
					}

					mWeapons.erase(mWeapons.begin() + i);
					mWeaponsDescPanel.erase(mWeaponsDescPanel.begin() + i);
					mCombinationButtons.erase(mCombinationButtons.begin() + i);
					break;
				}
			}
			ReSettingWeaponButton(curSc, weapon, haveWeaponPanel, weaponButton, weaponDescTex, weaponImgPanel, recycleButton, recycleTex);
			WeaponButtonsReSetting(20.0f, haveWeaponText->GetPos().mY + haveWeaponText->GetHeight() + 20.0f, 10.0f, haveItemPanel->GetWidth());
			weaponDescPanel->InActive();
			});



		// 무기 버튼이 선택되어도 아무것도 안하고 넘길 수 있게 취소 버튼
		CUIButton* cancleButton = new CUIButton();
		cancleButton->SetWidth(weaponDescPanel->GetWidth() - 20.0f);
		cancleButton->SetHeight(20.0f);
		cancleButton->SetPos(SVector2D(recycleButton->GetPos().mX, recycleButton->GetPos().mY + recycleButton->GetHeight() + 10.0f));
		cancleButton->SetBackColor(Gdiplus::Color::Gray);
		cancleButton->SetCornerRadius(10);

		weaponDescPanel->AddChild(cancleButton);

		CUIText* cancleTex = new CUIText();
		cancleTex->SetWidth(cancleButton->GetWidth());
		cancleTex->SetHeight(cancleButton->GetHeight());
		cancleTex->SetPos(SVector2D());
		cancleTex->SetIgnoreMouse(true);
		cancleTex->SetColor(Gdiplus::Color::White);
		cancleTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);
		cancleTex->SetText(L"취소");

		cancleButton->AddChild(cancleTex);

		cancleButton->SetEventHover([=]() {
			cancleButton->SetBackColor(Gdiplus::Color::White);
			cancleTex->SetColor(Gdiplus::Color::Black);
			});
		cancleButton->SetEventOutHover([=]() {
			cancleButton->SetBackColor(Gdiplus::Color::Gray);
			cancleTex->SetColor(Gdiplus::Color::White);
			});
		cancleButton->SetEventClick([=]() {
			weaponButton->SetSelected(false);
			weaponDescPanel->InActive();
			});
	



		weaponDescPanel->SetHeight(cancleButton->GetPos().mY + cancleButton->GetHeight() + 10.0f);
		weaponDescPanel->SetPos(SVector2D(weaponButton->GetPos().mX + weaponButton->GetWidth() - weaponDescPanel->GetWidth(), weaponButton->GetPos().mY - (10.0f + weaponDescPanel->GetHeight())));

		weaponButton->SetEventHover([=]() {
			weaponButton->SetBackColor(Gdiplus::Color::White);
			weaponDescPanel->Active();
			});
		weaponButton->SetEventOutHover([=]() {
			weaponButton->SetBackColor(color);
			if (!weaponButton->IsSelected()) {
				weaponDescPanel->InActive();
			}
			});
		weaponButton->SetEventClick([=]() {
			weaponButton->SetSelected(true);
			});

		if (x > haveItemPanel->GetWidth()) {
			x = 20.0f;
			y = y + weaponButton->GetPos().mY + weaponButton->GetHeight() + offset;
		}
	}




	// 사이드에서 현재 플레이어의 스탯을 보여주기 위한 패널
	CUIPanel* shopSidePanel = new CUIPanel();
	shopSidePanel->SetWidth(this->GetWidth() - mShopMainPanel->GetWidth());
	shopSidePanel->SetHeight(this->GetHeight());
	shopSidePanel->SetPos(SVector2D(mShopMainPanel->GetWidth(), 0.0f));

	this->AddChild(shopSidePanel);

	CUIPanel* statPanel = new CUIPanel();
	statPanel->SetWidth(shopSidePanel->GetWidth() - 40.0f);
	statPanel->SetHeight(3 * shopSidePanel->GetHeight() / 4);
	statPanel->SetPos(SVector2D(20.0f, 20.0f));
	statPanel->SetBackColor(Gdiplus::Color::Gray);
	statPanel->SetCornerRadius(10);

	shopSidePanel->AddChild(statPanel);

	CUIText* statTex = new CUIText();
	statTex->SetText(L"능력치");
	statTex->SetFontSize(30.0f);
	statTex->SetStrokeWidth(1.0f);
	statTex->SetOutline(2.0f, Gdiplus::Color::Black);
	statTex->SetWidth(statTex->CalculateTextSize().Width);
	statTex->SetHeight(statTex->CalculateTextSize().Height);
	statTex->SetPos(SVector2D(statPanel->GetWidth() / 2 - statTex->GetWidth() / 2, 5.0f));
	statTex->SetColor(Gdiplus::Color::White);

	statPanel->AddChild(statTex);

	CUIPanel* statLevel = MakeStat(statPanel->GetWidth(), 30.0f, L"UpgradeIcon", L"현재 레벨", mLevel);
	statLevel->SetPos(SVector2D(20.0f, statTex->GetHeight() + statTex->GetPos().mY + 20.0f));
	statPanel->AddChild(statLevel);

	float statPosY = (statPanel->GetHeight() - 30.0f - (statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f)) / 12;

	CUIPanel* statMaxHP = MakeStat(statPanel->GetWidth(), 30.0f, L"MaxHP", L"최대 체력", mMaxHP);
	statMaxHP->SetPos(SVector2D(20.0f, statLevel->GetHeight() + statLevel->GetPos().mY + 50.0f));
	statPanel->AddChild(statMaxHP);

	CUIPanel* statHPRegen = MakeStat(statPanel->GetWidth(), 30.0f, L"HPRegen", L"HP 재생", mHPRegen);
	statHPRegen->SetPos(SVector2D(20.0f, statMaxHP->GetPos().mY + statPosY));
	statPanel->AddChild(statHPRegen);

	CUIPanel* statHPSteal = MakeStat(statPanel->GetWidth(), 30.0f, L"LifeSteal", L"% 생명 훔침", mHPSteal);
	statHPSteal->SetPos(SVector2D(20.0f, statHPRegen->GetPos().mY + statPosY));
	statPanel->AddChild(statHPSteal);




	CUIPanel* statDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"Damage", L"% 데미지", mDamage);
	statDamage->SetPos(SVector2D(20.0f, statHPSteal->GetPos().mY + statPosY));
	statPanel->AddChild(statDamage);

	CUIPanel* statMeleeDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"MeleeDamage", L"근거리 데미지", mMeleeDamage);
	statMeleeDamage->SetPos(SVector2D(20.0f, statDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statMeleeDamage);

	CUIPanel* statRangedDamage = MakeStat(statPanel->GetWidth(), 30.0f, L"RangedDamage", L"원거리 데미지", mRangedDamage);
	statRangedDamage->SetPos(SVector2D(20.0f, statMeleeDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statRangedDamage);

	CUIPanel* statAttackSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"AttackSpeed", L"% 공격 속도", mAttackSpeed);
	statAttackSpeed->SetPos(SVector2D(20.0f, statRangedDamage->GetPos().mY + statPosY));
	statPanel->AddChild(statAttackSpeed);

	CUIPanel* statCritChance = MakeStat(statPanel->GetWidth(), 30.0f, L"CritChance", L"% 치명타율", mCritChance);
	statCritChance->SetPos(SVector2D(20.0f, statAttackSpeed->GetPos().mY + statPosY));
	statPanel->AddChild(statCritChance);

	CUIPanel* statRange = MakeStat(statPanel->GetWidth(), 30.0f, L"Range", L"범위", mRange);
	statRange->SetPos(SVector2D(20.0f, statCritChance->GetPos().mY + statPosY));
	statPanel->AddChild(statRange);

	CUIPanel* statArmor = MakeStat(statPanel->GetWidth(), 30.0f, L"Armor", L"방어구", mArmor);
	statArmor->SetPos(SVector2D(20.0f, statRange->GetPos().mY + statPosY));
	statPanel->AddChild(statArmor);

	CUIPanel* statDodge = MakeStat(statPanel->GetWidth(), 30.0f, L"Dodge", L"% 회피", mDodge);
	statDodge->SetPos(SVector2D(20.0f, statArmor->GetPos().mY + statPosY));
	statPanel->AddChild(statDodge);

	CUIPanel* statSpeed = MakeStat(statPanel->GetWidth(), 30.0f, L"Speed", L"% 속도", mSpeed);
	statSpeed->SetPos(SVector2D(20.0f, statDodge->GetPos().mY + statPosY));
	statPanel->AddChild(statSpeed);


	// 다음 스테이지로 이동하는 이동 버튼
	CUIButton* nextStageButton = new CUIButton();
	nextStageButton->SetWidth(statPanel->GetWidth());
	nextStageButton->SetHeight(50.0f);
	nextStageButton->SetCornerRadius(10);
	nextStageButton->SetBackColor(Gdiplus::Color::Black);
	nextStageButton->SetPos(SVector2D(20.0f, shopSidePanel->GetHeight() - nextStageButton->GetHeight() - 10.0f));

	shopSidePanel->AddChild(nextStageButton);

	CUIText* nextTex = new CUIText();
	nextTex->SetText(L"이동");
	nextTex->SetFontSize(35.0f);
	nextTex->SetWidth(nextStageButton->GetWidth());
	nextTex->SetHeight(nextStageButton->GetHeight());
	nextTex->SetColor(Gdiplus::Color::White);
	nextTex->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	nextStageButton->AddChild(nextTex);

	nextStageButton->SetEventHover([=]() {
		nextTex->SetColor(Gdiplus::Color::Black);
		nextStageButton->SetBackColor(Gdiplus::Color::White);
		});
	nextStageButton->SetEventOutHover([=]() {
		nextTex->SetColor(Gdiplus::Color::White);
		nextStageButton->SetBackColor(Gdiplus::Color::Black);
		});
	nextStageButton->SetEventClick([=]() {
		// 스테이지 넘어가는 것 구현
		});

	CUIBase::OnCreate();
}

void CShopUI::Active()
{
	CUIBase::Active();
}

void CShopUI::InActive()
{
	CUIBase::InActive();
}

void CShopUI::OnDestroy()
{
	CUIBase::OnDestroy();
}

void CShopUI::OnUpdate(float tDeltaTime)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	mMoneyTex->SetText(std::to_wstring(plSc->GetMoney()));

	mResetTex->SetText(L"초기화 -" + std::to_wstring(mResetCost));
	mResetTex->SetWidth(mResetTex->CalculateTextSize().Width);

	mResetImg->SetPos(SVector2D(mResetTex->GetWidth() + 5.0f, 5.0f));

	mResetButton->SetWidth(mResetTex->GetWidth() + 60.0f);
	mResetButton->SetPos(SVector2D(mShopMainPanel->GetWidth() - mResetButton->GetWidth() - 20.0f, 20.0f));

	mLevel->SetColor(Gdiplus::Color::White);
	mLevel->SetText(std::to_wstring(plSc->GetLevel()));

	SettingStatTex(plSc->GetMaxHP(), mMaxHP);
	SettingStatTex(plSc->GetHPGeneration(), mHPRegen);
	SettingStatTex(plSc->GetLifeSteal(), mHPSteal);
	SettingStatTex(plSc->GetDamagePercent(), mDamage);
	SettingStatTex(plSc->GetMeleeDamage(), mMeleeDamage);
	SettingStatTex(plSc->GetRangedDamage(), mRangedDamage);
	SettingStatTex(plSc->GetAttackSpeedPercent(), mAttackSpeed);
	SettingStatTex(plSc->GetCriticalChancePercent(), mCritChance);
	SettingStatTex(plSc->GetRange(), mRange);
	SettingStatTex(plSc->GetArmor(), mArmor);
	SettingStatTex(plSc->GetDodge(), mDodge);
	SettingStatTex(plSc->GetSpeedPercent(), mSpeed);

	CUIBase::OnUpdate(tDeltaTime);
}

void CShopUI::OnLateUpdate(float tDeltaTime)
{
	CUIBase::OnLateUpdate(tDeltaTime);
}

void CShopUI::Render(HDC tHDC)
{
	CUIBase::Render(tHDC);
}

void CShopUI::UIClear()
{
	CUIBase::UIClear();
}

CUIPanel* CShopUI::MakeGoods(float tWidth, float tHeight)
{
	int rand = std::rand() % 2;
	// 아이템과 무기 중 랜덤하게

	CUIPanel* goodsPanel = new CUIPanel();
	goodsPanel->SetPos(SVector2D(5.0f, 5.0f));
	goodsPanel->SetWidth(tWidth);
	goodsPanel->SetHeight(tHeight);
	goodsPanel->SetBackColor(Gdiplus::Color::Black);
	goodsPanel->SetCornerRadius(10);

	CUIPanel* goodsImgPanel = new CUIPanel();

	goodsImgPanel->SetWidth(75.0f);
	goodsImgPanel->SetHeight(75.0f);
	goodsImgPanel->SetPos(SVector2D(10.0f, 10.0f));
	goodsImgPanel->SetBackColor(Gdiplus::Color::LightGray);
	goodsImgPanel->SetCornerRadius(10.0f);

	goodsPanel->AddChild(goodsImgPanel);

	CUIImg* goodsImg = new CUIImg();

	goodsImg->SetImageMode(CUIImg::eImageMode::KeepAspect);
	goodsImg->SetWidth(goodsImgPanel->GetWidth());
	goodsImg->SetHeight(goodsImgPanel->GetHeight());

	goodsImgPanel->AddChild(goodsImg);

	CUIText* goodsNameTex = new CUIText();

	goodsNameTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY));
	goodsNameTex->SetWidth(100.0f);
	goodsNameTex->SetHeight(25.0f);

	goodsNameTex->SetText(L"Test");
	goodsNameTex->SetFont(L"Noto Sans KR Medium");
	goodsNameTex->SetFontSize(20.0f);
	goodsNameTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(goodsNameTex);

	CUIText* goodsTex = new CUIText();

	goodsTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX + goodsImgPanel->GetWidth() + 10.0f, goodsImgPanel->GetPos().mY + 30.0f));
	goodsTex->SetWidth(100.0f);
	goodsTex->SetHeight(25.0f);

	goodsTex->SetText(L"Test");
	goodsTex->SetFont(L"Noto Sans KR Medium");
	goodsTex->SetFontSize(15.0f);
	goodsTex->SetColor(Gdiplus::Color::LightYellow);

	goodsPanel->AddChild(goodsTex);


	CUIText* descriptionGoodsTex = new CUIText();

	descriptionGoodsTex->SetPos(SVector2D(goodsImgPanel->GetPos().mX, goodsImgPanel->GetPos().mY + goodsImgPanel->GetHeight() + 10.0f));
	descriptionGoodsTex->SetWidth(200.0f);
	descriptionGoodsTex->SetHeight(250.0f);

	descriptionGoodsTex->SetFont(L"Noto Sans KR Medium");
	descriptionGoodsTex->SetFontSize(15.0f);
	descriptionGoodsTex->SetColor(Gdiplus::Color::White);

	goodsPanel->AddChild(descriptionGoodsTex);

	// 각 아이템 or 무기 가져오기
	if (rand == 0) {

	}
	else {

	}

	CUIButton* buyButton = new CUIButton();

	CUIText* buyCost = new CUIText();
	buyCost->SetText(std::to_wstring(10));
	buyCost->SetWidth(buyCost->CalculateTextSize().Width + 10.0f);
	buyCost->SetPos(SVector2D(5.0f, -5.0f));
	buyCost->SetHeight(buyCost->CalculateTextSize().Height + 20.0f);
	buyCost->SetFontSize(30.0f);
	buyCost->SetColor(Gdiplus::Color::White);
	buyCost->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	buyButton->AddChild(buyCost);

	CUIImg* buyImg = new CUIImg();
	buyImg->SetTexture(CResourceMgr::Find<CTexture>(L"HarvestIcon"));
	buyImg->SetPos(SVector2D(buyCost->GetWidth() + 5.0f, 0.0f));
	buyImg->SetWidth(40.0f);
	buyImg->SetHeight(buyCost->GetHeight());
	buyImg->SetImageMode(CUIImg::eImageMode::KeepAspect);

	buyButton->AddChild(buyImg);

	buyButton->SetWidth(buyCost->GetWidth() + buyImg->GetWidth() + 10.0f);
	buyButton->SetHeight(buyCost->GetHeight());
	buyButton->SetPos(SVector2D(goodsPanel->GetWidth() / 2 - buyButton->GetWidth() / 2, goodsPanel->GetHeight() - buyButton->GetHeight() - 20.0f));
	buyButton->SetBackColor(Gdiplus::Color::Gray);
	buyButton->SetCornerRadius(10);

	goodsPanel->AddChild(buyButton);

	buyButton->SetEventHover([=]() {
		buyButton->SetBackColor(Gdiplus::Color::White);
		buyCost->SetColor(Gdiplus::Color::Black);
		});
	buyButton->SetEventOutHover([=]() {
		buyButton->SetBackColor(Gdiplus::Color::Gray);
		buyCost->SetColor(Gdiplus::Color::White);
		});
	buyButton->SetEventClick([=]() {
		// 아이템 능력 구현
		});


	CUIButton* rockButton = new CUIButton();
	rockButton->SetWidth(60.0f);
	rockButton->SetHeight(40.0f);
	rockButton->SetPos(SVector2D(goodsPanel->GetWidth() / 2 - rockButton->GetWidth() / 2, goodsPanel->GetHeight() + 30.0f));
	rockButton->SetBackColor(Gdiplus::Color::Black);
	rockButton->SetCornerRadius(10);

	goodsPanel->AddChild(rockButton);

	CUIText* rockText = new CUIText();
	rockText->SetText(L"잠금");
	rockText->SetWidth(60.0f);
	rockText->SetHeight(40.0f);
	rockText->SetColor(Gdiplus::Color::White);
	rockText->SetStrokeWidth(1.0f);
	rockText->SetAlign(Gdiplus::StringAlignmentCenter, Gdiplus::StringAlignmentCenter);

	rockButton->AddChild(rockText);

	rockButton->SetEventHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::White);
		rockText->SetColor(Gdiplus::Color::Black);
		});
	rockButton->SetEventOutHover([=]() {
		rockButton->SetBackColor(Gdiplus::Color::Black);
		rockText->SetColor(Gdiplus::Color::White);
		});
	rockButton->SetEventClick([=]() {
		
		// 잠금 구현
		});


	return goodsPanel;
}

// 결합이 클릭된 버튼의 수정 사항들을 보이기 위한 함수
void CShopUI::ReSettingWeaponButton(CWeaponScript* tWpScript, CWeapon* tCurWp, CUIPanel* tParPanel, CUIButton* tWpButton, CUIText* tDescTex, CUIPanel* tWpImgPanel, CUIButton* tRecycleButton, CUIText* tRecycleTex)
{
	// 결합 시 수정될 부분들만 수정하기 위한 함수
	CPlayer* pl = CPlayScene::GetPlayer();
	CPlayerScript* plSc = pl->GetComponent<CPlayerScript>();
	std::vector<CWeapon*> plWeapons = pl->GetComponent<CWeaponMgr>()->GetWeapons();

	auto currentWeapon = CDataMgr::GetWeaponDatas().find(tCurWp->GetID());

	if (currentWeapon == CDataMgr::GetWeaponDatas().end()) return;

	UINT color = 0;

	switch (tWpScript->GetTier()) {
	case 2:
		color = Gdiplus::Color::SteelBlue;
		break;
	case 3:
		color = Gdiplus::Color::BlueViolet;
		break;
	case 4:
		color = Gdiplus::Color::DarkRed;
		break;
	default:
		color = 0xFF111111;
		break;
	}


	tWpButton->SetBackColor(color);
	tWpImgPanel->SetBackColor(color);

	std::wstring weaponDiscription = L"";

	auto& data = currentWeapon->second.tier[tWpScript->GetTier() - 1];

	// 깔끔한 wstring으로 바꿔주는 도우미 람다
	auto GetCleanVal = [](float value) -> std::wstring {
		std::wstring valStr = std::to_wstring(value);
		if (valStr.find(L'.') != std::wstring::npos) {
			valStr.erase(valStr.find_last_not_of(L'0') + 1);
			if (valStr.back() == L'.') valStr.pop_back();
		}
		return valStr;
		};

	// 텍스트를 붙여주는 도우미 람다
	auto descriptionStat = [&](std::wstring label, float value, std::wstring suffix = L"") {
		if (value <= 0.0f) return;
		weaponDiscription += L"<c=#FFFFE0>" + label + L":</c> " + GetCleanVal(value) + suffix + L"\n";
		};

	descriptionStat(L"데미지", data.damage);

	if (data.critDamagePer > 0.0f) {
		weaponDiscription += L"<c=#FFFFE0>치명타:</c> x" + GetCleanVal(data.critDamagePer);

		if (data.critChancePer > 0.0f) {
			weaponDiscription += L" (" + GetCleanVal(data.critChancePer) + L"% 확률)";
		}
		weaponDiscription += L"\n";
	}

	descriptionStat(L"쿨다운", data.delay, L"s");
	descriptionStat(L"범위", data.range);

	if (data.lifeSteal != 0.0f) {
		descriptionStat(L"생명 훔침", data.lifeSteal, L"%");
	}

	tDescTex->SetText(weaponDiscription);

	int recycleCost = data.basePrice / 3.0f;

	tRecycleButton->SetEventClick([=]() {
		plSc->ChangeMoney(recycleCost);
		for (int i = 0; i < plWeapons.size(); i++) {
			if (plWeapons[i]->GetID() == tCurWp->GetID() && plWeapons[i]->GetComponent<CWeaponScript>()->GetTier() == tWpScript->GetTier() - 1) {
				pl->GetComponent<CWeaponMgr>()->RemoveWeapon(i);

				if (mWeapons[i] != nullptr) {
					tParPanel->RemoveChild(mWeapons[i]);
					mWeapons[i]->OnDestroy();
				}

				mWeapons.erase(mWeapons.begin() + i);
				break;
			}
		}
		});

	tRecycleTex->SetText(L"재활용 (+" + std::to_wstring(recycleCost) + L")");
}

// 결합이나 재결합이 클릭되었을 때 모든 무기 버튼들의 수정 사항 (위치, 설명 패널 위치, 결합 버튼 활성화 여부)
void CShopUI::WeaponButtonsReSetting(float tX, float tY, float tOffset, float tMax)
{
	CPlayer* pl = CPlayScene::GetPlayer();
	CPlayerScript* plSc = pl->GetComponent<CPlayerScript>();
	std::vector<CWeapon*> plWeapons = pl->GetComponent<CWeaponMgr>()->GetWeapons();

	for (int i = 0; i < mWeapons.size(); i++) {
		mWeapons[i]->SetPos(SVector2D(tX, tY));
		tX += mWeapons[i]->GetWidth() + tOffset;

		mWeaponsDescPanel[i]->SetPos(SVector2D(mWeapons[i]->GetPos().mX + mWeapons[i]->GetWidth() - mWeaponsDescPanel[i]->GetWidth(), mWeapons[i]->GetPos().mY - (10.0f + mWeaponsDescPanel[i]->GetHeight())));
		
		if (tX > tMax) {
			tX = 20.0f;
			tY = tY + mWeapons[i]->GetPos().mY + mWeapons[i]->GetHeight() + tOffset;
		}
	}

	int index = 0;

	for (auto button : mCombinationButtons) {
		button->InActive();
		auto weapon = plWeapons[index];
		CWeaponScript* curSc = weapon->GetComponent<CWeaponScript>();

		int count = 0;
		for (int i = 0; i < plWeapons.size(); i++) {
			if (plWeapons[i]->GetID() == weapon->GetID() && plWeapons[i]->GetComponent<CWeaponScript>()->GetTier() == curSc->GetTier()) count++;

			if (count >= 2) {
				button->Active();
				count = 0;
				break;
			}
		}
		index++;
	}
}
