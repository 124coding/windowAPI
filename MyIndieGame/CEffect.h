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

	virtual void Reset(SVector2D tPos) = 0;

	void SetOwner(GameObject* tObj) {
		this->mOwnerObj = tObj;
	}

	GameObject* GetOwner() {
		if (mOwnerObj == nullptr || mOwnerObj->IsDead()) {
			return nullptr;
		}

		return this->mOwnerObj;
	}

protected:
	GameObject* mOwnerObj;
	SVector2D mStartPos;

	float mLifeTime = 0.0f;
	float mCurTime = 0.0f;
};