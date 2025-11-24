#pragma once

#include "CFMOD.h"
#include "CResource.h"

#include "winMacro.h"

class CAudioClip : public CResource
{
public:
	CAudioClip() : 
		CResource(eResourceType::AudioClip), 
		mSound(nullptr), 
		mChannel(nullptr),
		mMinDistance(1.0f),
		mMaxDistance(1000.0f),
		mbLoop(false)
	{}
	virtual ~CAudioClip() {
		SAFE_DELETE(mSound);
	}

	virtual HRESULT Load(const std::wstring& tPath) override;

	void Play();
	void Stop();
	void Set2DAttributes(const SVector2D tPos);
	void SetLoop(bool tLoop) { 
		mbLoop = tLoop; 
	}

private:
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;

	float mMinDistance;
	float mMaxDistance;
	bool mbLoop;
};

