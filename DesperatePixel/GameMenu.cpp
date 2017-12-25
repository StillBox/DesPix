#include "GameMenu.h"
#include "GameScrn.h"
#include "GameAudio.h"
#include "SaveData.h"

GameMenu *g_pMenu = new GameMenu();

GameMenu &GetGameMenu()
{
	assert(g_pMenu != NULL);
	return *g_pMenu;
}

BOOL InitGameMenu()
{
	HMENU hMenuPop = NULL;
	HMENU hMenuPopPop = NULL;

	GetGameMenu().hMenu = CreateMenu();

	//////////////////
	//  Game Menu   //
	//////////////////

	hMenuPop = CreateMenu();
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_TITLE, L"主菜单(&M)\tM");
	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_PONG_S1, L"第&1关\tF1");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_PONG_S2, L"第&2关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_PONG_S3, L"第&3关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_PONG_S4, L"第&4关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_PONG_S5, L"第&5关");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"PONG(&G)");

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_RACING_S1, L"第&1关\tF2");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_RACING_S2, L"第&2关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_RACING_S3, L"第&3关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_RACING_S4, L"第&4关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_RACING_S5, L"第&5关");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"赛车(&A)");

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S1, L"第&1关\tF3");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S2, L"第&2关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S3, L"第&3关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S4, L"第&4关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S5, L"第&5关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_TETRIS_S6, L"第&6关");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"俄罗斯方块(&T)");

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S1, L"第&1关\tF4");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S2, L"第&2关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S3, L"第&3关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S4, L"第&4关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S5, L"第&5关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S6, L"第&6关");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_GAME_SNAKE_S7, L"第&7关");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"贪吃蛇(&S)");

	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_EXTRA, L"音乐盒(&E)");

	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_PAUSE, L"暂停(&P)\tP");
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_CONTINUE, L"继续(&C)\tC");
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_RESET, L"复位(&R)\tR");
	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPop, MF_STRING, IDM_GAME_EXIT, L"退出(&X)\tAlt+F4");

	AppendMenu(GetGameMenu().hMenu, MF_POPUP, (UINT_PTR)hMenuPop, L"游戏(&G)");

	//////////////////
	// Display Menu //
	//////////////////

	hMenuPop = CreateMenu();

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_DISPLAY_SIZE1, L"&1\tAlt+1");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_DISPLAY_SIZE2, L"&2\tAlt+2");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_DISPLAY_SIZE3, L"&3\tAlt+3");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_DISPLAY_SIZE4, L"&4\tAlt+4");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_DISPLAY_SIZE5, L"&5\tAlt+5");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"像素尺寸(&S)");
	
	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
