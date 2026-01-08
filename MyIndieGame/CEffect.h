#pragma once

#include "GameObject.h"

class CEffect : public GameObject
{
public:
	CEffect() {}
	virtual ~CEffect() {}

	CEffect(const CEffect& tEffect);
	CEffect& operator=(const CEffect& tEffect);

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(float tDeltaTime) override;
	virtual void OnLateUpdate(float tDeltaTime) override;
	virtual void Render(HDC tHDC) override;

	// 이펙트 재사용 시 상태를 초기화하는 순수 가상 함수
	// 파생 클래스(데미지 텍스트, 애니메이션 이펙트 등)에서 반드시 구현해야 함
	virtual void Reset(SVector2D tPos) = 0;

	// 이펙트를 유발한 주체(Owner) 설정
	void SetOwner(GameObject* tObj) {
		this->mOwnerObj = tObj;
	}

	// 이펙트가 재생되는 도중에 주인이 사망(Dead)하거나 삭제될 수 있음
	// 댕글링 포인터 접근을 막기 위해 유효성 검사를 수행한 후 반환
	GameObject* GetOwner() {
		if (mOwnerObj == nullptr || mOwnerObj->IsDead()) {
			return nullptr;
		}

		return this->mOwnerObj;
	}

protected:
	GameObject* mOwnerObj;	// 이펙트의 주인 (참조용, 소유권 없음)
	SVector2D mStartPos;	// 생성 위치

	// 일정 시간 후 자동으로 사라지게 하기 위한 타이머 변수들
	float mLifeTime = 0.0f;	// 지속 시간
	float mCurTime = 0.0f;	// 경과 시간
};