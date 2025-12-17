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

	if (mWeapons.size() >= mWeaponCount) {
		return false;
	}

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
	weapon->SetID(tWeaponId);

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
	wpSc->SetCriticalDamage(currentWeapon.tier[tWeaponTier - 1].critDamagePer);
	wpSc->SetCriticalChance(currentWeapon.tier[tWeaponTier - 1].critChancePer);
	wpSc->SetRange(currentWeapon.tier[tWeaponTier - 1].range);
	wpSc->SetLifeSteal(currentWeapon.tier[tWeaponTier - 1].lifeSteal);
	wpSc->SetBasePrice(currentWeapon.tier[tWeaponTier - 1].basePrice);
	wpSc->SetSpeed(currentWeapon.speed);
	wpSc->SetTier(tWeaponTier);

	if (tType == eLayerType::MeleeWeapon) {
		CCollider* wpCl = weapon->AddComponent<CBoxCollider2D>();
		wpCl->SetSize(SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY));
	}
	else if (tType == eLayerType::RangedWeapon) {
		CRangedWeaponScript* wpSc = weapon->GetComponent<CRangedWeaponScript>();
		wpSc->SetBullet(SVector2D(currentWeapon.bulletSizeX, currentWeapon.bulletSizeY), SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY), currentWeapon.bulletName);
	}

	mWeapons.push_back(weapon);
	return true;
}

bool CWeaponMgr::RemoveWeapon(int tIndex)
{
	if (mWeapons[tIndex] == nullptr || tIndex < 0 || tIndex >= mWeapons.size()) return false;
	ObjDestroy(mWeapons[tIndex]);
	mWeapons.erase(mWeapons.begin() + tIndex);
	return true;
}

void CWeaponMgr::WeaponsPosition()
{
	int count = mWeapons.size();
	SVector2D targetPos = GetOwner()->GetComponent<CTransform>()->GetPos();

	if (count == 0) return;
	else if (count == 1) {
		mWeapons[0]->GetComponent<CTransform>()->SetPos(targetPos);
		mWeapons[0]->GetComponent<CWeaponScript>()->SetOffset(SVector2D(0.0f, 20.0f));
	}
	else {
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
}

void CWeaponMgr::ChangeWeaponTier(CWeapon* tWeapon, int tTier)
{
	auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeapon->GetID());
	if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
		return;
	}

	CDataMgr::SWeapon currentWeapon = weaponIter->second;

	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return;
	}

	CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>();

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
