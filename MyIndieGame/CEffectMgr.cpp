#include "CEffectMgr.h"

#include "CEffectText.h"
#include "CEnemyHit.h"

std::unordered_map<std::wstring, std::vector<CEffect*>> CEffectMgr::mEffects;
std::vector<CEffectText*> CEffectMgr::mEffectTexts;

void CEffectMgr::OnCreate() {
	// 게임 시작 시 이펙트 객체들을 미리 메모리에 할당해둡니다.
	// 런타임 중에 발생할 수 있는 프레임 드랍(렉)을 방지하기 위함입니다.
	mEffects.insert({ L"EnemyHit", std::vector<CEffect*>() });

	// 타격 이펙트 100개 미리 생성
	for (int i = 0; i < 100; i++) {
		mEffects[L"EnemyHit"].push_back(Instantiate<CEnemyHit>(eLayerType::Effect));
		mEffects[L"EnemyHit"][i]->OnCreate();
	}

	// 데미지 텍스트 100개 미리 생성
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

// 이펙트 재생 요청 처리
void CEffectMgr::PlayEffect(std::wstring tName, SVector2D tPos, GameObject* tObj)
{
	std::vector<CEffect*>& pool = mEffects[tName];

	CEffect* target = nullptr;

	// 1. [Search] 풀(Pool)을 순회하며 '쉬고 있는(Inactive)' 객체를 찾습니다.
	for (CEffect* effect : pool) {
		if (!effect->IsActive()) {	// 비활성화 상태 확인
			target = effect;

			// 2. [Reuse] 찾은 객체를 초기화(Reset)하여 재사용합니다.
			// Reset 함수 내부에서 위치 설정, 타이머 초기화, SetState(true) 등이 호출될 것입니다.
			target->Reset(tPos);
			target->SetOwner(tObj);
			break;
		}
	}

	// 3. [Dynamic Expansion] 만약 미리 만들어둔 100개를 다 쓰고 있다면?
	if (target == nullptr) {
		// 풀이 고갈되었을 때, 새로 생성하여 풀을 확장합니다. (Safety Net)
		if (tName == L"EnemyHit") {
			CEnemyHit* newEnemyHit = Instantiate<CEnemyHit>(eLayerType::Effect);

			newEnemyHit->Reset(tPos);
			newEnemyHit->SetOwner(tObj);

			mEffects[tName].push_back(newEnemyHit);
		}
	}
}

// 데미지 텍스트(Floating Text) 전용 재생 함수
void CEffectMgr::ShowEffectText(SVector2D tPos, std::wstring tText, Gdiplus::Color tColor)
{
	// 1. 비활성화된 텍스트 객체 탐색
	for (auto& it : mEffectTexts) {
		if (!it->IsActive()) {
			// 2. 재사용 (위치, 내용, 색상 초기화)
			it->Reset(tPos, tText, tColor);
			return;
		}
	}

	// 3. 풀 고갈 시 동적 생성 및 확장
	CEffectText* newText = Instantiate<CEffectText>(eLayerType::EffectText);

	newText->Reset(tPos, tText, tColor);

	mEffectTexts.push_back(newText);
}
