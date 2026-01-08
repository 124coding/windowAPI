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
	// 몬스터마다 움직임(Bobbing Speed)에 랜덤성을 주어 획일적인 움직임을 방지 (Variation)
	SetBobbingSpeed(static_cast<float>(std::rand() % 2) + 1);
	SetSquashMagnitude(0.2f);
}

void CEnemyScript::OnDestroy()
{
}

void CEnemyScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;

	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	// 이동 방향에 따른 스프라이트 좌우 반전 (Visual Feedback)
	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}

	if (mAnimator == nullptr) {
		mAnimator = GetOwner()->GetComponent<CAnimator>(eComponentType::Animator);
	}

	// [Hit Feedback] 피격 시 스프라이트가 잠시 하얗게 변했다가 원래대로 돌아오는 로직
	if (mTextureChangeDelay > 0) {
		mTextureChangeDelay -= tDeltaTime;
	}

	if (mTextureChangeDelay < 0) {
		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
		thisSr->SetTexture(mBaseTexture);	// 원본 텍스처 복구
		mTextureChangeDelay = 0.0f;
	}

	// [Death Logic] HP가 0이 되면 사망 상태로 전환
	if (mHP == 0 && mDeadTimeTaken >= 1.0f) {
		mState = eState::Dead;

		CCollider* cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);
		cl->SetActivate(false);		// 충돌체 비활성화 (시체에 부딪히지 않게)

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

		// [Drop Logic] 재료(돈/경험치) 드랍
		if (CMonsterSpawnMgr::GetTime() > 0) {
			CMaterial* material = Instantiate<CMaterial>(eLayerType::Material, ObjectCenterPos(GetOwner()));
			material->OnCreate();
		}
	}

	// [FSM State Update]
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

// 충돌 처리: 무기에 맞았을 때
void CEnemyScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
	// 근접 무기 또는 총알과 충돌 시
	if (tOther->GetOwner()->GetLayerType() == eLayerType::MeleeWeapon || tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
		CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

		// [Life Steal Logic] 흡혈 확률 계산
		if (plSc->GetMaxHP() > plSc->GetHP()) {
			int rand = std::rand() % 100;

			CWeaponScript* wpSc = nullptr;
			
			if (tOther->GetOwner()->GetLayerType() == eLayerType::Bullet) {
				wpSc = tOther->GetOwner()->GetComponent<CBulletScript>(eComponentType::Script)->GetWeapon()->GetComponent<CWeaponScript>(eComponentType::Script);
			}
			else {
				wpSc = tOther->GetOwner()->GetComponent<CWeaponScript>(eComponentType::Script);
			}
			
			
			int lifeSteal = plSc->GetLifeSteal();

			if (rand <= lifeSteal + wpSc->GetLifeSteal()) {
				plSc->IncreaseHP(1);
				// 힐 이펙트
				CEffectMgr::ShowEffectText(CPlayScene::GetPlayer()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), std::to_wstring((int)1.0f), Gdiplus::Color::LimeGreen);
			}
		}

		// 데미지 처리 함수 호출
		GameObject* weapon = tOther->GetOwner();
		DamageByWeapon(weapon);

		// [Hit Flash Effect] 피격 시 스프라이트 교체 (하얀색 실루엣 등)
		CSpriteRenderer* thisSr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

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

// [Spawn Animation] 등장 시 깜빡임(Alpha Blinking) 연출
void CEnemyScript::Spawn(float tDeltaTime)
{
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
	CCollider* cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);

	mBlinkTime -= tDeltaTime;

	// 스폰 시간 종료 시 정상 상태로 전환
	if (mBlinkTime < 0.0f) {
		sr->SetTexture(mBaseTexture);
		sr->SetAlphaMultiplier(1.0f);
		mState = eState::Walk;
		cl->SetActivate(true);	// 충돌체 활성화 (이제 공격 가능)
		return;
	}

	// 알파값(투명도)을 오르내리며 깜빡이는 효과 구현 (Sine Wave 유사)
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

// [Death Animation] 사망 시 회전하며 축소 (Juice)
void CEnemyScript::Death(float tDeltaTime) {
	
	mDeadTimeTaken -= tDeltaTime;

	// GDI+ 모드 활성화 (회전/투명도 지원)
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);
	if (sr->GetTexture()->GetHBitmap(false) != nullptr || sr->GetTexture()->GetHBitmap(true) != nullptr) {
		sr->SetGdiplusDraw(true);
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 회전 (Spin)
	float curRot = tr->GetRot();
	float rotSpeed = 720.0f;

	tr->SetRot(curRot + (rotSpeed * tDeltaTime));

	// 축소 (Scale Down)
	float totalTime = 1.0f;
	float ratio = mDeadTimeTaken / totalTime;
	if (ratio < 0.0f) ratio = 0.0f;

	tr->SetScale(SVector2D(ratio, ratio));

	// 연출 종료 시 객체 파괴
	if (mDeadTimeTaken < 0.0f) {
		ObjDestroy(GetOwner());
	}
}

// [Damage Logic] 무기 타입에 따른 데미지 계산 및 UI 출력
void CEnemyScript::DamageByWeapon(GameObject* tWeapon)
{
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);
	CWeaponScript::SDamageInfo dmgInfo = { 0.0f, false };

	// 무기 타입별(근접/원거리) 추가 데미지 합산
	if (tWeapon->GetLayerType() == eLayerType::MeleeWeapon) {
		CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>(eComponentType::Script);
		wpSc->SetDamage(wpSc->GetDamage() + plSc->GetMeleeDamage());
		dmgInfo = wpSc->GetFinalDamage();
	}
	else if (tWeapon->GetLayerType() == eLayerType::Bullet) {
		CBulletScript* blSc = tWeapon->GetComponent<CBulletScript>(eComponentType::Script);
		dmgInfo = blSc->GetFinalDamage(plSc->GetRangedDamage());
	}

	DecreaseHP(dmgInfo.damage);

	// 타격 이펙트 및 데미지 텍스트 출력
	SVector2D pos = GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos();

	CEffectMgr::PlayEffect(L"EnemyHit", pos, GetOwner());

	// [Floating Text] 치명타 여부에 따라 색상 변경 (노란색/흰색)
	Gdiplus::Color textColor = Gdiplus::Color(255, 255, 255);
	if (dmgInfo.isCritical) {
		textColor = Gdiplus::Color(255, 255, 0);
	}

	SVector2D textPos = pos;
	textPos.mY -= 50.0f; // 머리 위로 살짝 올리기

	CEffectMgr::ShowEffectText(textPos, std::to_wstring((int)dmgInfo.damage), textColor);
}
