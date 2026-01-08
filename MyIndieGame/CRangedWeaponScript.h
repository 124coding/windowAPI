#pragma once

#include "CWeaponScript.h"

class CBullet;

class CRangedWeaponScript : public CWeaponScript
{
public:
	CRangedWeaponScript() : mBullet(nullptr) {}
	virtual ~CRangedWeaponScript() {}

	virtual CComponent* Clone() override {
		return new CRangedWeaponScript(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnCollisionEnter(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionStay(float tDeltaTime, CCollider* tOther) override;
	void OnCollisionExit(float tDeltaTime, CCollider* tOther) override;

	// ==========================================
	// AI Logic (Attack Condition)
	// ==========================================
	// 사거리(Range) 내에 적이 있는지 확인하고, 공격 가능한 상태(Cooldown)인지 판단
	void CanAttackCheck(std::vector<GameObject*> tEnemies);

public:
	// ==========================================
	// Bullet Configuration (Projectile Builder)
	// ==========================================
	// 이 무기가 발사할 총알의 속성(크기, 충돌체 크기, 이미지)을 미리 설정
	// 무기 획득 시점이나 초기화 시점에 한 번 호출됨
	void SetBullet(SVector2D tSize, SVector2D tColliderSize, const std::wstring& tTextureName);

	CBullet* GetBullet() {
		return this->mBullet;
	}

private:
	// 발사된 총알들을 리스트로 관리하는 것이 아니라,
	// "발사될 총알의 원형(Template)"을 들고 있음.
	// 공격 시 이 mBullet의 정보를 바탕으로 실제 게임 월드에 새로운 총알 객체(Instance)를 Instantiate함.
	CBullet* mBullet;
};

