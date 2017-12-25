#include "Game.h"
#include "GameGUI.h"
#include "GameAudio.h"

//
//MainMenu
//

MainMenu::MainMenu()
{
	DXDialog::DXDialog();
	m_Width = DXUTGetDeviceWidth();
	m_Height = DXUTGetDeviceHeight();
	CodeCheck = 0;
}

MainMenu::~MainMenu()
{
	DXDialog::~DXDialog();
}

void MainMenu::Init()
{
	SetFont(0, L"黑体", 28, 0);
	SetFont(1, L"黑体", 72, 0);
	SetFont(2, L"Small Fonts", 28, 0);

	DXElement TitleElement(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement TitleElement2(1, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	DXElement NormalElement		(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement	(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement	(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));

	m_pSoundFocus = new Sound();
	m_pSoundFocus->Register(ID_SE_MAIN_CHOOSE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundFocus->AddSound(FILE_CURSOR);

	m_pSoundPress = new Sound();
	m_pSoundPress->Register(ID_SE_MAIN_DECIDE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundPress->AddSound(FILE_DECIDE);

	m_pSoundCancel = new Sound();
	m_pSoundCancel->Register(ID_SE_MAIN_CANCEL, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundCancel->AddSound(FILE_CANCEL);
	
	DXStatic *pTitle = AddStatic(-1, 120, 184, 400, 28, L"绝 对 绝 望");
	pTitle->AddElement(TitleElement);

	DXStatic *pTitle2 = AddStatic(-1, 300, 140, 400, 72, L"像素");
	pTitle2->AddElement(TitleElement2);

	DXButton *pStart = AddButton(IDC_START, 120, m_y + m_Height / 2 , 100, 30, L"开 始");
	pStart->AddElement(NormalElement);
	pStart->AddElement(PressedElement);
	pStart->AddElement(DisabledElement);

	DXButton *pExtra = AddButton(IDC_HOPE, 120, m_y + m_Height / 2 + 60, 100, 30, L"希 望");
	pExtra->AddElement(NormalElement);
	pExtra->AddElement(PressedElement);
	pExtra->AddElement(DisabledElement);

	DXButton *pOption = AddButton(IDC_SETTING, 120, m_y + m_Height / 2 + 120, 100, 30, L"选 项");
	pOption->AddElement(NormalElement);
	pOption->AddElement(PressedElement);
	pOption->AddElement(DisabledElement);

	DXButton *pQuit = AddButton(IDC_QUIT, 120, m_y + m_Height / 2 + 180, 100, 30, L"退 出");
	pQuit->AddElement(NormalElement);
	pQuit->AddElement(PressedElement);
	pQuit->AddElement(DisabledElement);
}

void MainMenu::Default()
{
	DXDialog::Default();
	SetHidden(IDC_HOPE);
}

void MainMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (3 == GetFocus()->GetID())
			{
				PlayCancelSound();
				SetPressed(3);
			}
			else
			{
				PlayCancelSound();
				SetFocus(3);
			}
		}
		if (wParam == 49 && CodeCheck == 0) CodeCheck++;
		if (wParam == 49 && CodeCheck == 1) CodeCheck++;
		if (wParam == 48 && CodeCheck == 2) CodeCheck++;
		if (wParam == 51 && CodeCheck == 3) CodeCheck++;
		if (wParam == 55 && CodeCheck == 4) CodeCheck++;
		if (CodeCheck == 5)
		{
			SetNormal(IDC_HOPE);
			CodeCheck = 0;
		}
		break;
	default:
		break;
	}
	DXDialog::OnMsgProc(message, wParam, lParam);
}

//
//AudioMenu
//

AudioMenu::AudioMenu()
{
	DXDialog::DXDialog();
	m_Width = DXUTGetDeviceWidth();
	m_Height = DXUTGetDeviceHeight();
}

AudioMenu::~AudioMenu()
{
	DXDialog::~DXDialog();
}

void AudioMenu::Init()
{

}

void AudioMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PlayCancelSound();
			OpenMainMenu();
		}
	default:
		break;
	}
	DXDialog::OnMsgProc(message, wParam, lParam);
}

//
//VideoMenu
//

VideoMenu::VideoMenu()
{
	DXDialog::DXDialog();
	m_Width = DXUTGetDeviceWidth();
	m_Height = DXUTGetDeviceHeight();
}

VideoMenu::~VideoMenu()
{
	DXDialog::~DXDialog();
}

void VideoMenu::Init()
{

}

void VideoMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PlayCancelSound();
			OpenMainMenu();
		}
	default:
		break;
	}
	DXDialog::OnMsgProc(message, wParam, lParam);
}

//
//GameSelect
//

GameSelect::GameSelect()
{
	DXDialog::DXDialog();
	m_Width = DXUTGetDeviceWidth();
	m_Height = DXUTGetDeviceHeight();
}

GameSelect::~GameSelect()
{
	DXDialog::~DXDialog();
}

