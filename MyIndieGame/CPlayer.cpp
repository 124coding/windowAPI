#include "CPlayer.h"

void CPlayer::OnCreate()
{
	GameObject::OnCreate();
}

void CPlayer::OnDestroy() {
	GameObject::OnDestroy();
}

void CPlayer::OnUpdate(float tDeltaTime)
{
	GameObject::OnUpdate(tDeltaTime);
}

void CPlayer::OnLateUpdate(float tDeltaTime)
{
	GameObject::OnLateUpdate(tDeltaTime);
}

void CPlayer::Render(HDC tHDC)
{
	GameObject::Render(tHDC);

}

void CPlayer::IncreaseHP(int tIncreaseAmount) {
	mHP = mHP + tIncreaseAmount;

	if (mHP > mMaxHP) {
		mHP = mMaxHP;
	}
}

void CPlayer::DecreaseHP(int tDecreaseAmount) {
	mHP = mHP - tDecreaseAmount;

	if (mHP < 0) {
		mHP = 0;
	}
}

void CPlayer::IncreaseMaxHP(int tIncreaseAmount)
{
	mMaxHP = mMaxHP + tIncreaseAmount;
}

void CPlayer::DecreaseMaxHP(int tDecreaseAmount)
{
	mMaxHP = mMaxHP - tDecreaseAmount;

	if (mMaxHP < 0) {
		mMaxHP = 0;
	}
}
