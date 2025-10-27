#pragma once

#include "CComponent.h"

class CCollider : public CComponent
{
public:
	CCollider(eColliderType tType) : 
		CComponent(eComponentType::Collider), 
		mID(mCollisionID++), 
		mSize(SVector2D(1.0f, 1.0f)), 
		mType(tType) {}

	virtual ~CCollider() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
	virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);

	void SetOffset(SVector2D tOffset) {
		this->mOffset = tOffset;
	}

	SVector2D GetOffset() {
		return this->mOffset;
	}

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

private:
	static UINT32 mCollisionID;
	UINT32 mID;
	SVector2D mOffset;
	SVector2D mSize;
	eColliderType mType;
};

