#pragma once

#include "CFMOD.h"
#include "CResource.h"

class CAudioClip : public CResource
{
public:
	CAudioClip() : CResource(eResourceType::AudioClip), mSound(nullptr), mChannel(nullptr) {}
	virtual ~CAudioClip() {}

	virtual HRESULT Load(CAPIEngine* tEngine, const std::wstring& tPath) override;

private:
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
};

