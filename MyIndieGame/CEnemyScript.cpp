#include "CEnemyScript.h"

#include "CTransform.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CCircleCollider2D.h"
#include "CSpriteRenderer.h"

#include "CPlayerScript.h"
#include "CWeaponScript.h"
#include "CBulletScript.h"

#include "Object.h"

void CEnemyScript::OnCreate()
{
	SetBobbingSpeed(static_cast<float>(std::rand() % 2) + 1);
	SetSquashMagnitude(0.2f);
}

void CEnemyScript::OnDestroy()
{
}

void CEnemyScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

	CTransform* plTr = mTarget->GetComponent<CTransform>();
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	SetDistanceToPlayer((plTr->GetPos() - tr->GetPos()).Length());

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>();
	}

	if (mTextureChangeDelay > 0) {
		mTextureChangeDelay -= tDeltaTime;
	}

	if (mTextureChangeDelay < 0) {
		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>();
		thisSr->SetTexture(mBaseTexture);
		mTextureChangeDelay = 0.0f;
	}

	if (mHP == 0) {
		ObjDestroy(GetOwner());
	}

	switch (mState) {
	case eState::Spawn:
		Spawn(tDeltaTime);
		break;
	case eState::Walk:
		Bounce();
		break;
	case eState::Attack:
		break;
	case eState::Dead:
		break;
	default:
		break;
	}
}

void CEnemyScript::OnLateUpdate(float tDeltaTime)
{
}

void CEnemyScript::Render(HDC tHDC)
{
}

void CEnemyScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	if (tOther->GetOwner()->GetLayerType() == eLayerType::Player) {
		GameObject* player = tOther->GetOwner();
		ButtDamageToPlayer(player);
	}

	if (tOther->GetOwner()->GetLayerType() == eLayerType::MeleeWeapon || tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
		GameObject* weapon = tOther->GetOwner();
		DamageByWeapon(weapon);

		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>();

		std::wstring collisionName = GetOwner()->GetName() + L"Collision";
		thisSr->SetTexture(CResourceMgr::Find<CTexture>(collisionName));

		mTextureChangeDelay = 0.3f;
	}
}

void CEnemyScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{

	if (tOther->GetOwner()->GetLayerType() == eLayerType::Player) {
		GameObject* player = tOther->GetOwner();
		ButtDamageToPlayer(player);
	}
}

void CEnemyScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{

}

void CEnemyScript::DecreaseHP(int tDecreaseAmount) {
	mHP = mHP - tDecreaseAmount;

	if (mHP < 0) {
		mHP = 0;
	}
}

void CEnemyScript::Spawn(float tDeltaTime)
{
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	CCollider* cl = GetOwner()->GetComponent<CCollider>();

	mBlinkTime -= tDeltaTime;

	if (mBlinkTime < 0.0f) {
		sr->SetTexture(mBaseTexture);
		sr->GetTexture()->CreateHBitmapFromGdiPlus(false);
		sr->SetAlphaMultiplier(1.0f);
		mState = eState::Walk;
		cl->SetActivate(true);
		return;
	}

	float alphaChange = mBlinkSpeed * tDeltaTime;

	if (mBlinkFadeOut) {
		mCurrentBlinkAlpha -= alphaChange;
		if (mCurrentBlinkAlpha <= 0.0f) {
			mCurrentBlinkAlpha = 0.0f;
			mBlinkFadeOut = false;
		}
	}
	else {
		mCurrentBlinkAlpha += alphaChange;
		if (mCurrentBlinkAlpha >= 1.0f) {
			mCurrentBlinkAlpha = 1.0f;
			mBlinkFadeOut = true;
		}
	}

	sr->SetAlphaMultiplier(mCurrentBlinkAlpha);
}

void CEnemyScript::ButtDamageToPlayer(GameObject* tPlayer)
{
	if (tPlayer->GetComponent<CPlayerScript>()->GetCanCollideEnemy()) {
		tPlayer->GetComponent<CPlayerScript>()->DecreaseHP(mButtDamage);
		tPlayer->GetComponent<CPlayerScript>()->SetCanCollideEnemy(false);
	}
}

void CEnemyScript::DamageByWeapon(GameObject* tWeapon)
{
	float damage;

	if (tWeapon->GetLayerType() == eLayerType::MeleeWeapon) {
		CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>();
		damage = wpSc->GetDamage();
	}
	else if (tWeapon->GetLayerType() == eLayerType::Bullet) {
		CBulletScript* blSc = tWeapon->GetComponent<CBulletScript>();
		damage = blSc->GetDamage();
	}

	DecreaseHP(damage);
}