//	AppendMenu(hMenuPop, MF_STRING, IDM_DISPLAY_FRAME, L"显示网格(&F)");
	AppendMenu(hMenuPop, MF_STRING, IDM_DISPLAY_TEX, L"纹理贴图(&T)\tF5");
	AppendMenu(hMenuPop, MF_STRING, IDM_DISPLAY_FPS, L"显示帧数(&F)\tF6");
	AppendMenu(hMenuPop, MF_STRING, IDM_DISPLAY_SPEED, L"速度优先(&P)\tF7");
	AppendMenu(hMenuPop, MF_STRING, IDM_DISPLAY_CONT, L"连续刷新(&C)\tF8");

	AppendMenu(GetGameMenu().hMenu, MF_POPUP, (UINT_PTR)hMenuPop, L"显示(&D)");
	
	//////////////////
	//  Sound Menu  //
	//////////////////

	hMenuPop = CreateMenu();

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGMOFF, L"静音(&M)\tF10");
	AppendMenu(hMenuPopPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM1, L"&10%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM2, L"&20%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM3, L"&30%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM4, L"&40%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM5, L"&50%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM6, L"&60%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM7, L"&70%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM8, L"&80%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM9, L"&90%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_BGM0, L"1&00%");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"音乐音量(&B)");

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SEOFF, L"静音(&M)\tF11");
	AppendMenu(hMenuPopPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE1, L"&10%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE2, L"&20%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE3, L"&30%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE4, L"&40%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE5, L"&50%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE6, L"&60%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE7, L"&70%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE8, L"&80%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE9, L"&90%");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_SOUND_SE0, L"1&00%");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"音效音量(&E)");

	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPop, MF_STRING, IDM_SOUND_OFF, L"静音(&M)\tF9");

	AppendMenu(GetGameMenu().hMenu, MF_POPUP, (UINT_PTR)hMenuPop, L"声音(&S)");

	//////////////////
	//  Other Menu  //
	//////////////////

	hMenuPop = CreateMenu();

	hMenuPopPop = CreateMenu();
	AppendMenu(hMenuPopPop, MF_STRING, IDM_LANG_EN, L"&English");
	AppendMenu(hMenuPopPop, MF_STRING, IDM_LANG_CH, L"简中(&C)");
	AppendMenu(hMenuPop, MF_POPUP, (UINT_PTR)hMenuPopPop, L"语言(&L)");

	AppendMenu(hMenuPop, MF_STRING, IDM_OTHER_SKIP, L"跳过说明(&S)");

	AppendMenu(GetGameMenu().hMenu, MF_POPUP, (UINT_PTR)hMenuPop, L"其他(&O)");

	//////////////////
	// Accelerator  //
	//////////////////
	
	ACCEL arrAccel[20] =
	{
		//Game
		{ FVIRTKEY, 77, IDM_GAME_TITLE },

		{ FVIRTKEY, VK_F1, IDM_GAME_PONG_S1 },
		{ FVIRTKEY, VK_F2, IDM_GAME_RACING_S1 },
		{ FVIRTKEY, VK_F3, IDM_GAME_TETRIS_S1 },
		{ FVIRTKEY, VK_F4, IDM_GAME_SNAKE_S1 },

		{ FVIRTKEY, 80, IDM_GAME_PAUSE },
		{ FVIRTKEY, 67, IDM_GAME_CONTINUE },
		{ FVIRTKEY, 82, IDM_GAME_RESET },

		//Display
		{ FALT, 49, IDM_DISPLAY_SIZE1 },
		{ FALT, 50, IDM_DISPLAY_SIZE2 },
		{ FALT, 51, IDM_DISPLAY_SIZE3 },
		{ FALT, 52, IDM_DISPLAY_SIZE4 },
		{ FALT, 53, IDM_DISPLAY_SIZE5 },

		{ FVIRTKEY, VK_F5, IDM_DISPLAY_TEX },
		{ FVIRTKEY, VK_F6, IDM_DISPLAY_FPS },
		{ FVIRTKEY, VK_F7, IDM_DISPLAY_SPEED },
		{ FVIRTKEY, VK_F8, IDM_DISPLAY_CONT },
		
		//Sound
		{ FVIRTKEY, VK_F9, IDM_SOUND_OFF },
		{ FVIRTKEY, VK_F10, IDM_SOUND_BGMOFF },
		{ FVIRTKEY, VK_F11, IDM_SOUND_SEOFF }
	};

	GetGameMenu().hAccel = CreateAcceleratorTable(arrAccel, 20);

	return TRUE;
}

void MenuOnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = GetMenu(DXUTGetHWnd());

	if (WM_INITMENUPOPUP == message)
	{
		//////////////////
		//  Game Menu   //
		//////////////////

		for (int i = 0; i < 5; i++)
		{
			if (i < GetSaveData()._iStage[0])
				EnableMenuItem(hMenu, IDM_GAME_PONG_S1 + i, MF_ENABLED);
			else
				EnableMenuItem(hMenu, IDM_GAME_PONG_S1 + i, MF_GRAYED);
		}

		for (int i = 0; i < 5; i++)
		{
			if (i < GetSaveData()._iStage[1])
				EnableMenuItem(hMenu, IDM_GAME_RACING_S1 + i, MF_ENABLED);
			else
				EnableMenuItem(hMenu, IDM_GAME_RACING_S1 + i, MF_GRAYED);
		}

		for (int i = 0; i < 6; i++)
		{
			if (i < GetSaveData()._iStage[2])
				EnableMenuItem(hMenu, IDM_GAME_TETRIS_S1 + i, MF_ENABLED);
			else
				EnableMenuItem(hMenu, IDM_GAME_TETRIS_S1 + i, MF_GRAYED);
		}

		for (int i = 0; i < 7; i++)
		{
			if (i < GetSaveData()._iStage[3])
				EnableMenuItem(hMenu, IDM_GAME_SNAKE_S1 + i, MF_ENABLED);
			else
				EnableMenuItem(hMenu, IDM_GAME_SNAKE_S1 + i, MF_GRAYED);
		}

		switch (GetMode())
		{
		case GAME_MENU:
		case GAME_OVER:
			EnableMenuItem(hMenu, IDM_GAME_PAUSE, MF_GRAYED);
			EnableMenuItem(hMenu, IDM_GAME_CONTINUE, MF_GRAYED);
			break;

		case GAME_PLAY:
			EnableMenuItem(hMenu, IDM_GAME_PAUSE, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_GAME_CONTINUE, MF_GRAYED);
			break;

		case GAME_PAUSE:
			EnableMenuItem(hMenu, IDM_GAME_PAUSE, MF_GRAYED);
			EnableMenuItem(hMenu, IDM_GAME_CONTINUE, MF_ENABLED);
			break;
		}

		//////////////////
		// Display Menu //
		//////////////////

		GetGameMenu().iSize = IDM_DISPLAY_SIZE1 + GetSettings()._iPixSize - 1;
		CheckMenuItem(hMenu, GetGameMenu().iSize, MF_CHECKED);

		if (GetSettings()._iFrame)
		{
			GetGameMenu().bFrame = true;
			CheckMenuItem(hMenu, IDM_DISPLAY_FRAME, MF_CHECKED);
		}

		if (GetSettings()._iTexture)
		{
			GetGameMenu().bTex = true;
			CheckMenuItem(hMenu, IDM_DISPLAY_TEX, MF_CHECKED);
		}

		if (GetSettings()._iShowFPS)
		{
			GetGameMenu().bShowFPS = true;
			CheckMenuItem(hMenu, IDM_DISPLAY_FPS, MF_CHECKED);
		}

		if (GetSettings()._iHighFPS)
		{
			GetGameMenu().bHighFPS = true;
			CheckMenuItem(hMenu, IDM_DISPLAY_SPEED, MF_CHECKED);
		}

		if (GetSettings()._iContinuous)
		{
			GetGameMenu().bContinuous = true;
			CheckMenuItem(hMenu, IDM_DISPLAY_CONT, MF_CHECKED);
		}

		//////////////////
		//  Sound Menu  //
		//////////////////

		GetGameMenu().iBGMVol = IDM_SOUND_BGM1 + GetSettings()._iBGMVol - 1;
		GetGameMenu().iSEVol = IDM_SOUND_SE1 + GetSettings()._iSEVol - 1;

		CheckMenuItem(hMenu, GetGameMenu().iBGMVol, MF_CHECKED);
		CheckMenuItem(hMenu, GetGameMenu().iSEVol, MF_CHECKED);

		if (0 == GetSettings()._iSoundOn)
		{
			GetGameMenu().bMute = true;
			CheckMenuItem(hMenu, IDM_SOUND_OFF, MF_CHECKED);
		}

		if (0 == GetSettings()._iBGMOn)
		{
			GetGameMenu().bBGMOff = true;
			CheckMenuItem(hMenu, IDM_SOUND_BGMOFF, MF_CHECKED);
		}

		if (0 == GetSettings()._iSEOn)
		{
			GetGameMenu().bSEOff = true;
			CheckMenuItem(hMenu, IDM_SOUND_SEOFF, MF_CHECKED);
		}

		if (GetGameMenu().bMute)
		{
			EnableMenuItem(hMenu, IDM_SOUND_BGMOFF, MF_GRAYED);
			EnableMenuItem(hMenu, IDM_SOUND_SEOFF, MF_GRAYED);
		}
		else
		{
			EnableMenuItem(hMenu, IDM_SOUND_BGMOFF, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_SOUND_SEOFF, MF_ENABLED);
		}

		if (GetGameMenu().bMute || GetGameMenu().bBGMOff)
		{
			for (UINT ID = IDM_SOUND_BGM1; ID <= IDM_SOUND_BGM0; ID++)
			{
				EnableMenuItem(hMenu, ID, MF_GRAYED);
			}
		}
		else
		{
			for (UINT ID = IDM_SOUND_BGM1; ID <= IDM_SOUND_BGM0; ID++)
			{
				EnableMenuItem(hMenu, ID, MF_ENABLED);
			}
		}

		if (GetGameMenu().bMute || GetGameMenu().bSEOff)
		{
			for (UINT ID = IDM_SOUND_SE1; ID <= IDM_SOUND_SE0; ID++)
			{
				EnableMenuItem(hMenu, ID, MF_GRAYED);
			}
		}
		else
		{
			for (UINT ID = IDM_SOUND_SE1; ID <= IDM_SOUND_SE0; ID++)
			{
				EnableMenuItem(hMenu, ID, MF_ENABLED);
			}
		}

		//////////////////
		//  Other Menu  //
		//////////////////

		GetGameMenu().iLanguage = IDM_LANG_EN + GetSettings()._iLanguage;
		CheckMenuItem(hMenu, GetGameMenu().iLanguage, MF_CHECKED);

		if (GetSettings()._iSkipGuide)
		{
			GetGameMenu().bSkipGuide = true;
			CheckMenuItem(hMenu, IDM_OTHER_SKIP, MF_CHECKED);
		}
	}

	if (WM_COMMAND == message)
	{
		switch (LOWORD(wParam))
		{
			//////////////////
			//  Game Menu   //
			//////////////////

		case IDM_GAME_TITLE:
			if (MENU != GetScene())
			{
				SetMode(GAME_MENU);
				GetScrn().SetRefPeriod(2.0f);
				SetScene(MENU);
				SetInitVal(0);
			}
			break;
			
		case IDM_GAME_PONG_S1:
		case IDM_GAME_PONG_S2:
		case IDM_GAME_PONG_S3:
		case IDM_GAME_PONG_S4:
		case IDM_GAME_PONG_S5:
			SetMode(GAME_PLAY);
			GetScrn().SetRefPeriod(2.0f);
			SetScene(PONG);
			SetInitVal(LOWORD(wParam) - IDM_GAME_PONG_S0);
			break;

		case IDM_GAME_RACING_S1:
		case IDM_GAME_RACING_S2:
		case IDM_GAME_RACING_S3:
		case IDM_GAME_RACING_S4:
		case IDM_GAME_RACING_S5:
			SetMode(GAME_PLAY);
			GetScrn().SetRefPeriod(2.0f);
			SetScene(RACING);
			SetInitVal(LOWORD(wParam) - IDM_GAME_RACING_S0);
			break;

		case IDM_GAME_TETRIS_S1:
		case IDM_GAME_TETRIS_S2:
		case IDM_GAME_TETRIS_S3:
		case IDM_GAME_TETRIS_S4:
		case IDM_GAME_TETRIS_S5:
		case IDM_GAME_TETRIS_S6:
			SetMode(GAME_PLAY);
			GetScrn().SetRefPeriod(2.0f);
			SetScene(TETRIS);
			SetInitVal(LOWORD(wParam) - IDM_GAME_TETRIS_S0);
			break;

		case IDM_GAME_SNAKE_S1:
		case IDM_GAME_SNAKE_S2:
		case IDM_GAME_SNAKE_S3:
		case IDM_GAME_SNAKE_S4:
		case IDM_GAME_SNAKE_S5:
		case IDM_GAME_SNAKE_S6:
		case IDM_GAME_SNAKE_S7:
			SetMode(GAME_PLAY);
			GetScrn().SetRefPeriod(2.0f);
			SetScene(SNAKE);
			SetInitVal(LOWORD(wParam) - IDM_GAME_SNAKE_S0);
			break;
			
		case IDM_GAME_EXTRA:
			GetScrn().SetRefPeriod(2.0f);
			SetScene(EXTRA);
			SetInitVal(1);
			break;

		case IDM_GAME_PAUSE:
			SetMode(GAME_PAUSE);
			DXAudioStop();
			break;

		case IDM_GAME_CONTINUE:
			SetMode(GAME_PLAY);
			DXAudioPlay();
			break;

		case IDM_GAME_RESET:
			SetMode(GAME_MENU);
			GetScrn().SetRefPeriod(2.0f);
			SetScene(OP);
			SetInitVal(0);
			break;

		case IDM_GAME_EXIT:
			QuitGame();
			break;

			//////////////////
			// Display Menu //
			//////////////////

		case IDM_DISPLAY_SIZE1:
		case IDM_DISPLAY_SIZE2:
		case IDM_DISPLAY_SIZE3:
		case IDM_DISPLAY_SIZE4:
		case IDM_DISPLAY_SIZE5:
			if (GetGameMenu().iSize != LOWORD(wParam))
			{
				CheckMenuItem(hMenu, GetGameMenu().iSize, MF_UNCHECKED);
				GetGameMenu().iSize = LOWORD(wParam);
				CheckMenuItem(hMenu, GetGameMenu().iSize, MF_CHECKED);

				GetSettings()._iPixSize = GetGameMenu().iSize - IDM_DISPLAY_SIZE1 + 1;
				UpdateSettings();
				SaveSettings();

				DXUTChangeWindow(GetSettings()._cxWindow, GetSettings()._cyWindow, WS_OVERLAPPEDWINDOW);
				DXUTChangeDevice(GetSettings()._cxDevice, GetSettings()._cyDevice);
			}
			break;

		case IDM_DISPLAY_FRAME:
			if (GetGameMenu().bFrame)
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_FRAME, MF_UNCHECKED);
				GetGameMenu().bFrame = false;
				GetSettings()._iFrame = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_FRAME, MF_CHECKED);
				GetGameMenu().bFrame = true;
				GetSettings()._iFrame = 1;
			}
			SaveSettings();
			break;

		case IDM_DISPLAY_TEX:
			if (GetGameMenu().bTex)
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_TEX, MF_UNCHECKED);
				GetGameMenu().bTex = false;
				GetSettings()._iTexture = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_TEX, MF_CHECKED);
				GetGameMenu().bTex = true;
				GetSettings()._iTexture = 1;
			}
			SaveSettings();
			break;

		case IDM_DISPLAY_FPS:
			if (GetGameMenu().bShowFPS)
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_FPS, MF_UNCHECKED);
				GetGameMenu().bShowFPS = false;
				GetSettings()._iShowFPS = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_FPS, MF_CHECKED);
				GetGameMenu().bShowFPS = true;
				GetSettings()._iShowFPS = 1;
			}
			SaveSettings();
			break;

		case IDM_DISPLAY_SPEED:
			if (GetGameMenu().bHighFPS)
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_SPEED, MF_UNCHECKED);
				GetGameMenu().bHighFPS = false;
				GetSettings()._iHighFPS = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_SPEED, MF_CHECKED);
				GetGameMenu().bHighFPS = true;
				GetSettings()._iHighFPS = 1;
			}
			SaveSettings();
			break;

		case IDM_DISPLAY_CONT:
			if (GetGameMenu().bContinuous)
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_CONT, MF_UNCHECKED);
				GetGameMenu().bContinuous = false;
				GetSettings()._iContinuous = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_DISPLAY_CONT, MF_CHECKED);
				GetGameMenu().bContinuous = true;
				GetSettings()._iContinuous = 1;
			}
			SaveSettings();
			break;

			//////////////////
			//  Sound Menu  //
			//////////////////

		case IDM_SOUND_OFF:
			if (GetGameMenu().bMute)
			{
				CheckMenuItem(hMenu, IDM_SOUND_OFF, MF_UNCHECKED);
				GetGameMenu().bMute = false;
				GetSettings()._iSoundOn = 1;
				SetMasterVolumn(1.0f);
			}
			else
			{
				CheckMenuItem(hMenu, IDM_SOUND_OFF, MF_CHECKED);
				GetGameMenu().bMute = true;
				GetSettings()._iSoundOn = 0;
				SetMasterVolumn(0.0f);
			}
			SaveSettings();
			break;

		case IDM_SOUND_BGMOFF:
			if (GetGameMenu().bBGMOff)
			{
				CheckMenuItem(hMenu, IDM_SOUND_BGMOFF, MF_UNCHECKED);
				GetGameMenu().bBGMOff = false;
				GetSettings()._iBGMOn = 1;
				SetSubmixVolumn(ID_SUBMIX_BACKGROUND, 0.1f * GetSettings()._iBGMVol);
			}
			else
			{
				CheckMenuItem(hMenu, IDM_SOUND_BGMOFF, MF_CHECKED);
				GetGameMenu().bBGMOff = true;
				GetSettings()._iBGMOn = 0;
				SetSubmixVolumn(ID_SUBMIX_BACKGROUND, 0.0f);
			}
			SaveSettings();
			break;

		case IDM_SOUND_SEOFF:
			if (GetGameMenu().bSEOff)
			{
				CheckMenuItem(hMenu, IDM_SOUND_SEOFF, MF_UNCHECKED);
				GetGameMenu().bSEOff = false;
				GetSettings()._iSEOn = 1;
				SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, 0.1f * GetSettings()._iSEVol);
			}
			else
			{
				CheckMenuItem(hMenu, IDM_SOUND_SEOFF, MF_CHECKED);
				GetGameMenu().bSEOff = true;
				GetSettings()._iSEOn = 0;
				SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, 0.0f);
			}
			SaveSettings();
			break;
			break;

		case IDM_SOUND_BGM1:
		case IDM_SOUND_BGM2:
		case IDM_SOUND_BGM3:
		case IDM_SOUND_BGM4:
		case IDM_SOUND_BGM5:
		case IDM_SOUND_BGM6:
		case IDM_SOUND_BGM7:
		case IDM_SOUND_BGM8:
		case IDM_SOUND_BGM9:
		case IDM_SOUND_BGM0:
			if (GetGameMenu().iBGMVol != LOWORD(wParam))
			{
				CheckMenuItem(hMenu, GetGameMenu().iBGMVol, MF_UNCHECKED);
				GetGameMenu().iBGMVol = LOWORD(wParam);
				CheckMenuItem(hMenu, GetGameMenu().iBGMVol, MF_CHECKED);

				GetSettings()._iBGMVol = GetGameMenu().iBGMVol - IDM_SOUND_BGM1 + 1;
				SaveSettings();

				SetSubmixVolumn(ID_SUBMIX_BACKGROUND, 0.1f * GetSettings()._iBGMVol);
			}
			break;

		case IDM_SOUND_SE1:
		case IDM_SOUND_SE2:
		case IDM_SOUND_SE3:
		case IDM_SOUND_SE4:
		case IDM_SOUND_SE5:
		case IDM_SOUND_SE6:
		case IDM_SOUND_SE7:
		case IDM_SOUND_SE8:
		case IDM_SOUND_SE9:
		case IDM_SOUND_SE0:
			if (GetGameMenu().iSEVol != LOWORD(wParam))
			{
				CheckMenuItem(hMenu, GetGameMenu().iSEVol, MF_UNCHECKED);
				GetGameMenu().iSEVol = LOWORD(wParam);
				CheckMenuItem(hMenu, GetGameMenu().iSEVol, MF_UNCHECKED);

				GetSettings()._iSEVol = GetGameMenu().iSEVol - IDM_SOUND_SE1 + 1;
				SaveSettings();

				SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, 0.1f * GetSettings()._iSEVol);
			}
			break;

			//////////////////
			//  Other Menu  //
			//////////////////

		case IDM_LANG_EN:
			if (GetGameMenu().iLanguage != IDM_LANG_EN)
			{
				CheckMenuItem(hMenu, IDM_LANG_CH, MF_UNCHECKED);
				CheckMenuItem(hMenu, IDM_LANG_EN, MF_CHECKED); 
				GetGameMenu().iLanguage = IDM_LANG_EN;
				GetSettings()._iLanguage = 0;
				SaveSettings();
			}
			break;

		case IDM_LANG_CH:
			if (GetGameMenu().iLanguage != IDM_LANG_CH)
			{
				CheckMenuItem(hMenu, IDM_LANG_EN, MF_UNCHECKED);
				CheckMenuItem(hMenu, IDM_LANG_CH, MF_CHECKED);
				GetGameMenu().iLanguage = IDM_LANG_CH;
				GetSettings()._iLanguage = 1;
				SaveSettings();
			}
			break;

		case IDM_OTHER_SKIP:
			if (GetGameMenu().bSkipGuide)
			{
				CheckMenuItem(hMenu, IDM_OTHER_SKIP, MF_UNCHECKED);
				GetGameMenu().bSkipGuide = false;
				GetSettings()._iSkipGuide = 0;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_OTHER_SKIP, MF_CHECKED);
				GetGameMenu().bSkipGuide = true;
				GetSettings()._iSkipGuide = 1;
			}
			SaveSettings();
			break;
		}
	}
}