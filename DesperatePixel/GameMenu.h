#pragma once
#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "DXUT.h"
#include "Game.h"
#include "Settings.h"

#define	IDM_GAME_TITLE		1001

#define IDM_GAME_PONG_S0	1010
#define IDM_GAME_PONG_S1	1011
#define IDM_GAME_PONG_S2	1012
#define IDM_GAME_PONG_S3	1013
#define IDM_GAME_PONG_S4	1014
#define IDM_GAME_PONG_S5	1015

#define IDM_GAME_RACING_S0	1020
#define IDM_GAME_RACING_S1	1021
#define IDM_GAME_RACING_S2	1022
#define IDM_GAME_RACING_S3	1023
#define IDM_GAME_RACING_S4	1024
#define IDM_GAME_RACING_S5	1025

#define IDM_GAME_TETRIS_S0	1030
#define IDM_GAME_TETRIS_S1	1031
#define IDM_GAME_TETRIS_S2	1032
#define IDM_GAME_TETRIS_S3	1033
#define IDM_GAME_TETRIS_S4	1034
#define IDM_GAME_TETRIS_S5	1035
#define IDM_GAME_TETRIS_S6	1036

#define IDM_GAME_SNAKE_S0	1040
#define IDM_GAME_SNAKE_S1	1041
#define IDM_GAME_SNAKE_S2	1042
#define IDM_GAME_SNAKE_S3	1043
#define IDM_GAME_SNAKE_S4	1044
#define IDM_GAME_SNAKE_S5	1045
#define IDM_GAME_SNAKE_S6	1046
#define IDM_GAME_SNAKE_S7	1047

#define IDM_GAME_EXTRA		1050

#define IDM_GAME_PAUSE		1101
#define IDM_GAME_CONTINUE	1102
#define IDM_GAME_RESET		1103

#define IDM_GAME_EXIT		1104

#define IDM_DISPLAY_SIZE1	2001
#define IDM_DISPLAY_SIZE2	2002
#define IDM_DISPLAY_SIZE3	2003
#define IDM_DISPLAY_SIZE4	2004
#define IDM_DISPLAY_SIZE5	2005

#define IDM_DISPLAY_FRAME	2101
#define IDM_DISPLAY_TEX		2102
#define IDM_DISPLAY_FPS		2103
#define IDM_DISPLAY_SPEED	2104
#define IDM_DISPLAY_CONT	2105

#define IDM_SOUND_BGMOFF	3000		
#define IDM_SOUND_BGM1		3001		
#define IDM_SOUND_BGM2		3002		
#define IDM_SOUND_BGM3		3003		
#define IDM_SOUND_BGM4		3004		
#define IDM_SOUND_BGM5		3005		
#define IDM_SOUND_BGM6		3006		
#define IDM_SOUND_BGM7		3007		
#define IDM_SOUND_BGM8		3008		
#define IDM_SOUND_BGM9		3009		
#define IDM_SOUND_BGM0		3010		

#define IDM_SOUND_SEOFF		3100		
#define IDM_SOUND_SE1		3101		
#define IDM_SOUND_SE2		3102		
#define IDM_SOUND_SE3		3103		
#define IDM_SOUND_SE4		3104		
#define IDM_SOUND_SE5		3105		
#define IDM_SOUND_SE6		3106		
#define IDM_SOUND_SE7		3107		
#define IDM_SOUND_SE8		3108		
#define IDM_SOUND_SE9		3109		
#define IDM_SOUND_SE0		3110		

#define IDM_SOUND_OFF		3201

#define IDM_LANG_EN			4001
#define IDM_LANG_CH			4002
#define IDM_OTHER_SKIP		4010

struct GameMenu
{
	HMENU hMenu = NULL;
	HACCEL hAccel = NULL;

	//Display

	UINT iSize = IDM_DISPLAY_SIZE1;
	bool bFrame = false;
	bool bTex = false;
	bool bShowFPS = false;
	bool bHighFPS = false;
	bool bContinuous = false;

	//Sound

	UINT iBGMVol = IDM_SOUND_BGM0;
	UINT iSEVol = IDM_SOUND_SE0;
	bool bMute = false;
	bool bBGMOff = false;
	bool bSEOff = false;

	//Ohter
	int iLanguage = IDM_LANG_EN;
	bool bSkipGuide = false;
};

GameMenu &GetGameMenu();
BOOL InitGameMenu();
void MenuOnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);

#endif // !GAMEMENU_H
