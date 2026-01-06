#include "CPlayerScript.h"

#include "CEffectMgr.h"
#include "CMonsterSpawnMgr.h"

#include "GameObject.h"
#include "CPlayer.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CSpriteRenderer.h"
#include "CTilemapRenderer.h"
#include "CEnemyScript.h"
#include "CWeaponMgr.h"
#include "CMaterialScript.h"

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
		mAnimator = GetOwner()->GetComponent<CAnimator>(eComponentType::Animator);
	}
	switch (mState) {
	case eState::Idle:
		Idle();
		Bounce();
		Acquire();
		break;
	case eState::Walk:
		Move();
		Bounce();
		Acquire();
		break;
	case eState::Dead:
		Death(tDeltaTime);
		break;
	default:
		break;
	}

	if (mState != eState::Dead) {
		if (mHP == 0) {
			CWeaponMgr* plWeaponMgr = GetOwner()->GetComponent<CWeaponMgr>(eComponentType::WeaponMgr);
			for (auto& weapon : plWeaponMgr->GetWeapons()) {
				weapon->SetState(false);
			}
			mState = eState::Dead;

			CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
			CPlayerScript* sc = GetOwner()->GetComponent<CPlayerScript>(eComponentType::Script);

			tr->SetVelocity(SVector2D());

			SVector2D size = GetOwner()->GetSize();
			SVector2D scale = tr->GetScale();

			SVector2D originAnchor = GetOwner()->GetAnchorPoint();
			GetOwner()->SetAnchorPoint(sc->GetBaseTexture()->GetWidth() / 2, sc->GetBaseTexture()->GetHeight() / 2);

			float modifyWidth = originAnchor.mX - sc->GetBaseTexture()->GetWidth() / 2;
			float modifyHeight = originAnchor.mY - sc->GetBaseTexture()->GetHeight() / 2;

			modifyWidth *= (size.mX * scale.mX);
			modifyHeight *= (size.mY * scale.mY);

			SVector2D curPos = tr->GetPos();
			curPos.mX -= modifyWidth;
			curPos.mY -= modifyHeight;
			tr->SetPos(curPos);

			return;
		}

		if (0.1f * mHPRegeneration * mHPRegenTime > 1.0f) {
			IncreaseHP(1.0f);
			mHPRegenTime = 0.0f;
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), std::to_wstring((int)1.0f), Gdiplus::Color::LimeGreen);
		}

		if (!mCanCollideEnemy) {
			mGracePeriod -= tDeltaTime;
			if (mGracePeriod <= 0) {
				mCanCollideEnemy = true;
				mGracePeriod = 0.2f;
			}
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
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

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
	CCollider* Cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);

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

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}
}

void CPlayerScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {
	if (mCanCollideEnemy && tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100;

		if (mDodge > mDodgeLimit) {
			if (rand > mDodgeLimit) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
		else {
			if (rand > mDodge) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
	}

	if (tOther->GetOwner()->GetLayerType() == eLayerType::Material) {
		this->ChangeMoney(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetMoney() * CMonsterSpawnMgr::GetMoneyMultiple());
		this->IncreasedExp(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetExp() * CMonsterSpawnMgr::GetExpMultiple());

		if (this->mExp >= this->mNeedLevelUpExp) {
			this->mExp -= this->mNeedLevelUpExp;
			this->IncreaseLevel();
			this->mCurStageLevelUpCount++;
			this->IncreaseMaxHP(1);
			this->mNeedLevelUpExp *= 1.2f;
		}

		ObjDestroy(tOther->GetOwner());
	}
}

void CPlayerScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

	if (mCanCollideEnemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100 + 1;
		if (rand > mDodge) {
			if (tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
		}
		else {
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
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
	float damage = tEnemy->GetComponent<CEnemyScript>(eComponentType::Script)->GetDamage();
	damage = DecreaseDamageBecauseArmor(damage);
	IncreaseHP(-std::ceil(damage));
	CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), std::to_wstring((int)std::ceil(damage)), Gdiplus::Color::Red);
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

void CPlayerScript::ResetStats()
{
	mHP = 10;
	mMaxHP = 10;
	mHPRegeneration = 0;

	mDodge = 0;
	mArmor = 0;
	mBasicMoveSpeed = 450;
	mSpeedPercent = 0;

	mRange = 0;
	mDamagePer = 0;
	mCriticalChancePer = 0;
	mMeleeDamage = 0;
	mRangedDamage = 0;
	mAttackSpeedPer = 0;

	mLifeSteal = 0;

	mLevel = 0;
	mExp = 0.0f;
	mNeedLevelUpExp = 20.0f;
	mMoney = 0;

	mStatGainModifiers.clear();
}

void CPlayerScript::Death(float tDeltaTime) {
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
	if (tr->GetScale().mX <= 0.0f && tr->GetScale().mY <= 0.0f) return;

	mDeadTimeTaken -= tDeltaTime;
	
	if (sr->GetTexture()->GetHBitmap(false) != nullptr || sr->GetTexture()->GetHBitmap(true) != nullptr) {
		sr->SetGdiplusDraw(true);
	}

	float curRot = tr->GetRot();
	float rotSpeed = 720.0f;

	tr->SetRot(curRot + (rotSpeed * tDeltaTime));

	float totalTime = 1.0f;
	float ratio = mDeadTimeTaken / totalTime;
	if (ratio < 0.0f) {
		ratio = 0.0f;
		GetOwner()->SetState(false);
	}
	tr->SetScale(SVector2D(ratio, ratio));
}

void CPlayerScript::Acquire()
{
	CTransform* plTr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	for (auto& material : CSceneMgr::GetGameObjects(eLayerType::Material)) {
		CTransform* materialTr = material->GetComponent<CTransform>(eComponentType::Transform);
		if ((plTr->GetPos() - materialTr->GetPos()).LengthSq() <= 200.0f * 200.0f) {
			material->GetComponent<CMaterialScript>(eComponentType::Script)->SetStateFollow();
		}
	}
}
