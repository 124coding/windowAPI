#include "CPlayer.h"
#include "CPlayerScript.h"

void CPlayer::OnCreate()
{
	GameObject::OnCreate();
	AddComponent<CPlayerScript>();
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
