#include "CFMOD.h"

FMOD::Studio::System* CFMOD::mSystem = nullptr;
FMOD::System* CFMOD::mCoreSystem = nullptr;

void CFMOD::OnCreate() {
	void* extraDriverData = nullptr;

	FMOD::Studio::System::create(&mSystem);

	mSystem->getCoreSystem(&mCoreSystem);
	mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
}

bool CFMOD::CreateSound(const std::string& tPath, FMOD::Sound* tSound) {
	if (FMOD_OK != mCoreSystem->createSound(tPath.c_str(), FMOD_3D, 0, &tSound))
		return false;

	return true;
}

void CFMOD::SoundPlay(FMOD::Sound* tSound, FMOD::Channel** tChannel) {
	mCoreSystem->playSound(tSound, 0, false, tChannel);
}