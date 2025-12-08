#include "CEffectMgr.h"

#include "CDamageText.h"
#include "CEnemyHit.h"

std::unordered_map<std::wstring, std::vector<CEffect*>> CEffectMgr::mEffects;
std::vector<CDamageText*> CEffectMgr::mDamageTexts;

void CEffectMgr::OnCreate() {
	mEffects.insert({ L"EnemyHit", std::vector<CEffect*>() });

	for (int i = 0; i < 100; i++) {
		mEffects[L"EnemyHit"].push_back(Instantiate<CEnemyHit>(eLayerType::Effect));
		mEffects[L"EnemyHit"][i]->OnCreate();
	}

	for (int i = 0; i < 100; i++) {
		mDamageTexts.push_back(Instantiate<CDamageText>(eLayerType::DamageText));
		mDamageTexts[i]->OnCreate();
	}
}

void CEffectMgr::OnDestroy() {
	mEffects.clear();
	mDamageTexts.clear();
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

void CEffectMgr::ShowDamageText(SVector2D tPos, int tDamage, Gdiplus::Color tColor)
{
	for (auto& it : mDamageTexts) {
		if (!it->IsActive()) {
			it->Reset(tPos, tDamage, tColor);
			return;
		}
	}

	CDamageText* newText = Instantiate<CDamageText>(eLayerType::DamageText);

	newText->Reset(tPos, tDamage, tColor);

	mDamageTexts.push_back(newText);
}
