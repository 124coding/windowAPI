#include "CSettingScene.h"

#include "Object.h"

#include "CUIMgr.h"

void CSettingScene::OnCreate()
{
	GameObject* bg = Instantiate<GameObject>(eLayerType::BackGround);
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(CResourceMgr::Find<CTexture>(L"SettingMap"));

	CScene::OnCreate();
}

void CSettingScene::OnDestroy()
{
	CScene::OnDestroy();
}

void CSettingScene::OnUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CSettingScene::OnLateUpdate(float tDeltaTime)
{
	CScene::OnUpdate(tDeltaTime);
}

void CSettingScene::Render(HDC tHDC)
{
	CScene::Render(tHDC);

	HFONT oldFont;
	HFONT font = CreateFont(70, 0, 0, 0, FW_HEAVY, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"Noto Sans KR Medium");
	oldFont = (HFONT)SelectObject(tHDC, font);

	std::wstring wstr = L"";

	if (mCurSelect == eSelect::Character) {
		wstr = L"캐릭터 선택";
	}
	else if (mCurSelect == eSelect::Weapon) {
		wstr = L"무기 선택";
	}
	else if (mCurSelect == eSelect::Difficulty) {
		wstr = L"난이도 선택";
	}

	UINT oldAlign = SetTextAlign(tHDC, TA_CENTER);

	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(tHDC, hPen);

	BeginPath(tHDC);

	TextOut(tHDC, windowWidth / 2, windowHeight / 15, wstr.c_str(), wstr.length());

	EndPath(tHDC);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = (HBRUSH)SelectObject(tHDC, hBrush);

	int oldPolyFillMode = SetPolyFillMode(tHDC, WINDING);

	StrokePath(tHDC);

	SetTextColor(tHDC, RGB(255, 255, 255));

	TextOut(tHDC, windowWidth / 2, windowHeight / 15, wstr.c_str(), wstr.length());

	SetPolyFillMode(tHDC, oldPolyFillMode);

	SelectObject(tHDC, oldBrush);
	DeleteObject(hBrush);

	SelectObject(tHDC, oldPen);
	DeleteObject(hPen);

	// SetTextColor(tHDC, RGB(255, 255, 255));
	SetBkMode(tHDC, TRANSPARENT);
	SetTextAlign(tHDC, oldAlign);
	SelectObject(tHDC, oldFont);
	DeleteObject(font);
}

void CSettingScene::OnEnter()
{
	CScene::OnEnter();

	CUIMgr::Push(eUIType::CharacterSelectUI);
	mCurSelect = eSelect::Character;
}

void CSettingScene::OnExit()
{
	CScene::OnExit();

	CUIMgr::Pop(eUIType::CharacterSelectUI);
}
