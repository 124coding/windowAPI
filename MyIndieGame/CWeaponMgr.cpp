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

	if (mWeapons.size() >= mWeaponCount) {
		if (tWeaponTier == 4) {
			return std::make_pair(-1, nullptr);
		}
		for (int i = 0; i < mWeapons.size(); i++) {
			CWeaponScript* wpSc = mWeapons[i]->GetComponent<CWeaponScript>(eComponentType::Script);
			if (mWeapons[i]->GetID() == tWeaponId && wpSc->GetTier() == tWeaponTier) {
				auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
				if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
					return std::make_pair(i, mWeapons[i]);
				}

				CDataMgr::SWeapon currentWeapon = weaponIter->second;

				wpSc->SetDamage(currentWeapon.tier[tWeaponTier].damage);
				wpSc->SetDelay(currentWeapon.tier[tWeaponTier].delay);
				wpSc->SetCriticalDamage(currentWeapon.tier[tWeaponTier].critDamagePer);
				wpSc->SetCriticalChance(currentWeapon.tier[tWeaponTier].critChancePer);
				wpSc->SetRange(currentWeapon.tier[tWeaponTier].range);
				wpSc->SetLifeSteal(currentWeapon.tier[tWeaponTier].lifeSteal);
				wpSc->SetBasePrice(currentWeapon.tier[tWeaponTier].basePrice);
				wpSc->SetSpeed(currentWeapon.speed);
				wpSc->SetTier(tWeaponTier + 1);

				return std::make_pair(i, mWeapons[i]);
			}
		}
		return std::make_pair(-1, nullptr);
	}

	auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
	if (weaponIter == CDataMgr::GetWeaponDatas().end()) {
		return std::make_pair(-1, nullptr);
	}

	CDataMgr::SWeapon currentWeapon = weaponIter->second;

	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return std::make_pair(-1, nullptr);
	}
	
	CWeapon* weapon = iter->second();
	weapon->SetID(tWeaponId);

	CTransform* wpTr = weapon->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(currentWeapon.name);
	weapon->SetSize(SVector2D(currentWeapon.sizeX, currentWeapon.sizeY));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 20.0f, wpImg->GetHeight() / 2);

	weapon->OnCreate();

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

	if (tType == eLayerType::MeleeWeapon) {
		CCollider* wpCl = weapon->AddComponent<CBoxCollider2D>();
		wpCl->SetSize(SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY));
	}
	else if (tType == eLayerType::RangedWeapon) {
		CRangedWeaponScript* wpSc = weapon->GetComponent<CRangedWeaponScript>(eComponentType::Script);
		wpSc->SetBullet(SVector2D(currentWeapon.bulletSizeX, currentWeapon.bulletSizeY), SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY), currentWeapon.bulletName);
	}

	mWeapons.push_back(weapon);
	return std::make_pair(mWeapons.size() - 1, mWeapons[mWeapons.size() - 1]);
}

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

void CWeaponMgr::WeaponsPosition()
{
	int count = mWeapons.size();
	SVector2D targetPos = GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos();

	if (count == 0) return;
	else if (count == 1) {
		mWeapons[0]->GetComponent<CTransform>(eComponentType::Transform)->SetPos(targetPos);
		mWeapons[0]->GetComponent<CWeaponScript>(eComponentType::Script)->SetOffset(SVector2D(0.0f, 20.0f));
	}
	else {
		float radius = 60.0f;
		float angleStep = 360.0f / count;

		for (int i = 0; i < count; i++) {
			float angleDeg = angleStep * i;
			float angleRad = DegToRad(angleDeg);

			float offsetX = cosf(angleRad) * radius;
			float offsetY = sinf(angleRad) * radius;

			mWeapons[i]->GetComponent<CWeaponScript>(eComponentType::Script)->SetOffset(SVector2D(offsetX, offsetY));
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

	CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>(eComponentType::Script);

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
