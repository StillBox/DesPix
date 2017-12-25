#pragma once
#ifndef SCENETETRIS_H
#define SCENETETRIS_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"
#include <vector>

class SceneTetris :public GameScene
{
public:
	SceneTetris();
	~SceneTetris();

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

	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();
	bool Rotate();
	bool SetPos(int x, int y, int iRot);

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
	int m_ixField;
	int m_iyField;
	int m_cxField;
	int m_cyField;
	int m_iTarScore;
	float m_timePeriod;

	//Counter

	int m_iStage;
	int m_iScore;
	float m_timeUpdate;

	//Control

	bool m_bKeyUp[8];
	float m_fKeyUp[8];
	float m_fRate;

	//Piece
	int m_xPiece[7][4][4] =
	{
		{ { 0,1,2,3 },{ 1,1,1,1 },{ 0,1,2,3 },{ 1,1,1,1 } },			//I
		{ { 0,0,1,2 },{ 0,0,0,1 },{ 0,1,2,2 },{ 0,1,1,1 } },			//J
		{ { 0,1,2,2 },{ 1,1,1,2 },{ 0,0,1,2 },{ 0,1,1,1 } },			//L
		{ { 0,1,1,2 },{ 1,1,0,0 },{ 0,1,1,2 },{ 1,1,0,0 } },			//Z
		{ { 2,1,1,0 },{ 0,0,1,1 },{ 2,1,1,0 },{ 0,0,1,1 } },			//S
		{ { 1,0,1,2 },{ 2,1,1,1 },{ 0,1,2,1 },{ 0,1,1,1 } },			//T
		{ { 1,2,2,1 },{ 1,2,2,1 },{ 1,2,2,1 },{ 1,2,2,1 } },			//O
	};
	int m_yPiece[7][4][4] =
	{
		{ { 0,0,0,0 },{ -2,-1,0,1 },{ 0,0,0,0 },{ -2,-1,0,1 } },		//I
		{ { 0,1,1,1 },{ -1,0,1,-1 },{ 0,0,0,1 },{ 1,-1,0,1 } },			//J
		{ { 1,1,0,1 },{ -1,0,1,1 },{ 0,1,0,0 },{ 0,0,1,2 } },			//L
		{ { 0,0,1,1 },{ -1,0,0,1 },{ 0,0,1,1 },{ -1,0,0,1 } },			//Z
		{ { 0,0,1,1 },{ -1,0,0,1 },{ 0,0,1,1 },{ -1,0,0,1 } },			//S
		{ { 0,1,1,1 },{ 1,0,1,2 },{ 1,1,1,2 },{ 1,0,1,2 } },			//T
		{ { 0,0,1,1 },{ 0,0,1,1 },{ 0,0,1,1 },{ 0,0,1,1 } },			//O
	};
	D3DXCOLOR m_colorPiece[13][2] =
	{
		{ MAGE, MAGE_D },
		{ MONK, MONK_D },
		{ DRUID, DRUID_D },
		{ ROGUE, ROGUE_D },
		{ HUNTER, HUNTER_D },
		{ PRIEST, PRIEST_D },
		{ SHAMAN, SHAMAN_D },
		{ DKNIGHT, DKNIGHT_D },
		{ DHUNTER, DHUNTER_D },
		{ PALADIN, PALADIN_D },
		{ WARLOCK, WARLOCK_D },
		{ WARRIOR, WARRIOR_D },
		{ GRAY(14), GRAY(14) }
	};

	int m_iColorStock[256][256];
	int m_iColorPreStock[256][256];

	bool m_bNewPiece;
	POINT m_posCur;
	int m_iRotCur;
	int m_iTypeCur;
	int m_iColorCur;

	int m_iTypeNext;
	int m_iColorNext;

	std::vector<int> m_vecClear;
	bool m_bClear;

	//Display

	bool m_bRefresh;
	bool m_bHighFPS;
	bool m_bTraject;

	RECT m_rcField;
	RECT m_rcNext;

	//Text

	RECT m_rcScreen;
	RECT m_rcTextLeft;
	RECT m_rcTextRTop;
	RECT m_rcTextRBottom;	
	
	std::vector<ID3DXFont*> m_vecFont = { NULL, NULL };

	//Sound

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	bool m_bBGMPlay;
	int m_iSECount;
};

#endif // !SCENETETRIS_H