void GameSelect::Init()
{
	SetFont(0, L"黑体", 28, 0);

	DXElement TitleElement(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));

	m_pSoundFocus = new Sound();
	m_pSoundFocus->Register(ID_SE_SELECT_CHOOSE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundFocus->AddSound(FILE_CURSOR);

	m_pSoundPress = new Sound();
	m_pSoundPress->Register(ID_SE_SELECT_DECIDE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundPress->AddSound(FILE_DECIDE);

	m_pSoundCancel = new Sound();
	m_pSoundCancel->Register(ID_SE_SELECT_CANCEL, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundCancel->AddSound(FILE_CANCEL);

	DXStatic *pTitle = AddStatic(-1, 120, 184, 400, 28, L"游 戏 选 择");
	pTitle->AddElement(TitleElement);

	DXButton *pTetris = AddButton(IDC_TETRIS, 120, m_y + m_Height / 2, 200, 30, L"俄罗斯方块");
	pTetris->AddElement(NormalElement);
	pTetris->AddElement(PressedElement);
	pTetris->AddElement(DisabledElement);

	DXButton *pSnake = AddButton(IDC_SNAKE, 120, m_y + m_Height / 2 + 45, 200, 30, L"贪　吃　蛇");
	pSnake->AddElement(NormalElement);
	pSnake->AddElement(PressedElement);
	pSnake->AddElement(DisabledElement);

	DXButton *pSweeper = AddButton(IDC_SWEEPER, 120, m_y + m_Height / 2 + 90, 200, 30, L"扫　　　雷");
	pSweeper->AddElement(NormalElement);
	pSweeper->AddElement(PressedElement);
	pSweeper->AddElement(DisabledElement);

	DXButton *pShooter = AddButton(IDC_SHOOTER, 120, m_y + m_Height / 2 + 135, 200, 30, L"射　　　击");
	pShooter->AddElement(NormalElement);
	pShooter->AddElement(PressedElement);
	pShooter->AddElement(DisabledElement);

	DXButton *pDownstair = AddButton(IDC_DOWNSTAIR, 120, m_y + m_Height / 2 + 180, 200, 30, L"一　百　层");
	pDownstair->AddElement(NormalElement);
	pDownstair->AddElement(PressedElement);
	pDownstair->AddElement(DisabledElement);
}

void GameSelect::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PlayCancelSound();
			OpenMainMenu();
		}
	default:
		break;
	}
	DXDialog::OnMsgProc(message, wParam, lParam);
}

//
//PauseMenu
//

PauseMenu::PauseMenu()
{
	DXDialog::DXDialog();
	m_Width = DXUTGetDeviceWidth();
	m_Height = DXUTGetDeviceHeight();
}

PauseMenu::~PauseMenu()
{
	DXDialog::~DXDialog();
}

void PauseMenu::Init()
{
	SetFont(0, L"Small Fonts", 28, 0);

	DXElement NormalElement		(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	DXElement PressedElement	(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement	(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	m_pSoundFocus = new Sound();
	m_pSoundFocus->Register(ID_SE_PAUSE_CHOOSE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundFocus->AddSound(FILE_CURSOR);

	m_pSoundPress = new Sound();
	m_pSoundPress->Register(ID_SE_PAUSE_DECIDE, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundPress->AddSound(FILE_DECIDE);

	m_pSoundCancel = new Sound();
	m_pSoundCancel->Register(ID_SE_PAUSE_CANCEL, ID_SUBMIX_SOUNDEFFECT);
	m_pSoundCancel->AddSound(FILE_CANCEL);

	DXStatic *pTitle = AddStatic(-1, 10, 10, 400, 30, L"A Test for Game Core");
	pTitle->AddElement(0, DT_LEFT | DT_VCENTER, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	DXButton *pContinue = AddButton(IDC_CONTINUE, 100, m_y + m_Height / 2 - 30, 200, 30, L"Continue");
	pContinue->AddElement(NormalElement);
	pContinue->AddElement(PressedElement);
	pContinue->AddElement(DisabledElement);

	DXButton *pRetry = AddButton(IDC_RETRY, 100, m_y + m_Height / 2 + 40, 200, 30, L"Retry");
	pRetry->AddElement(NormalElement);
	pRetry->AddElement(PressedElement);
	pRetry->AddElement(DisabledElement);

	DXButton *pMain = AddButton(IDC_MAIN, 100, m_y + m_Height / 2 + 110, 200, 30, L"Main Menu");
	pMain->AddElement(NormalElement);
	pMain->AddElement(PressedElement);
	pMain->AddElement(DisabledElement);

	DXButton *pQuit = AddButton(IDC_QUIT, 100, m_y + m_Height / 2 + 180, 200, 30, L"Quit Game");
	pQuit->AddElement(NormalElement);
	pQuit->AddElement(PressedElement);
	pQuit->AddElement(DisabledElement);
}

void PauseMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (3 == GetFocus()->GetID())
			{
				PlayCancelSound();
				SetPressed(3);
			}
			else
			{ 
				PlayCancelSound();
				SetFocus(3);
			}
		}
	default:
		break;
	}
	DXDialog::OnMsgProc(message, wParam, lParam);
}