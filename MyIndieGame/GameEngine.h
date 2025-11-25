#pragma once

#include "CAPIEngine.h"
#include "GameObject.h"
#include "CSceneMgr.h"

class GameEngine : public CAPIEngine {
private:


public:
	GameEngine(){}
	virtual ~GameEngine(){}

	GameEngine(const GameEngine& t) = delete;
	GameEngine operator=(const GameEngine& t) = delete;

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float tDeltaTime) override;
	void OnLateUpdate(float tDeltaTime) override;
	void Render() override;

	void LoadScenes();
	void LoadResources();
	void LoadMonster(std::wstring tName, std::wstring tPath);

	void DestroyScenes();
	void DestroyResources();

	void RemoveDeadObjects();
};