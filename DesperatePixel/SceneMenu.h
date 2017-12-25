#pragma once
#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"
#include <vector>

enum BUTTONSTATE
{
	NORMAL,
	FOCUS,
	PRESSED,
	HIDDEN
};

class SceneMenu :public GameScene
{
public:
	SceneMenu();
	~SceneMenu();

	void Init(int iInitVal = 1);
	void End();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
	void Refresh() {}

private:
	void SetText();

	void SetButton(int iMenu, int iButton, BUTTONSTATE state, bool bRefresh = true);
	void SetButton(BUTTONSTATE state, bool bRefresh = true);
	void SetMenu(int iMenu);
	void SetMenu();
	void SetStage(int iStage, bool bOn);

private:
	int m_iMenu;
	int m_iFocus;
	int m_iStage;

	bool m_bChange;
	bool m_bMenuChange;

	GAME_SCENE m_nextScene;

	//Counter

	int m_iUnlock;

	bool m_bKeyUp[2];
	float m_fKeyUp[2];

	//Title

	RECT m_rcTitle;
	std::vector<PIX> m_vecTitle;

	//Menu
	
	RECT m_rcMenu;

	//Main Menu

	RECT m_rcGame;
	std::vector<PIX> m_vecGame;

//	RECT m_rcSetting;
//	std::vector<PIX> m_vecSetting;

	RECT m_rcExtra;
	std::vector<PIX> m_vecExtra;

	RECT m_rcQuit;
	std::vector<PIX> m_vecQuit;

	//GameSelect

	RECT m_rcPong;
	std::vector<PIX> m_vecPong;

	RECT m_rcRacing;
	std::vector<PIX> m_vecRacing;

	RECT m_rcTetris;
	std::vector<PIX> m_vecTetris;

	RECT m_rcSnake;
	std::vector<PIX> m_vecSnake;
	
//	//Setting
//
//	RECT m_rcVolumn;
//	std::vector<PIX> m_vecVolumn;
//
//	RECT m_rcMasterVol;
//	std::vector<PIX> m_vecMasterVol;
//
//	RECT m_rcBGMVol;
//	std::vector<PIX> m_vecBGMVol;
//
//	RECT m_rcSEVol;
//	std::vector<PIX> m_vecSEVol;
//
//	RECT m_rcDisplay;
//	std::vector<PIX> m_vecDisplay;
//
//	RECT m_rcPixSize;
//	std::vector<PIX> m_vecPixSize;
//
//	RECT m_rcFrame;
//	std::vector<PIX> m_vecFrame;
//
//	RECT m_rcTexture;
//	std::vector<PIX> m_vecTexture;
//
//	RECT m_rcOther;
//	std::vector<PIX> m_vecOther;
//
//	RECT m_rcLanguage;
//	std::vector<PIX> m_vecLanguage;

	//Other

	RECT m_rcDespair;
	float m_timeUpdateDespair;

	std::vector<PIX> m_vecDespair;
	std::vector<int> m_vecDespairOffset;
	
	//Sound

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	bool m_bBGMPlay;
};

#endif // !SCENEMENU_H
