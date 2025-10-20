#include "CFMOD.h"
#include "winMacro.h"

FMOD::Studio::System* CFMOD::mSystem = nullptr;
FMOD::System* CFMOD::mCoreSystem = nullptr;

void CFMOD::OnCreate() {
	void* extraDriverData = nullptr;

	FMOD::Studio::System::create(&mSystem);

	mSystem->getCoreSystem(&mCoreSystem);
	mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

	mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
}

void CFMOD::OnDestroy() {
	SAFE_DELETE(mSystem);
}

bool CFMOD::CreateSound(const std::string& tPath, FMOD::Sound** tSound) {
	if (FMOD_OK != mCoreSystem->createSound(tPath.c_str(), FMOD_3D, 0, tSound))
		return false;

	return true;
}

void CFMOD::SoundPlay(FMOD::Sound* tSound, FMOD::Channel** tChannel) {
	mCoreSystem->playSound(tSound, 0, false, tChannel);
}

void CFMOD::Set2DListenerAttributes(const SVector2D* tPos) {
	FMOD_VECTOR fmodPos(tPos->mX, tPos->mY, 0.0f);
	FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);
	FMOD_VECTOR fmodForward(0.0f, 0.0f, 1.0f);
	FMOD_VECTOR fmodUp(0.0f, 1.0f, 0.0f);

	mCoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
}
