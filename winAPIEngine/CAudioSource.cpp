#include "CAudioSource.h"

#include "GameObject.h"

#include "CFMOD.h"

#include "CTransform.h"

void CAudioSource::OnCreate() {

}
void CAudioSource::OnDestroy() {

}

void CAudioSource::OnUpdate(float tDeltaTime) {

}

void CAudioSource::OnLateUpdate(float tDeltaTime) {
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	SVector2D pos = tr->GetPos();

	mAudioClip->Set2DAttributes(pos);
}

void CAudioSource::Render(HDC tHDC) {

}

void CAudioSource::Play() {
	mAudioClip->Play();
}

void CAudioSource::Stop() {
	mAudioClip->Stop();
}

void CAudioSource::SetLoop(bool tLoop) {
	mAudioClip->SetLoop(tLoop);
}