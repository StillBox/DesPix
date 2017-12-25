#include "Game.h"
#include "DXInput.h"
#include "DXAudio.h"
#include "DXColor.h"

#include "Settings.h"
#include "GameMenu.h"

#include "GameScrn.h"
#include "GameAudio.h"
#include "GameScene.h"
#include "SceneOP.h"
#include "SceneMenu.h"
#include "ScenePong.h"
#include "SceneRacing.h"
#include "SceneTetris.h"
#include "SceneSnake.h"
#include "SceneExtra.h"

using std::vector;

GAME_STATE g_GameState;
BOOL g_IsKeyUp[256];

LPD3DXFONT g_pSysInfoText = NULL;

Sound *g_pMenuBGM = NULL;
Sound *g_pPlayBGM = NULL;

GameScene *g_pScene = NULL;

HRESULT InitScene();
void UpdateScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);
void RenderScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);

LRESULT CALLBACK OnMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITMENUPOPUP:
	case WM_COMMAND:
		MenuOnMsgProc(message, wParam, lParam);
		break;

	default:
		break;
	}
	return 0;
}

HRESULT CALLBACK OnCreateDevice()
{
	InitInput(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	
	InitDXAudio();
	SetMasterVolumn(1.0f * GetSettings()._iSoundOn);
	if (GetSettings()._iBGMOn)
		SetSubmixVolumn(ID_SUBMIX_BACKGROUND, 0.1f * GetSettings()._iBGMVol);
	else
		SetSubmixVolumn(ID_SUBMIX_BACKGROUND, 0.0f);
	if (GetSettings()._iSEOn)
		SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, 0.1f * GetSettings()._iSEVol);
	else		
		SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, 0.0f);
	
	GetScrn().Init();

	D3DXCreateFont(DXUTGetD3DDevice(), min(GetSettings()._iPixSize * 11, 22), 0, 0, 1000, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &g_pSysInfoText);

	return S_OK;
}

HRESULT CALLBACK OnResetDevice()
{
	SAFE_RELEASE(g_pSysInfoText);
	D3DXCreateFont(DXUTGetD3DDevice(), min(GetSettings()._iPixSize * 11, 22), 0, 0, 1000, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &g_pSysInfoText);

	if (g_GameState.m_GameMode == GAME_PLAY)
		g_GameState.m_GameMode = GAME_PAUSE;

	GetScrn().OnResetDevice();

	if (NULL != g_pScene)
		g_pScene->OnResetDevice();

	return S_OK;
}

HRESULT CALLBACK OnLostDevice()
{
	SAFE_RELEASE(g_pSysInfoText);

	if (g_GameState.m_GameMode == GAME_PLAY)
		g_GameState.m_GameMode = GAME_PAUSE;

	if (NULL != g_pScene)
		g_pScene->OnLostDevice();

	return S_OK;
}

HRESULT CALLBACK OnDestroyDevice()
{
	HRESULT hr;

	GetScrn().End();

	HR(hr, ReleaseInput());
	HR(hr, ReleaseDXAudio());

	SAFE_RELEASE(g_pSysInfoText);

	return S_OK;
}

HRESULT InitGame()
{
	SetScene(OP);
	SetMode(GAME_MENU);

	return S_OK;
}

void CALLBACK OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	if (!g_GameState.m_bSceneInit)
	{
		InitScene();
		g_GameState.m_bSceneInit = true;
		return;
	}

	if (!g_GameState.m_bUpdate)
	{
		g_GameState.m_bUpdate = true;
		return;
	}

	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, GRAY(1), 1.0f, 0);

	pd3dDevice->BeginScene();
	
	if (fElapsedTime < 500.0f)
	{
		UpdateScene(pd3dDevice, fTime, fElapsedTime);
		RenderScene(pd3dDevice, fTime, fElapsedTime);
	}
	
	RECT rect;
	SetRect(&rect, 0, 0, DXUTGetDeviceWidth(), DXUTGetDeviceHeight());
	TCHAR szBuffer[20];

	if (GetSettings()._iShowFPS)
	{
		g_pSysInfoText->DrawText(NULL, szBuffer, swprintf_s(szBuffer, TEXT("FPS:%0.2f"), GetRealtimeFPS()), &rect,
			DT_TOP | DT_RIGHT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	}

	pd3dDevice->EndScene();
}

HRESULT InitScene()
{
	if (NULL != g_pScene)
	{
		g_pScene->End();
		SAFE_DELETE(g_pScene);
	}

	switch (g_GameState.m_GameScene)
	{
	case OP:
		g_pScene = new SceneOP();
		break;

	case MENU:
		g_pScene = new SceneMenu();
		break;

	case PONG:
		g_pScene = new ScenePong();
		break;

	case RACING:
		g_pScene = new SceneRacing();
		break;

	case TETRIS:
		g_pScene = new SceneTetris();
		break;

	case SNAKE:
		g_pScene = new SceneSnake();
		break;

	case EXTRA:
		g_pScene = new SceneExtra();
		break;

	default:
		break;
	}

	g_pScene->Init(GetInitVal());

	g_GameState.m_bSceneInit = true;

	return S_OK;
}

void UpdateScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	g_pScene->Update(fTime, fElapsedTime);
}

void RenderScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	g_pScene->Render(fTime, fElapsedTime);
}

void SetInitVal(int iVal)
{
	g_GameState.m_iInitVal = iVal;
}

int GetInitVal()
{
	return g_GameState.m_iInitVal;
}

void SetMode(GAME_MODE mode)
{
	g_GameState.m_GameMode = mode;
}

GAME_MODE GetMode()
{
	return g_GameState.m_GameMode;
}

void SetScene(GAME_SCENE scene)
{
	g_GameState.m_GameScene = scene;
	g_GameState.m_bSceneInit = false;
	g_GameState.m_bUpdate = false;
}

GAME_SCENE GetScene()
{
	return g_GameState.m_GameScene;
}

void QuitGame()
{
	DestroyWindow(DXUTGetHWnd());
}