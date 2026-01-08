#pragma once

#include "CComponent.h"

class CCollider : public CComponent
{
public:
	CCollider(eColliderType tType) : 
		CComponent(eComponentType::Collider), 
		mID(mCollisionID++),	// 생성될 때마다 정적 변수를 증가시켜 고유 ID 부여
		mSize(SVector2D(1.0f, 1.0f)), 
		mType(tType) {}

	virtual ~CCollider() {}

	virtual CComponent* Clone() override {
		return new CCollider(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	// ==========================================
	// Collision Events (이벤트 전파)
	// ==========================================
	// CollisionMgr가 충돌을 감지하면 이 함수들을 호출함
	// 이 함수들은 다시 자신의 주인(Owner)이 가진 'Script' 컴포넌트들의 OnCollision...을 호출하여 로직을 실행시킴
	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);

	// 충돌체 중심점 보정
	void SetOffset(SVector2D tOffset) {
		this->mOffset = tOffset;
	}

	// 충돌체 고유 ID
	SVector2D GetOffset() {
		return this->mOffset;
	}

	// 충돌체 크기
	UINT32 GetID() {
		return this->mID;
	}

	void SetSize(SVector2D tSize) {
		this->mSize = tSize;
	}

	SVector2D GetSize() {
		return this->mSize;
	}

	eColliderType GetColliderType() {
		return this->mType;
	}

	// 충돌체 활성화/비활성화
	void SetActivate(bool tActivate) {
		this->mActivate = tActivate;
	}

	bool GetActivate() {
		return this->mActivate;
	}

private:
	static UINT32 mCollisionID;		// 모든 Collider가 공유하는 정적 카운터
	UINT32 mID;						// 나의 고유 번호
	SVector2D mOffset;				// 오브젝트 중심으로부터의 거리
	SVector2D mSize;				// 충돌체의 크기
	eColliderType mType;			// Rect2D, Circle2D 등

	bool mActivate = true;			// 충돌 활성화 여부
};

