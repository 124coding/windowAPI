#include "CWeaponMgr.h"

#include "Object.h"

#include "CDataMgr.h"

#include "CWeaponScript.h"
#include "CRangedWeaponScript.h"

#include "CCircleCollider2D.h"
#include "CBoxCollider2D.h"

void CWeaponMgr::OnCreate()
{
}

void CWeaponMgr::OnDestroy()
{
}

void CWeaponMgr::OnUpdate(float tDeltaTime)
{
}

void CWeaponMgr::OnLateUpdate(float tDeltaTime)
{
}

void CWeaponMgr::Render(HDC tHDC)
{
}

std::pair<int, CWeapon*> CWeaponMgr::PlusWeapon(eLayerType tType, std::wstring tWeaponId, int tWeaponTier) {

	// 1. [Auto-Merge Logic] 인벤토리가 가득 찼을 때의 처리
	if (mWeapons.size() >= mWeaponCount) {
		// 최고 티어(4)라면 더 이상 합칠 수 없으므로 획득 불가
		if (tWeaponTier == 4) {
			return std::make_pair(-1, nullptr);
		}

		// 기존 무기 중 '같은 ID'이고 '같은 티어'인 무기가 있는지 탐색
		for (int i = 0; i < mWeapons.size(); i++) {
			CWeaponScript* wpSc = mWeapons[i]->GetComponent<CWeaponScript>(eComponentType::Script);
			if (mWeapons[i]->GetID() == tWeaponId && wpSc->GetTier() == tWeaponTier) {
				// [Upgrade] 같은 무기를 찾으면 티어를 +1 상승시키고 스탯 갱신
				auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
				if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
					return std::make_pair(i, mWeapons[i]);
				}

				CDataMgr::SWeapon currentWeapon = weaponIter->second;

				// 다음 티어의 스탯으로 덮어쓰기 (Damage, Delay, Range 등)
				wpSc->SetDamage(currentWeapon.tier[tWeaponTier].damage);
				wpSc->SetDelay(currentWeapon.tier[tWeaponTier].delay);
				wpSc->SetCriticalDamage(currentWeapon.tier[tWeaponTier].critDamagePer);
				wpSc->SetCriticalChance(currentWeapon.tier[tWeaponTier].critChancePer);
				wpSc->SetRange(currentWeapon.tier[tWeaponTier].range);
				wpSc->SetLifeSteal(currentWeapon.tier[tWeaponTier].lifeSteal);
				wpSc->SetBasePrice(currentWeapon.tier[tWeaponTier].basePrice);
				wpSc->SetSpeed(currentWeapon.speed);
				wpSc->SetTier(tWeaponTier + 1);

				return std::make_pair(i, mWeapons[i]);	// 업데이트된 무기 반환
			}
		}
		// 합칠 무기도 없고 자리도 없으면 획득 실패
		return std::make_pair(-1, nullptr);
	}

	// 2. [New Weapon Creation] 자리가 있을 때 새 무기 생성
	// Data Manager에서 무기 기본 정보 조회
	auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
	if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
		return std::make_pair(-1, nullptr);
	}

	CDataMgr::SWeapon currentWeapon = weaponIter->second;

	// [Factory Pattern Usage] ID에 맞는 무기 객체 생성
	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return std::make_pair(-1, nullptr);
	}
	
	CWeapon* weapon = iter->second();
	weapon->SetID(tWeaponId);

	// 렌더링 및 물리 컴포넌트 설정
	CTransform* wpTr = weapon->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(currentWeapon.name);
	weapon->SetSize(SVector2D(currentWeapon.sizeX, currentWeapon.sizeY));
	wpSr->SetTexture(wpImg);

	// 무기가 캐릭터 주변을 돌 때, 무기의 손잡이 부분이 중심을 향하도록 Anchor 조정
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 20.0f, wpImg->GetHeight() / 2);

	weapon->OnCreate();

	// 스크립트 스탯 설정 (티어에 맞는 수치 적용)
	CWeaponScript* wpSc = weapon->GetComponent<CWeaponScript>(eComponentType::Script);
	wpSc->SetTarget(this->GetOwner());


	wpSc->SetDamage(currentWeapon.tier[tWeaponTier - 1].damage);
	wpSc->SetDelay(currentWeapon.tier[tWeaponTier - 1].delay);
	wpSc->SetCriticalDamage(currentWeapon.tier[tWeaponTier - 1].critDamagePer);
	wpSc->SetCriticalChance(currentWeapon.tier[tWeaponTier - 1].critChancePer);
	wpSc->SetRange(currentWeapon.tier[tWeaponTier - 1].range);
	wpSc->SetLifeSteal(currentWeapon.tier[tWeaponTier - 1].lifeSteal);
	wpSc->SetBasePrice(currentWeapon.tier[tWeaponTier - 1].basePrice);
	wpSc->SetSpeed(currentWeapon.speed);
	wpSc->SetTier(tWeaponTier);

	// 무기 타입(근접/원거리)에 따른 추가 설정
	if (tType == eLayerType::MeleeWeapon) {
		CCollider* wpCl = weapon->AddComponent<CBoxCollider2D>();
		wpCl->SetSize(SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY));
	}
	else if (tType == eLayerType::RangedWeapon) {
		// 원거리 무기는 총알(Bullet) 정보를 세팅
		CRangedWeaponScript* wpSc = weapon->GetComponent<CRangedWeaponScript>(eComponentType::Script);
		wpSc->SetBullet(SVector2D(currentWeapon.bulletSizeX, currentWeapon.bulletSizeY), SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY), currentWeapon.bulletName);
	}

	mWeapons.push_back(weapon);	// 인벤토리 추가
	return std::make_pair(mWeapons.size() - 1, mWeapons[mWeapons.size() - 1]);
}

