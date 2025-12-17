#include "CEffectMgr.h"

#include "CEffectText.h"
#include "CEnemyHit.h"

std::unordered_map<std::wstring, std::vector<CEffect*>> CEffectMgr::mEffects;
std::vector<CEffectText*> CEffectMgr::mEffectTexts;

void CEffectMgr::OnCreate() {
	mEffects.insert({ L"EnemyHit", std::vector<CEffect*>() });

	for (int i = 0; i < 100; i++) {
		mEffects[L"EnemyHit"].push_back(Instantiate<CEnemyHit>(eLayerType::Effect));
		mEffects[L"EnemyHit"][i]->OnCreate();
	}

	for (int i = 0; i < 100; i++) {
		mEffectTexts.push_back(Instantiate<CEffectText>(eLayerType::EffectText));
		mEffectTexts[i]->OnCreate();
	}
}

void CEffectMgr::OnDestroy() {
	mEffects.clear();
	mEffectTexts.clear();
}

void CEffectMgr::OnUpdate(float tDeltaTime) {

}

void CEffectMgr::OnLateUpdate(float tDeltaTime) {

}

void CEffectMgr::Render(HDC tHDC) {

}

void CEffectMgr::PlayEffect(std::wstring tName, SVector2D tPos, GameObject* tObj)
{
	std::vector<CEffect*>& pool = mEffects[tName];

	CEffect* target = nullptr;
	for (CEffect* effect : pool) {
		if (!effect->IsActive()) {
			target = effect;
			target->Reset(tPos);
			target->SetOwner(tObj);
			break;
		}
	}

	if (target == nullptr) {
		if (tName == L"EnemyHit") {
			CEnemyHit* newEnemyHit = Instantiate<CEnemyHit>(eLayerType::Effect);

			newEnemyHit->Reset(tPos);
			newEnemyHit->SetOwner(tObj);

			mEffects[tName].push_back(newEnemyHit);
		}
	}
}

void CEffectMgr::ShowEffectText(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor)
{
	for (auto& it : mEffectTexts) {
		if (!it->IsActive()) {
			it->Reset(tPos, tText, tColor);
			return;
		}
	}

	CEffectText* newText = Instantiate<CEffectText>(eLayerType::EffectText);

	newText->Reset(tPos, tText, tColor);

	mEffectTexts.push_back(newText);
}
