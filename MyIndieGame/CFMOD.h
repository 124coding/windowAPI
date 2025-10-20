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

// ���� mSystem�� ����� create ���� �ʴ� �� ����. �׷��� audioSource�� �θ��� ����.

// ��ǥ ����� ������ ��ǥ��� �޸� -1 ~ 1�� ���� ��ǥ�踦 ����ϹǷ� ����ó�� audio���� �ڵ���� �ۼ��Ǹ� �ʹ� �ְ� ��

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

