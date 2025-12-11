#pragma once

#include "CUIPanel.h"
#include "CUIText.h"
#include "CUIImg.h"


CUIPanel* MakeStat(float tWidth, float tHeight, std::wstring tTexName, std::wstring tName, CUIText*& tSaveUITex);

void SettingStatTex(int tValue, CUIText* tTexUI);