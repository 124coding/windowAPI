#pragma once

#include "CComponent.h"

#include "CAudioClip.h"

class CAudioSource : public CComponent {
public:
	CAudioSource() : CComponent(eComponentType::AudioSource) {}
	virtual ~CAudioSource() {}

	virtual CComponent* Clone() override {
		return new CAudioSource(*this);
	}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void Play();
	void Stop();
	void SetLoop(bool tLoop);

	void SetClip(CAudioClip* tClip) {
		this->mAudioClip = tClip;
	}
	
	CAudioClip* GetClip() {
		return this->mAudioClip;
	}

private:
	CAudioClip* mAudioClip;
};