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

bool CWeaponMgr::PlusWeapon(eLayerType tType, std::wstring tWeaponId, int tWeaponTier) {

	auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
	if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
		return false;
	}

	CDataMgr::SWeapon currentWeapon = weaponIter->second;

	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return false;
	}
	
	CWeapon* weapon = iter->second();

	CTransform* wpTr = weapon->GetComponent<CTransform>();
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(currentWeapon.name);
	weapon->SetSize(SVector2D(currentWeapon.sizeX, currentWeapon.sizeY));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 20.0f, wpImg->GetHeight() / 2);

	weapon->OnCreate();

	CWeaponScript* wpSc = weapon->GetComponent<CWeaponScript>();
	wpSc->SetTarget(this->GetOwner());


	wpSc->SetDamage(currentWeapon.tier[tWeaponTier - 1].damage);
	wpSc->SetDelay(currentWeapon.tier[tWeaponTier - 1].delay);
	wpSc->SetRange(currentWeapon.tier[tWeaponTier - 1].range);
	wpSc->SetCriticalDamage(currentWeapon.tier[tWeaponTier - 1].critDamagePer);
	wpSc->SetCriticalChance(currentWeapon.tier[tWeaponTier - 1].critChancePer);
	wpSc->SetBasePrice(currentWeapon.tier[tWeaponTier - 1].basePrice);
	wpSc->SetSpeed(currentWeapon.speed);

	if (tType == eLayerType::MeleeWeapon) {
		CCollider* wpCl = weapon->AddComponent<CBoxCollider2D>();
		wpCl->SetSize(SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY));
	}
	else if (tType == eLayerType::RangedWeapon) {
		CRangedWeaponScript* wpSc = weapon->GetComponent<CRangedWeaponScript>();
		wpSc->SetBullet(SVector2D(currentWeapon.bulletSizeX, currentWeapon.bulletSizeY), SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY), currentWeapon.bulletName);
	}

	if (mWeapons.size() < mWeaponCount) {
		mWeapons.push_back(weapon);
		return true;
	}

	return false;
}

void CWeaponMgr::WeaponsPosition()
{
	int count = mWeapons.size();
	if (count == 0) return;

	SVector2D targetPos = GetOwner()->GetComponent<CTransform>()->GetPos();

	float radius = 60.0f;
	float angleStep = 360.0f / count;

	for (int i = 0; i < count; i++) {
		float angleDeg = angleStep * i;
		float angleRad = DegToRad(angleDeg);

		float offsetX = cosf(angleRad) * radius;
		float offsetY = sinf(angleRad) * radius;

		mWeapons[i]->GetComponent<CWeaponScript>()->SetOffset(SVector2D(offsetX, offsetY));
	}
}
