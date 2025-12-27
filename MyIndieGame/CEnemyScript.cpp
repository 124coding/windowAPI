#include "CEnemyScript.h"

#include "CEffectMgr.h"
#include "CMonsterSpawnMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"
#include "CMaterial.h"

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

	CTransform* tr = GetOwner()->GetComponent<CTransform>();

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

	if (mHP == 0 && mDeadTimeTaken >= 1.0f) {
		mState = eState::Dead;

		CCollider* cl = GetOwner()->GetComponent<CCollider>();
		cl->SetActivate(false);

		// AnchorPoint가 가운데가 아니더라도 가운데를 기준으로 회전시키기 위함
		SVector2D size = GetOwner()->GetSize();
		SVector2D scale = tr->GetScale();

		SVector2D originAnchor = GetOwner()->GetAnchorPoint();
		GetOwner()->SetAnchorPoint(sr->GetTexture()->GetWidth() / 2, sr->GetTexture()->GetHeight() / 2);

		float modifyWidth = originAnchor.mX - sr->GetTexture()->GetWidth() / 2;
		float modifyHeight = originAnchor.mY - sr->GetTexture()->GetHeight() / 2;

		modifyWidth *= (size.mX * scale.mX);
		modifyHeight *= (size.mY * scale.mY);

		SVector2D curPos = tr->GetPos();
		curPos.mX -= modifyWidth;
		curPos.mY -= modifyHeight;
		tr->SetPos(curPos);

		if (CMonsterSpawnMgr::GetTime() > 0) {
			CMaterial* material = Instantiate<CMaterial>(eLayerType::Material, ObjectCenterPos(GetOwner()));
			material->OnCreate();
		}
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
		Death(tDeltaTime);
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
	if (tOther->GetOwner()->GetLayerType() == eLayerType::MeleeWeapon || tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
		CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();

		if (plSc->GetMaxHP() > plSc->GetHP()) {
			int rand = std::rand() % 100;



			CWeaponScript* wpSc = nullptr;
			
			if (tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
				wpSc = tOther->GetOwner()->GetComponent<CBulletScript>()->GetWeapon()->GetComponent<CWeaponScript>();
			}
			else {
				wpSc = tOther->GetOwner()->GetComponent<CWeaponScript>();
			}
			
			
			int lifeSteal = plSc->GetLifeSteal();

			if (rand <= lifeSteal + wpSc->GetLifeSteal()) {
				plSc->IncreaseHP(1);
				CEffectMgr::ShowEffectText(CPlayScene::GetPlayer()->GetComponent<CTransform>()->GetPos(), std::to_wstring((int)1.0f), Gdiplus::Color::LimeGreen);
			}
		}

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

void CEnemyScript::Death(float tDeltaTime) {
	
	mDeadTimeTaken -= tDeltaTime;

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr->GetTexture()->GetHBitmap(false) != nullptr || sr->GetTexture()->GetHBitmap(true) != nullptr) {
		sr->SetGdiplusDraw(true);
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	float curRot = tr->GetRot();
	float rotSpeed = 720.0f;

	tr->SetRot(curRot + (rotSpeed * tDeltaTime));

	float totalTime = 1.0f;
	float ratio = mDeadTimeTaken / totalTime;
	if (ratio < 0.0f) ratio = 0.0f;

	tr->SetScale(SVector2D(ratio, ratio));

	if (mDeadTimeTaken < 0.0f) {
		ObjDestroy(GetOwner());
	}
}

void CEnemyScript::DamageByWeapon(GameObject* tWeapon)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>();
	CWeaponScript::SDamageInfo dmgInfo = { 0.0f, false };

	if (tWeapon->GetLayerType() == eLayerType::MeleeWeapon) {
		CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>();
		wpSc->SetDamage(wpSc->GetDamage() + plSc->GetMeleeDamage());
		dmgInfo = wpSc->GetFinalDamage();
	}
	else if (tWeapon->GetLayerType() == eLayerType::Bullet) {
		CBulletScript* blSc = tWeapon->GetComponent<CBulletScript>();
		dmgInfo = blSc->GetFinalDamage(plSc->GetRangedDamage());
	}

	DecreaseHP(dmgInfo.damage);

	SVector2D pos = GetOwner()->GetComponent<CTransform>()->GetPos();

	CEffectMgr::PlayEffect(L"EnemyHit", pos, GetOwner());

	Gdiplus::Color textColor = Gdiplus::Color(255, 255, 255);
	if (dmgInfo.isCritical) {
		textColor = Gdiplus::Color(255, 255, 0);
	}

	SVector2D textPos = pos;
	textPos.mY -= 50.0f; // 머리 위로 살짝 올리기

	CEffectMgr::ShowEffectText(textPos, std::to_wstring((int)dmgInfo.damage), textColor);
}
