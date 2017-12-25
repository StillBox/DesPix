#pragma once
#ifndef SCENERACING_H
#define SCENERACING_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"
#include <vector>

class SceneRacing :public GameScene
{
public:
	SceneRacing();
	~SceneRacing();

	void Init(int iInitVal = 1);
	void End();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
	void Refresh();

private:
	void InitField();
	void InitStage();

private:
	//GameState

	bool m_bSceneOver;
	bool m_bFieldInit;
	bool m_bStageInit;
	bool m_bStageShow;
	bool m_bPlay;
	bool m_bWin;
	bool m_bLose;
	bool m_bGuide;
	bool m_bGuideOver;
	float m_timeShow;

	int m_iSize;
	int m_cWidth;
	int m_cEnemyGap;
	int m_iPlayerHeight;

	//Counter

	int m_iStage;

	float m_fSignRoll;
	float m_fEnemyRoll;
	float m_cEnemy;
	float m_iGapEnemy;

	int m_iCounter;

	//Control

	bool m_bKeyUp[8];

	float m_timeUpdate;
	float m_fRollSpeed;
	float m_fEnemySpeed;
	float m_fRate;

	int m_iLPlayer;
	float m_fVPlayer;
	float m_fSPlayer;
	
	//Display

	bool m_bRefresh;
	bool m_bHighFPS;
	bool m_bTraject;

	RECT m_rcField;
	RECT m_rcLSign;
	RECT m_rcRSign;
	RECT m_rcProc;

	RECT m_rcPlayer;

	std::vector<POINT> m_vecSign;
	std::vector<POINT> m_vecEnemy;

	RECT m_rcScreen;
	RECT m_rcTextCenter;
	RECT m_rcTextLTop;
	RECT m_rcTextLBottom;

	std::vector<ID3DXFont*> m_vecFont = { NULL, NULL };

	//Sound

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	bool m_bBGMPlay;
};


#endif // !SCENERACING_H
