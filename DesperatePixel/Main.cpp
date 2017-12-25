#include "Game.h"
#include "SaveData.h"
#include "Settings.h"
#include "GameMenu.h"
#include "resource.h"

#define GAME_TITLE L"¾øÍûÏñËØ"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	SetCallbackMsgProc(OnMsgProc);
	SetCallbackCreateDevice(OnCreateDevice);
	SetCallbackResetDevice(OnResetDevice);
	SetCallbackLostDevice(OnLostDevice);
	SetCallbackDestroyDevice(OnDestroyDevice);
	SetCallbackD3DFrameRender(OnFrameRender);

	srand(timeGetTime());
	InitSaveData();
	InitSettings();
	InitGameMenu();

	DXUTCreateWindow(GAME_TITLE, 
		WS_OVERLAPPEDWINDOW, 
		hInstance, 
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DP)) , 
		GetGameMenu().hMenu, 
		GetSettings()._cxWindow, GetSettings()._cyWindow);
	DXUTCreateDevice(GetSettings()._cxDevice, GetSettings()._cyDevice);

	InitGame();

	DXUTMainLoop(GetGameMenu().hAccel);

	return DXUTGetExitCode();
}