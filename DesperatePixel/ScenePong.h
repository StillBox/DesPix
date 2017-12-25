#pragma once
#ifndef SCENEPONG_H
#define SCENEPONG_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"

class ScenePong :public GameScene
{
public:
	ScenePong();
	~ScenePong();

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

	void RandBallSpeed(int u, int v);

private:
	//GameState

	bool m_bSceneOver;
	bool m_bFieldInit;
	bool m_bStageInit;
	bool m_bStageShow;
	bool m_bRoundShow;
	bool m_bPlay;
	bool m_bServe;
	bool m_bWin;
	bool m_bLose;
	bool m_bGuide;
	bool m_bGuideOver;
	float m_timeShow;

	int m_iSize;
	int m_iBound;
	int m_iTarScore;

	//Counter

	int m_iStage;
	int m_iRound;

	int m_iScorePlayer;
	int m_iScoreCom;

	//Control

	bool m_bKeyUp[8];
	
	float m_timeUpdate;
	float m_fRate;

	int m_iLPlayer;
	float m_fVPlayer;
	float m_fSPlayer;

	int m_iLCom;
	float m_fVCom;
	float m_fSCom;

	float m_fUBall;
	float m_fVBall;
	float m_fxBall;
	float m_fyBall;
	
	//Display

	bool m_bRefresh;
	bool m_bHighFPS;
	bool m_bTraject;

	RECT m_rcField;

	RECT m_rcPlayer;
	RECT m_rcCom;
	RECT m_rcBall;

	RECT m_rcScreen;
	RECT m_rcTextTop;
	RECT m_rcTextBottom;

	std::vector<ID3DXFont*> m_vecFont = { NULL, NULL };

	//Sound

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	bool m_bBGMPlay;
};

#endif // !SCENEPONG_H
