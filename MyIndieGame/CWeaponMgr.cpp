#include "CWeaponMgr.h"

#include "Object.h"
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

bool CWeaponMgr::PlusWeapon(eLayerType tType, std::string tWeaponId, int tWeaponTier) {

	std::string weaponType;

	if (tType == eLayerType::MeleeWeapon) {
		weaponType = "MeleeWeapons";
	}
	else if (tType == eLayerType::RangedWeapon) {
		weaponType = "RangedWeapons";
	}

	CDataMgr::SWeapon currentWeapon;

	for (const auto& weaponData : CDataMgr::GetWeaponDatas()[weaponType]) {
		if (weaponData["W_ID"] == tWeaponId) {
			currentWeapon.ID = weaponData["W_ID"];
			currentWeapon.name = weaponData["Name"];
			currentWeapon.range = weaponData["Range"];
			currentWeapon.speed = weaponData["Speed"];
			currentWeapon.sizeX = weaponData["SizeX"];
			currentWeapon.sizeY = weaponData["SizeY"];
			currentWeapon.collisionSizeX = weaponData["CollisionSizeX"];
			currentWeapon.collisionSizeY = weaponData["CollisionSizeY"];

			if (weaponType == "RangedWeapons") {
				currentWeapon.bulletName = weaponData["BulletName"];
				currentWeapon.bulletSizeX = weaponData["BulletSizeX"];
				currentWeapon.bulletSizeY = weaponData["BulletSizeY"];
			}

			int i = 0;
			for (const auto& t : weaponData["Tiers"]) {
				currentWeapon.tier[i].tier = t["Tier"];
				currentWeapon.tier[i].damage = t["Damage"];
				currentWeapon.tier[i++].delay = t["Delay"];
			}

			break;
		}
	}

	if (currentWeapon.ID != tWeaponId) {
		return false;
	}

	auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.name);
	if (iter == CDataMgr::GetWeaponCreator().end()) {
		return false;
	}
	
	CWeapon* weapon = iter->second();

	CTransform* wpTr = weapon->GetComponent<CTransform>();
	CSpriteRenderer* wpSr = weapon->AddComponent<CSpriteRenderer>();

	CTexture* wpImg = CResourceMgr::Find<CTexture>(std::wstring(currentWeapon.name.begin(), currentWeapon.name.end()));
	weapon->SetSize(SVector2D(currentWeapon.sizeX, currentWeapon.sizeY));
	wpSr->SetTexture(wpImg);
	weapon->SetAnchorPoint((wpImg->GetWidth() / 2) - 20.0f, wpImg->GetHeight() / 2);

	weapon->OnCreate();

	CWeaponScript* wpSc = weapon->GetComponent<CWeaponScript>();
	wpSc->SetTarget(this->GetOwner());


	wpSc->SetDamage(currentWeapon.tier[tWeaponTier - 1].damage);
	wpSc->SetDelay(currentWeapon.tier[tWeaponTier - 1].delay);

	wpSc->SetRange(currentWeapon.range);
	wpSc->SetSpeed(currentWeapon.speed);

	if (tType == eLayerType::MeleeWeapon) {
		CCollider* wpCl = weapon->AddComponent<CBoxCollider2D>();
		wpCl->SetSize(SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY));
	}
	else if (tType == eLayerType::RangedWeapon) {
		CRangedWeaponScript* wpSc = weapon->GetComponent<CRangedWeaponScript>();
		wpSc->SetBullet(SVector2D(currentWeapon.bulletSizeX, currentWeapon.bulletSizeY), SVector2D(currentWeapon.collisionSizeX, currentWeapon.collisionSizeY), std::wstring(currentWeapon.bulletName.begin(), currentWeapon.bulletName.end()));
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
