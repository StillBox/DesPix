#pragma once

#ifndef GAMEGUI_H
#define GAMEGUI_H

#include"DXGUI.h"

//
//MenuDialogs
//

#define IDD_MAIN		0
#define IDD_AUDIO		1
#define IDD_VIDEO		2
#define IDD_SELECT		3
#define IDD_PAUSE		4

//
//MainMenu
//

#define IDC_START		1
#define IDC_SETTING		2
#define IDC_QUIT		3
#define IDC_HOPE		11037

class MainMenu :public DXDialog
{
private:
	int CodeCheck;
public:
	MainMenu();
	~MainMenu();
	void Init();
	void Default();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//AudioMenu
//

#define IDC_VOL			11
#define IDC_BGM_VOL		12
#define IDC_SE_VOL		13
#define IDC_AUDIO_TAG	10

class AudioMenu :public DXDialog
{
public:
	AudioMenu();
	~AudioMenu();
	void Init();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//VideoMenu
//

#define IDC_WINSIZE		21
#define	IDC_WINDOWED	22
#define IDC_FPS			23
#define	IDC_VIDEO_TAG	20

class VideoMenu :public DXDialog
{
public:
	VideoMenu();
	~VideoMenu();
	void Init();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//GameSelect
//

#define IDC_TETRIS		101
#define IDC_SNAKE		102
#define IDC_SWEEPER		103
#define	IDC_SHOOTER		104
#define IDC_DOWNSTAIR	105

class GameSelect :public DXDialog
{
public:
	GameSelect();
	~GameSelect();
	void Init();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//PauseMenu
//

#define IDC_CONTINUE	31
#define IDC_RETRY		32
#define IDC_MAIN		33

class PauseMenu :public DXDialog
{
public:
	PauseMenu();
	~PauseMenu();
	void Init();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // !GAMEGUI_H
