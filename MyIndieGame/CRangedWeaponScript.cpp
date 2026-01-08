#include "CRangedWeaponScript.h"

#include "CBullet.h"

#include "CSceneMgr.h"
#include "CResourceMgr.h"

#include "CPlayScene.h"

#include "CPlayer.h"

#include "CPlayerScript.h"

#include "CCircleCollider2D.h"
#include "CBulletScript.h"

#include "Object.h"

void CRangedWeaponScript::OnCreate()
{
	CWeaponScript::OnCreate();
}

void CRangedWeaponScript::OnDestroy()
{
	CWeaponScript::OnDestroy();

	// 이 스크립트가 소멸될 때, 들고 있던 '총알 견본(Template)'도 메모리 해제
	// mBullet은 씬에 등록되지 않은 독자적인 객체이므로 직접 delete 해야 함
	if (mBullet != nullptr) {
		mBullet->OnDestroy();
		SAFE_DELETE(mBullet);
	}
}

void CRangedWeaponScript::OnUpdate(float tDeltaTime)
{
	CWeaponScript::OnUpdate(tDeltaTime);

	// 공격 쿨타임 누적
	mTotalTime += tDeltaTime;

	switch (mState) {
	case eState::Idle:
		// 부모 클래스(CWeaponScript)의 기능을 활용해 타겟팅 및 위치 동기화 수행
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		CalculatePosNextToTarget();

		// 발사 조건 체크
		CanAttackCheck(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		break;
	default:
		break;
	}
}

void CRangedWeaponScript::OnLateUpdate(float tDeltaTime)
{
	CWeaponScript::OnLateUpdate(tDeltaTime);
}

void CRangedWeaponScript::Render(HDC tHDC)
{
	CWeaponScript::Render(tHDC);
}

void CRangedWeaponScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther)
{
}

void CRangedWeaponScript::OnCollisionStay(float tDeltaTime, CCollider* tOther)
{
}

void CRangedWeaponScript::OnCollisionExit(float tDeltaTime, CCollider* tOther)
{
}

// 공격 가능 여부 판단 및 투사체 발사
void CRangedWeaponScript::CanAttackCheck(std::vector<GameObject*> tEnemies)
{
	// 플레이어의 현재 스탯(사거리, 공격속도) 가져오기
	CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);

	float range = plSc->GetRange();
	int attackSpeed = plSc->GetAttackSpeedPercent();

	// 적이 없거나 타겟팅된 적이 유효하지 않으면 리턴
	if (tEnemies.empty()) {
		return;
	}

	if (GetClosedEnemyPos().mX == 9999.0f && GetClosedEnemyPos().mY == 9999.0f) {
		return;
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 타겟과의 거리 계산
	float distanceToEnemy = (ObjectCenterPos(GetOwner()) - GetClosedEnemyPos()).Length();

	// 1. 사거리 내 진입 && 2. 쿨타임 완료 (공격 속도 반영 공식 적용)
	if (distanceToEnemy <= GetRange() + range && mTotalTime > GetDelay() / (1 + attackSpeed / 100.0f)) {
		mTotalTime = 0.0f;

		// 미리 만들어둔 총알 견본(mBullet)의 위치와 속도만 현재 상황에 맞게 수정
		CTransform* blTr = mBullet->GetComponent<CTransform>(eComponentType::Transform);

		blTr->SetPos(tr->GetPos()); // 무기 위치에서 발사
		// 방향 벡터 정규화(Normalize) * 탄속
		blTr->SetVelocity((GetClosedEnemyPos() - tr->GetPos()).Normalize() * GetSpeed());

		// 견본 객체를 복사하여 실제 게임 월드에 투입할 인스턴스 생성
		CBullet* bullet = new CBullet();
		*bullet = *mBullet;		// 깊은 복사 (Deep Copy) 또는 대입 연산자를 통해 컴포넌트 정보 복제
		AddObjectToScene<CBullet>(bullet);	// 씬에 등록하여 업데이트 및 렌더링 시작
	};
}

// 무기 획득 시 한 번 호출되어 '총알 견본'을 생성
void CRangedWeaponScript::SetBullet(SVector2D tSize, SVector2D tColliderSize, const std::wstring& tTextureName)
{
	// 템플릿용 총알 객체 생성 (씬에 등록하지 않음)
	mBullet = new CBullet();
	mBullet->SetLayerType(eLayerType::Bullet);

	// 스크립트 및 스탯 설정
	CBulletScript* blSc = mBullet->GetComponent<CBulletScript>(eComponentType::Script);

	blSc->SetWeapon(this->GetOwner());
	blSc->SetDamage(GetDamage());

	// 물리 및 렌더링 컴포넌트 부착 (미리 해둠으로써 런타임 부하 감소)
	CCircleCollider2D* cBlCollider = mBullet->AddComponent<CCircleCollider2D>();
	cBlCollider->SetSize(tColliderSize);

	CTexture* blImg = CResourceMgr::Find<CTexture>(tTextureName);

	CSpriteRenderer* blSr = mBullet->AddComponent<CSpriteRenderer>();
	blSr->SetTexture(blImg);

	mBullet->SetSize(tSize);
	// 중심점을 기준으로 회전 및 위치 설정을 위해 Anchor 조정
	mBullet->SetAnchorPoint(blImg->GetWidth() / 2, blImg->GetHeight() / 2);
}
