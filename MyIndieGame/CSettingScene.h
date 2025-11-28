#pragma once

#include "CScene.h"

class CSettingScene : public CScene
{
public:
	friend class CCharacterSelectUI;

	enum class eSelect{
		Character,
		Weapon,
		Difficulty
	};

	struct SEffectArg {
		std::wstring value; 
		std::wstring color;
	};

	struct SEffect {
		std::wstring id;
		std::vector<SEffectArg> args;
		std::wstring description;
	};

	CSettingScene() {}
	virtual ~CSettingScene() {}

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render(HDC tHDC) override;

	void OnEnter() override;
	void OnExit() override;

private:
	eSelect mCurSelect = eSelect::Character;

	static std::vector<SEffect> mApplicableEffects;
};

