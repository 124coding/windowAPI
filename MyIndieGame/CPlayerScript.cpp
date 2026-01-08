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
	// 플레이어의 생동감을 위한 렌더링 효과 설정 (숨쉬는 듯한 효과)
	SetBobbingSpeed(1.3f);
	SetSquashMagnitude(0.1f);

	//애니메이터 지연 초기화(Lazy Initialization)
	if (mAnimator == nullptr) {
		if (GetOwner()->GetComponent<CAnimator>(eComponentType::Animator) != nullptr) {
			mAnimator = GetOwner()->GetComponent<CAnimator>(eComponentType::Animator);
		}
		else {
			mAnimator = GetOwner()->AddComponent<CAnimator>();
		}
	}
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::OnUpdate(float tDeltaTime)
{
	mTotalTime += tDeltaTime;
	// [HP Regeneration Logic]
	// 매 프레임 회복하는 것이 아니라, 누적 시간(Time Accumulator)을 계산하여 틱(Tick) 단위로 회복 처리
	if (mMaxHP > mHP) {
		mHPRegenTime += tDeltaTime;
	}
	else {
		mHPRegenTime = 0.0f;
	}

	// [FSM State Update]
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

	// [Death Check & Damage Logic]
	if (mState != eState::Dead) {
		if (mHP == 0) {
			// 모든 무기 비활성화
			CWeaponMgr* plWeaponMgr = GetOwner()->GetComponent<CWeaponMgr>(eComponentType::WeaponMgr);
			for (auto& weapon : plWeaponMgr->GetWeapons()) {
				weapon->SetState(false);
			}
			mState = eState::Dead;

			// 사망 연출을 위해 중심축(Anchor)을 중앙으로 보정하고 위치 재조정
			CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
			CPlayerScript* sc = GetOwner()->GetComponent<CPlayerScript>(eComponentType::Script);

			tr->SetVelocity(SVector2D());	// 이동 정지

			SVector2D size = GetOwner()->GetSize();
			SVector2D scale = tr->GetScale();

			// 텍스처 중심점으로 앵커 변경 (회전 연출을 위함)
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

		// 체력 재생 로직 (공식에 따라 일정 주기마다 1씩 회복)
		if (0.1f * mHPRegeneration * mHPRegenTime > 1.0f) {
			IncreaseHP(1.0f);
			mHPRegenTime = 0.0f;
			// 힐링 텍스트 표시 (초록색)
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), std::to_wstring((int)1.0f), Gdiplus::Color::LimeGreen);
		}

		// 무적 시간(Invincibility Frame) 쿨타임 처리
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
	// 키 입력 감지 시 이동 상태(Walk)로 전이
	if (mInputMgr->GetKeyPressed("DoMoveLt") || mInputMgr->GetKeyPressed("DoMoveRt") ||
		mInputMgr->GetKeyPressed("DoMoveFt") || mInputMgr->GetKeyPressed("DoMoveBt")) {
		mState = eState::Walk;
	}
}

void CPlayerScript::Move()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 실제 좌표 이동 로직 호출
	Translate(tr);

	// 모든 이동키를 떼면 Idle 상태로 복귀
	if ((mInputMgr->GetKeyUp("DoMoveLt") || mInputMgr->GetKeyNone("DoMoveLt")) &&
		(mInputMgr->GetKeyUp("DoMoveRt") || mInputMgr->GetKeyNone("DoMoveRt")) &&
		(mInputMgr->GetKeyUp("DoMoveFt") || mInputMgr->GetKeyNone("DoMoveFt")) &&
		(mInputMgr->GetKeyUp("DoMoveBt") || mInputMgr->GetKeyNone("DoMoveBt"))) {
		mState = eState::Idle;
		mAnimator->PlayAnimation(L"Idle", false);
	}
}

// 이동 및 맵 경계 처리
void CPlayerScript::Translate(CTransform* tr)
{
	SVector2D currentVelocity = SVector2D();
	CCollider* Cl = GetOwner()->GetComponent<CCollider>(eComponentType::Collider);

	// 1. 입력에 따른 방향 벡터 설정 (경계 체크 포함)
	// 맵 밖으로 나가려는 입력은 무시하여 벽에 막히는 느낌 구현
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

	// 2. 위치 클램핑 (Hard Clamping)
	// 빠른 속도로 이동 시 경계를 뚫는 것을 방지하기 위해 강제로 위치 보정
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

	// 3. 벡터 정규화 (Normalization)
	// 대각선 이동 시 속도가 빨라지는(루트 2배) 현상을 방지
	if (currentVelocity.Length() > 0.0f) {
		currentVelocity = currentVelocity.Normalize();
	}

	// 4. 최종 속도 적용 (이동 속도 스탯 반영)
	tr->SetVelocity(currentVelocity * GetSpeed() * (1 + mSpeedPercent / 100.0f));


	// 5. 스프라이트 좌우 반전 (Visual Feedback)
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	if (tr->GetVelocity().mX < 0) {
		sr->SetFlipX(true);
	}
	else if (tr->GetVelocity().mX > 0) {
		sr->SetFlipX(false);
	}
}

