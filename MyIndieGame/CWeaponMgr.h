#pragma once

#include "CComponent.h"
#include "CWeapon.h"

class CWeaponMgr : public CComponent
{
public:
	CWeaponMgr() : CComponent(eComponentType::WeaponMgr) {
	};
	virtual ~CWeaponMgr() {};

	virtual CComponent* Clone() override {
		return new CWeaponMgr(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// 새로운 무기를 생성하여 인벤토리에 추가
	std::pair<int, CWeapon*> PlusWeapon(eLayerType tType, std::wstring tWeaponId, int tWeaponTier);

	// 특정 인덱스의 무기 판매 또는 합성 시 제거
	bool RemoveWeapon(int tIndex);

	// 게임 재시작 시 초기화
	void ResetWeapons();

	// 소지한 무기 개수에 맞춰 플레이어 주변에 무기들을 정렬시키는 함수
	void WeaponsPosition();

	// 전체 무기 리스트 반환 (공격 로직이나 UI 표시용)
	std::vector<CWeapon*>& GetWeapons() {
		return this->mWeapons;
	}

	// 최대 무기 슬롯 개수 설정 (기본 6개)
	void SetWeaponCount(int tCount) {
		this->mWeaponCount = tCount;
	}

	// 같은 무기를 합성하여 상위 티어로 만들 때 사용
	// 기존 무기의 스탯(공격력, 이펙트 등)을 해당 티어에 맞게 갱신함
	void ChangeWeaponTier(CWeapon* tWeapon, int tTier);

private:
	// 실제 소유 중인 무기 객체들의 포인터 배열
	std::vector<CWeapon*> mWeapons;

	// 소지 한도 (Brotato 류 게임의 특징인 6개 제한)
	int mWeaponCount = 6;
};

