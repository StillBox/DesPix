#pragma once

#include <vector>
#include "DXUT.h"

#define CXPIX 240
#define CYPIX 180

enum GAME_MODE
{
	LOADING = 0,
	GAME_MENU,
	GAME_PLAY,
	GAME_PAUSE,
	GAME_OVER
};

enum GAME_SCENE
{
	OP,
	MENU,
	PONG,
	RACING,
	TETRIS,
	SNAKE,
	EXTRA
};

struct GAME_STATE
{
	bool m_bSceneInit = false;
	bool m_bUpdate = true;
	int m_iInitVal = 0;

	GAME_MODE m_GameMode;
	GAME_SCENE m_GameScene;
};

HRESULT InitGame();

LRESULT CALLBACK OnMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT CALLBACK OnCreateDevice();
HRESULT CALLBACK OnResetDevice();
HRESULT CALLBACK OnLostDevice();
HRESULT CALLBACK OnDestroyDevice();
void CALLBACK OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);

void SetInitVal(int iVal);
int GetInitVal();
void SetMode(GAME_MODE mode);
GAME_MODE GetMode();
void SetScene(GAME_SCENE scene);
GAME_SCENE GetScene();
void QuitGame();