#include "CPlayerScript.h"

#include "CEffectMgr.h"

#include "GameObject.h"
#include "CPlayer.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CSpriteRenderer.h"
#include "CTilemapRenderer.h"
#include "CEnemyScript.h"

void CPlayerScript::OnCreate()
{
	SetBobbingSpeed(1.3f);
	SetSquashMagnitude(0.1f);
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;
	if (mMaxHP > mHP) {
		mHPRegenTime += tDeltaTime;
	}
	else {
		mHPRegenTime = 0.0f;
	}

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}
	switch (mState) {
	case eState::Idle:
		Idle();
		break;
	case eState::Walk:
		Move();
		break;
	default:
		break;
	}

	Bounce();

	if (0.1f * mHPRegeneration * mHPRegenTime > 1.0f) {
		IncreaseHP(1.0f);
		mHPRegenTime = 0.0f;
		CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>()->GetPos(), std::to_wstring((int)1.0f), Gdiplus::Color::LimeGreen);
	}

	if (!mCanCollideEnemy) {
		mGracePeriod -= tDeltaTime;
		if (mGracePeriod <= 0) {
			mCanCollideEnemy = true;
			mGracePeriod = 0.2f;
		}
	}

}

void CPlayerScript::OnLateUpdate(float tDeltaTime)
{
}

void CPlayerScript::Render(HDC tHDC)
{
}

void CPlayerScript::Idle()
{
	if (mInputMgr->GetKeyPressed("DoMoveLt")) {
		mState = eState::Walk;
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt")) {
		mState = eState::Walk;
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt")) {
		mState = eState::Walk;
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt")) {
		mState = eState::Walk;
	}
}

void CPlayerScript::Move()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	Translate(tr);

	if ((mInputMgr->GetKeyUp("DoMoveLt") || mInputMgr->GetKeyNone("DoMoveLt")) &&
		(mInputMgr->GetKeyUp("DoMoveRt") || mInputMgr->GetKeyNone("DoMoveRt")) &&
		(mInputMgr->GetKeyUp("DoMoveFt") || mInputMgr->GetKeyNone("DoMoveFt")) &&
		(mInputMgr->GetKeyUp("DoMoveBt") || mInputMgr->GetKeyNone("DoMoveBt"))) {
		mState = eState::Idle;
		mAnimator->PlayAnimation(L"Idle", false);
	}
}

void CPlayerScript::Translate(CTransform* tr)
{
	SVector2D currentVelocity = SVector2D();
	CCollider* Cl = GetOwner()->GetComponent<CCollider>();

	if (mInputMgr->GetKeyPressed("DoMoveLt") && tr->GetPos().mX + Cl->GetOffset().mX > tileSizeX / 2) {
		currentVelocity.mX += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveRt") && tr->GetPos().mX + Cl->GetOffset().mX < mapWidth - tileSizeX / 2) {
		currentVelocity.mX += 1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveFt") && tr->GetPos().mY + Cl->GetOffset().mY > tileSizeY / 2) {
		currentVelocity.mY += -1.0f;
	}

	if (mInputMgr->GetKeyPressed("DoMoveBt") && tr->GetPos().mY + Cl->GetOffset().mY < mapHeight - tileSizeY / 2) {
		currentVelocity.mY += 1.0f;
	}

	if (tr->GetPos().mX + Cl->GetOffset().mX < tileSizeX / 2) {
		tr->SetPos(SVector2D((tileSizeX / 2 + 1) - Cl->GetOffset().mX, tr->GetPos().mY));
	}
	if (tr->GetPos().mX + Cl->GetOffset().mX > mapWidth - tileSizeX / 2) {
		tr->SetPos(SVector2D(mapWidth - (tileSizeX / 2) - 1 - Cl->GetOffset().mX, tr->GetPos().mY));
	}
	if (tr->GetPos().mY + Cl->GetOffset().mY < tileSizeY / 2) {
		tr->SetPos(SVector2D(tr->GetPos().mX, (tileSizeY / 2 + 1) - Cl->GetOffset().mY));
	}
	if (tr->GetPos().mY + Cl->GetOffset().mY > mapHeight - tileSizeY / 2) {
		tr->SetPos(SVector2D(tr->GetPos().mX, mapHeight - (tileSizeY / 2) - 1 - Cl->GetOffset().mY));
	}

	if (currentVelocity.Length() > 0.0f) {
		currentVelocity = currentVelocity.Normalize();
	}

	tr->SetVelocity(currentVelocity * GetSpeed() * (1 + mSpeedPercent / 100.0f));

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}
}

void CPlayerScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {
	if (mCanCollideEnemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100;

		if (mDodge > mDodgeLimit) {
			if (rand > mDodgeLimit) {
				if (tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
					GameObject* enemy = tOther->GetOwner();
					ButtDamageByEnemy(enemy);
				}
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>()->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
		else {
			if (rand > mDodge) {
				if (tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
					GameObject* enemy = tOther->GetOwner();
					ButtDamageByEnemy(enemy);
				}
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>()->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
	}
}

void CPlayerScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

	if (mCanCollideEnemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100;
		if (rand > mDodge) {
			if (tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
		}
		else {
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>()->GetPos(), L"회피", Gdiplus::Color::White);
		}
	}
}

void CPlayerScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}

void CPlayerScript::IncreaseHP(int tIncreaseAmount) {
	mHP = mHP + tIncreaseAmount;

	if (mHP > mMaxHP) {
		mHP = mMaxHP;
	}

	if (mHP < 0) {
		mHP = 0;
	}
}

void CPlayerScript::IncreaseMaxHP(int tIncreaseAmount)
{
	mMaxHP = mMaxHP + tIncreaseAmount * GetStatMultiplier(L"MaxHP");

	if (mMaxHP < 0) {
		mMaxHP = 0;
	}
}

void CPlayerScript::ButtDamageByEnemy(GameObject* tEnemy)
{
	float damage = tEnemy->GetComponent<CEnemyScript>()->GetDamage();
	damage = DecreaseDamageBecauseArmor(damage);
	IncreaseHP(-std::ceil(damage));
	CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>()->GetPos(), std::to_wstring((int)std::ceil(damage)), Gdiplus::Color::Red);
}

float CPlayerScript::DecreaseDamageBecauseArmor(float tDamage)
{
	float damage = 0;
	if (mArmor >= 0) {
		damage = tDamage * (1 / (1 + mArmor / 15));
	}
	else if (mArmor < 0) {
		damage = tDamage * (15 - 2 * mArmor) / (15 - mArmor);
	}

	return damage;
}