// 무기 제거 (판매 또는 합성 소모 시)
bool CWeaponMgr::RemoveWeapon(int tIndex)
{
	if (mWeapons[tIndex] == nullptr || tIndex < 0 || tIndex >= mWeapons.size()) return false;
	CWeapon* w = mWeapons[tIndex];
	ObjDestroy(mWeapons[tIndex]);
	mWeapons.erase(mWeapons.begin() + tIndex);
	return true;
}

void CWeaponMgr::ResetWeapons()
{
	for (int i = mWeapons.size() - 1; i >= 0; i--) {
		RemoveWeapon(i);
	}
}

// [Formation Algorithm] 무기 배치 로직
// 플레이어 주변에 무기들을 원형으로 균등 배치하는 함수
void CWeaponMgr::WeaponsPosition()
{
	int count = mWeapons.size();
	SVector2D targetPos = GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos();

	if (count == 0) return;
	else if (count == 1) {
		// 무기가 1개일 때는 정면에 배치
		mWeapons[0]->GetComponent<CTransform>(eComponentType::Transform)->SetPos(targetPos);
		mWeapons[0]->GetComponent<CWeaponScript>(eComponentType::Script)->SetOffset(SVector2D(0.0f, 20.0f));
	}
	else {
		// 무기가 2개 이상일 때: 360도를 N등분하여 배치
		float radius = 60.0f;
		float angleStep = 360.0f / count;

		for (int i = 0; i < count; i++) {
			float angleDeg = angleStep * i;
			float angleRad = DegToRad(angleDeg);

			// [Trigonometry] 극좌표(각도, 거리) -> 직교좌표(x, y) 변환
			float offsetX = cosf(angleRad) * radius;
			float offsetY = sinf(angleRad) * radius;

			// 각 무기 스크립트에 오프셋 설정 (매 프레임 Update에서 이 오프셋을 따라 회전함)
			mWeapons[i]->GetComponent<CWeaponScript>(eComponentType::Script)->SetOffset(SVector2D(offsetX, offsetY));
		}
	}
}

// 상점에서 업그레이드 시 호출
void CWeaponMgr::ChangeWeaponTier(CWeapon* tWeapon, int tTier)
{
	// 데이터 매니저에서 해당 등급의 스탯을 다시 불러와 적용
	auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeapon->GetID());
	if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
		return;
	}

	CDataMgr::SWeapon currentWeapon = weaponIter->second;

	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return;
	}

	CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>(eComponentType::Script);

	// 스탯 갱신
	wpSc->SetDamage(currentWeapon.tier[tTier - 1].damage);
	wpSc->SetDelay(currentWeapon.tier[tTier - 1].delay);
	wpSc->SetCriticalDamage(currentWeapon.tier[tTier - 1].critDamagePer);
	wpSc->SetCriticalChance(currentWeapon.tier[tTier - 1].critChancePer);
	wpSc->SetRange(currentWeapon.tier[tTier - 1].range);
	wpSc->SetLifeSteal(currentWeapon.tier[tTier - 1].lifeSteal);
	wpSc->SetBasePrice(currentWeapon.tier[tTier - 1].basePrice);
	wpSc->SetSpeed(currentWeapon.speed);
	wpSc->SetTier(tTier);
}
