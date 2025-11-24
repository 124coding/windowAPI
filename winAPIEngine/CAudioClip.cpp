#include "CAudioClip.h"
#include "CAudioSource.h"

HRESULT CAudioClip::Load(const std::wstring& tPath)
{
    std::string cPath(tPath.begin(), tPath.end());
    if (!CFMOD::CreateSound(cPath, &mSound))
        return S_FALSE;

    mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

    return E_NOTIMPL;
}

void CAudioSource::OnCreate()
{
}

void CAudioSource::OnDestroy()
{
}

void CAudioSource::OnUpdate(float tDeltaTime)
{
}

void CAudioSource::OnLateUpdate(float tDeltaTime)
{
}

void CAudioSource::Render(HDC tHDC)
{
}

void CAudioClip::Play() {
    if (mbLoop) {
        mSound->setMode(FMOD_LOOP_NORMAL);
    }
    else {
        mSound->setMode(FMOD_LOOP_OFF);
    }

    CFMOD::SoundPlay(mSound, &mChannel);
}

void CAudioClip::Stop() {
    mChannel->stop();
}

void CAudioClip::Set2DAttributes(const SVector2D tPos) {
    FMOD_VECTOR fmodPos(tPos.mX, tPos.mY, 0.0f);
    FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);

    mChannel->set3DAttributes(&fmodPos, &fmodVel);
}