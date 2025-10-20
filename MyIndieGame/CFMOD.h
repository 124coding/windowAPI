#pragma once

#include "..\\External\\Library\\FMOD\\inc\\fmod_studio.hpp"
#include "..\\External\\Library\\FMOD\\inc\\fmod.hpp"
#include "..\\External\\Library\\FMOD\\inc\\fmod_common.h"
#include "..\\External\\Library\\FMOD\\inc\\fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\FMOD\\Debug\\fmodL_vc.lib")
#pragma comment(lib, "..\\External\\Library\\FMOD\\Debug\\fmodstudioL_vc.lib")
#else
#pragma comment(lib, "..\\External\\Library\\FMOD\\Release\\fmod_vc.lib")
#pragma comment(lib, "..\\External\\Library\\FMOD\\Release\\fmodstudio_vc.lib")
#endif

#include "SVector2D.h"

#include <string>

// 현재 mSystem이 제대로 create 되지 않는 것 같음. 그래서 audioSource를 부르지 못함.

// 좌표 계산이 윈도우 좌표계랑 달리 -1 ~ 1의 비율 좌표계를 사용하므로 현재처럼 audio관련 코드들이 작성되면 너무 멀게 됨

class CFMOD
{
public:
	static void OnCreate();
	static void OnDestroy();
	static bool CreateSound(const std:: string& tPath, FMOD::Sound** tSound);
	static void SoundPlay(FMOD::Sound* tSound, FMOD::Channel** tChannel);
	static void Set2DListenerAttributes(const SVector2D* tPos);

private:
	static FMOD::Studio::System* mSystem;
	static FMOD::System* mCoreSystem;
};