// 충돌 이벤트 처리 (피격 및 아이템 획득)
void CPlayerScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {
	// [Enemy Collision] 적과 충돌
	if (mCanCollideEnemy && tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
		mCanCollideEnemy = false;	// 무적 상태 돌입
		int rand = std::rand() % 100;

		// 회피(Dodge) 로직: 확률에 따라 데미지 무효화
		bool isDodged = false;
		if (mDodge > mDodgeLimit) {
			if (rand <= mDodgeLimit) isDodged = true; // 상한선 로직
		}
		else {
			if (rand <= mDodge) isDodged = true;
		}

		if (!isDodged) {
			GameObject* enemy = tOther->GetOwner();
			ButtDamageByEnemy(enemy); // 피격 처리
		}
		else {
			// 회피 텍스트 출력
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
		}
	}

	// [Material Collision] 아이템(경험치/돈) 획득
	if (tOther->GetOwner()->GetLayerType() == eLayerType::Material) {
		// 재화 및 경험치 획득 (배율 적용)
		this->ChangeMoney(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetMoney() * CMonsterSpawnMgr::GetMoneyMultiple());
		this->IncreasedExp(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetExp() * CMonsterSpawnMgr::GetExpMultiple());

		// [Level Up Logic] 경험치 충족 시 레벨업 처리
		if (this->mExp >= this->mNeedLevelUpExp) {
			this->mExp -= this->mNeedLevelUpExp;
			this->IncreaseLevel();
			this->mCurStageLevelUpCount++;
			this->IncreaseMaxHP(1);
			this->mNeedLevelUpExp *= 1.2f;		// 다음 레벨 필요 경험치 증가 (곡선형)
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

// 피격 데미지 계산 및 적용
void CPlayerScript::ButtDamageByEnemy(GameObject* tEnemy)
{
	float damage = tEnemy->GetComponent<CEnemyScript>(eComponentType::Script)->GetDamage();
	damage = DecreaseDamageBecauseArmor(damage);	// 방어력에 의한 경감
	IncreaseHP(-std::ceil(damage));	// 올림 처리하여 최소 1 데미지 보장 가능성 높임
	// 피격 데미지 텍스트 표시 (빨간색)
	CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), std::to_wstring((int)std::ceil(damage)), Gdiplus::Color::Red);
}

// 방어력 공식
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

// 사망 연출 (Rotation & Scaling)
void CPlayerScript::Death(float tDeltaTime) {
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer);

	// 완전히 작아지면 종료
	if (tr->GetScale().mX <= 0.0f && tr->GetScale().mY <= 0.0f) return;

	mDeadTimeTaken -= tDeltaTime;
	
	// 투명도/회전 처리를 위해 GDI+ 모드로 전환
	if (sr->GetTexture()->GetHBitmap(false) != nullptr || sr->GetTexture()->GetHBitmap(true) != nullptr) {
		sr->SetGdiplusDraw(true);
	}

	// 1. 뱅글뱅글 회전
	float curRot = tr->GetRot();
	float rotSpeed = 720.0f;

	tr->SetRot(curRot + (rotSpeed * tDeltaTime));

	// 2. 점점 작아짐 (Scale Down)
	float totalTime = 1.0f;
	float ratio = mDeadTimeTaken / totalTime;
	if (ratio < 0.0f) {
		ratio = 0.0f;
		GetOwner()->SetState(false);
	}
	tr->SetScale(SVector2D(ratio, ratio));
}

// 자석 기능 (Magnet)
// 주변의 아이템(Material)을 감지하여 플레이어 쪽으로 끌어당김
void CPlayerScript::Acquire()
{
	CTransform* plTr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 모든 Material 레이어 객체 순회
	for (auto& material : CSceneMgr::GetGameObjects(eLayerType::Material)) {
		CTransform* materialTr = material->GetComponent<CTransform>(eComponentType::Transform);

		// [Optimization] 거리 비교 시 sqrt 없는 LengthSq 사용
		// 획득 범위(200.0f) 내에 들어오면 Material의 상태를 Follow로 변경
		if ((plTr->GetPos() - materialTr->GetPos()).LengthSq() <= 200.0f * 200.0f) {
			material->GetComponent<CMaterialScript>(eComponentType::Script)->SetStateFollow();
		}
	}
}
