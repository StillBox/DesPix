#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu()
{
	return;
}

SceneMenu::~SceneMenu()
{

}

void SceneMenu::Init(int iInitVal)
{
	GetScrn().Set(false, WHITE);
	SetText();
	m_iMenu = iInitVal;
	m_iFocus = 0;
	m_iStage = 1;
	m_bChange = true;
	m_bMenuChange = true;
	m_nextScene = MENU;
	m_iUnlock = 0;
	m_timeUpdateDespair = -100000.0f;

	for (auto &bUp : m_bKeyUp)
		bUp = false;
	for (auto &fUp : m_fKeyUp)
		fUp = 100.0f;

	//Settings

	GetScrn().Refresh(BRUSH, NULL);
	GetScrn().SetRefPeriod(2.0f);

	//Sound
	m_pBGM = new Sound();
	m_pBGM->Register(ID_BGM_MENU, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_TITLE, 255);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_MENU, ID_SUBMIX_SOUNDEFFECT);
	m_pSE->AddSoundFromRsrc(IDR_SELECT);
	m_pSE->AddSoundFromRsrc(IDR_CLEAR);

	m_bBGMPlay = false;
}

void SceneMenu::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
}

void SceneMenu::OnResetDevice()
{

}

void SceneMenu::OnLostDevice()
{

}

void SceneMenu::Update(double fTime, float fElapsedTime)
{
	GetInput();

	for (int i = 0; i < 2; i++)
	{
		if (m_fKeyUp[i] <= 0.0f)
			m_fKeyUp[i] = 0.0f;
		else
			m_fKeyUp[i] -= fElapsedTime;
	}

	//11037

	static bool bKeyUp = false;

	if (IsKeyDown())
	{
		if (bKeyUp)
		{
			if (IsKeyDown(DIK_1) || IsKeyDown(DIK_NUMPAD1))
			{
				if (m_iUnlock == 0 || m_iUnlock == 1)
					m_iUnlock++;
				else
					m_iUnlock = 0;
			}
			else if (IsKeyDown(DIK_0) || IsKeyDown(DIK_NUMPAD0))
			{
				if (m_iUnlock == 2)
					m_iUnlock++;
				else
					m_iUnlock = 0;
			}
			else if (IsKeyDown(DIK_3) || IsKeyDown(DIK_NUMPAD3))
			{
				if (m_iUnlock == 3)
					m_iUnlock++;
				else
					m_iUnlock = 0;
			}
			else if (IsKeyDown(DIK_7) || IsKeyDown(DIK_NUMPAD7))
			{
				if (m_iUnlock == 4)
					m_iUnlock++;
				else
					m_iUnlock = 0;
			}
			else
				m_iUnlock = 0;
		}
		bKeyUp = false;
	}
	else
		bKeyUp = true;

	if (m_iUnlock == 5)
	{
		m_iUnlock = 6;
		m_pSE->Play(1);

		GetSaveData()._iStage[0] = 5;
		GetSaveData()._iStage[1] = 5;
		GetSaveData()._iStage[2] = 6;
		GetSaveData()._iStage[3] = 7;

		GetSaveData()._iEnd[0] = 1;
		GetSaveData()._iEnd[1] = 1;
		GetSaveData()._iEnd[2] = 1;

		WriteSaveData();
	}

	//Scene Update

	if (GetSaveData()._iStage[2] > 1 && m_iMenu != -2)
	{
		m_timeUpdateDespair += fElapsedTime;

		if (m_timeUpdateDespair >= 562.5f)
		{
			GetScrn().Set(false, WHITE, &m_vecDespair);
			while (m_timeUpdateDespair >= 562.5)
			{
				for (size_t i = 0; i < m_vecDespair.size(); i++)
				{
					m_vecDespair[i]._y += m_vecDespairOffset[i];
					if (m_vecDespair[i]._y < 20)
						m_vecDespair[i]._y += 140;
					if (m_vecDespair[i]._y >= 160)
						m_vecDespair[i]._y -= 140;
				}
				m_timeUpdateDespair -= 562.5f;
			}

			GetScrn().Set(true, WHITE, &m_vecDespair);
			GetScrn().Refresh(IMM, &m_rcDespair);
		}
	}

	if (GetScrn().IsRefOver())
	{
		if (!m_bBGMPlay)
		{
			m_pBGM->Play(0);
			m_bBGMPlay = true;
			if (m_timeUpdateDespair < 0.0f)
				m_timeUpdateDespair = 562.5f * (rand() % 20 + 10) + 85.0f;
		}

		if (m_bChange)
		{
			if (m_bMenuChange)
			{
				switch (m_iMenu)
				{
				case -1:
					QuitGame();
					break;

				case -2:
					SetMode(GAME_PLAY);
					SetScene(m_nextScene);
					SetInitVal(m_iStage);
					break;

				default:
					SetMenu();
					break;
				}
				m_bMenuChange = false;
			}
			else
			{
				SetButton(FOCUS);
				m_bChange = false;
			}
		}
		else
		{
			if (m_iMenu == 0)
			{
				if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
				{
					if (m_iFocus != 0)
					{
						m_pSE->Play(0);
						SetButton(NORMAL);
						m_iFocus--;
						m_bChange = true;
					}
				}
				if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
				{
					if (m_iFocus != 2)
					{
						m_pSE->Play(0);
						SetButton(NORMAL);
						m_iFocus++;
						m_bChange = true;
					}
				}
				if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
				{
					if (m_iFocus != 2)
					{
						m_pSE->Play(0);
						SetButton(NORMAL);
						m_iFocus = 2;
						m_bChange = true;
					}
				}
				if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
				{
					m_pSE->Play(0);
					switch (m_iFocus)
					{
					case 0:
						GetScrn().Set(false, WHITE, &m_rcMenu);
						GetScrn().Refresh(BRUSH, &m_rcMenu);
						m_iMenu = 1;
						break;
					case 1:
						GetScrn().Set(false, WHITE);
						GetScrn().Refresh(BRUSH);
						m_nextScene = EXTRA;
						m_iMenu = -2;
						m_iFocus = 0;
						m_bMenuChange = true;
						m_bChange = true;
						break;
					case 2:
						GetScrn().Set(false, WHITE);
						GetScrn().Refresh(BRUSH, NULL);
						m_iMenu = -1;
						break;
					}
					m_iFocus = 0;
					m_bMenuChange = true;
					m_bChange = true;
				}
			}
			else if (m_iMenu == 1)
			{
				if (IsKeyDown(DIK_UP))
				{
					if (m_iFocus != 0)
					{
						m_pSE->Play(0);
						SetButton(NORMAL);
						m_iFocus--;
						m_iStage = 1;
						m_bChange = true;
						return;
					}
				}

				if (IsKeyDown(DIK_DOWN))
				{
					if (m_iFocus != 3)
					{
						m_pSE->Play(0);
						SetButton(NORMAL);
						m_iFocus++;
						m_iStage = 1;
						m_bChange = true;
						return;
					}
				}

				if (IsKeyDown(DIK_LEFT))
				{
					if (m_bKeyUp[0] || m_fKeyUp[0] <= 0.0f)
					{
						if (m_iStage > 1)
						{
							m_pSE->Play(0);
							SetStage(m_iStage, false);
							m_iStage--;
							m_fKeyUp[0] = m_bKeyUp[0] ? 500.0f : 100.0f;
						}
					}
					m_bKeyUp[0] = false;
				}
				else
					m_bKeyUp[0] = true;

				if (IsKeyDown(DIK_RIGHT))
				{
					if (m_bKeyUp[1] || m_fKeyUp[1] <= 0.0f)
					{
						if (m_iStage < GetSaveData()._iStage[m_iFocus])
						{
							m_pSE->Play(0);
							m_iStage++;
							SetStage(m_iStage, true);
							m_fKeyUp[1] = m_bKeyUp[1] ? 500.0f : 100.0f;
						}
					}
					m_bKeyUp[1] = false;
				}
				else
					m_bKeyUp[1] = true;

				if (IsKeyDown(DIK_ESCAPE))
				{
					m_pSE->Play(0);
					GetScrn().Set(false, WHITE, &m_rcMenu);
					GetScrn().Refresh(BRUSH, &m_rcMenu);
					m_iMenu = 0;
					m_iFocus = 0;
					m_bMenuChange = true;
					m_bChange = true;
				}

				if (IsKeyDown(DIK_RETURN))
				{
					m_pSE->Play(0);
					switch (m_iFocus)
					{
					case 0:
						m_nextScene = PONG;
						break;
					case 1:
						m_nextScene = RACING;
						break;
					case 2:
						m_nextScene = TETRIS;
						break;
					case 3:
						m_nextScene = SNAKE;
						break;
					}
					GetScrn().Set(false, WHITE);
					GetScrn().Refresh(BRUSH, NULL);
					m_iMenu = -2;
					m_iFocus = 0;
					m_bMenuChange = true;
					m_bChange = true;
				}
			}
		}
	}

	if (!GetScrn().IsRefOver())
	{
		GetScrn().Update(fTime, fElapsedTime);
	}
}

void SceneMenu::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);
}

void SceneMenu::SetText()
{
	int xLB, yLB;

	//
	//Title
	//

	SetRect(&m_rcTitle, 36, 30, 120, 60);

	//D

	xLB = m_rcTitle.left + 1;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 12 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 3 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 9 });
	m_vecTitle.push_back({ xLB + 1,yLB - 10 });
	m_vecTitle.push_back({ xLB + 1,yLB - 11 });
	m_vecTitle.push_back({ xLB + 1,yLB - 12 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 12 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 12 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 12 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 12 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 12 });
	m_vecTitle.push_back({ xLB + 7,yLB - 1 });
	m_vecTitle.push_back({ xLB + 7,yLB - 2 });
	m_vecTitle.push_back({ xLB + 7,yLB - 3 });
	m_vecTitle.push_back({ xLB + 7,yLB - 10 });
	m_vecTitle.push_back({ xLB + 7,yLB - 11 });
	m_vecTitle.push_back({ xLB + 8,yLB - 3 });
	m_vecTitle.push_back({ xLB + 8,yLB - 4 });
	m_vecTitle.push_back({ xLB + 8,yLB - 5 });
	m_vecTitle.push_back({ xLB + 8,yLB - 6 });
	m_vecTitle.push_back({ xLB + 8,yLB - 7 });
	m_vecTitle.push_back({ xLB + 8,yLB - 8 });
	m_vecTitle.push_back({ xLB + 8,yLB - 9 });
	m_vecTitle.push_back({ xLB + 8,yLB - 10 });

	//e

	xLB = m_rcTitle.left + 12;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 2 });
	m_vecTitle.push_back({ xLB + 0,yLB - 3 });
	m_vecTitle.push_back({ xLB + 0,yLB - 4 });
	m_vecTitle.push_back({ xLB + 0,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 4 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 4 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 4 });
	m_vecTitle.push_back({ xLB + 6,yLB - 5 });
	m_vecTitle.push_back({ xLB + 6,yLB - 6 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 7,yLB - 4 });
	m_vecTitle.push_back({ xLB + 7,yLB - 5 });
	
	//s

	xLB = m_rcTitle.left + 22;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 1 });
	m_vecTitle.push_back({ xLB + 0,yLB - 5 });
	m_vecTitle.push_back({ xLB + 0,yLB - 6 });
	m_vecTitle.push_back({ xLB + 0,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 3 });
	m_vecTitle.push_back({ xLB + 4,yLB - 4 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 1 });
	m_vecTitle.push_back({ xLB + 5,yLB - 2 });
	m_vecTitle.push_back({ xLB + 5,yLB - 3 });
	m_vecTitle.push_back({ xLB + 5,yLB - 7 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });

	//p

	xLB = m_rcTitle.left + 30;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB + 3 });
	m_vecTitle.push_back({ xLB + 0,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB + 3 });
	m_vecTitle.push_back({ xLB + 1,yLB + 2 });
	m_vecTitle.push_back({ xLB + 1,yLB + 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 3 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB + 3 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 7 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 2 });
	m_vecTitle.push_back({ xLB + 6,yLB - 6 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 7,yLB - 2 });
	m_vecTitle.push_back({ xLB + 7,yLB - 3 });
	m_vecTitle.push_back({ xLB + 7,yLB - 4 });
	m_vecTitle.push_back({ xLB + 7,yLB - 5 });
	m_vecTitle.push_back({ xLB + 7,yLB - 6 });

	//r

	xLB = m_rcTitle.left + 40;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 3 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 6 });
	m_vecTitle.push_back({ xLB + 3,yLB - 7 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 6,yLB - 8 });

	//a

	xLB = m_rcTitle.left + 49;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 1 });
	m_vecTitle.push_back({ xLB + 0,yLB - 2 });
	m_vecTitle.push_back({ xLB + 0,yLB - 3 });
	m_vecTitle.push_back({ xLB + 0,yLB - 6 });
	m_vecTitle.push_back({ xLB + 0,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 5 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 5 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 2 });
	m_vecTitle.push_back({ xLB + 6,yLB - 3 });
	m_vecTitle.push_back({ xLB + 6,yLB - 4 });
	m_vecTitle.push_back({ xLB + 6,yLB - 5 });
	m_vecTitle.push_back({ xLB + 6,yLB - 6 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 7,yLB - 0 });

	//t

	xLB = m_rcTitle.left + 58;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 9 });
	m_vecTitle.push_back({ xLB + 2,yLB - 1 });
	m_vecTitle.push_back({ xLB + 2,yLB - 2 });
	m_vecTitle.push_back({ xLB + 2,yLB - 3 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 5 });
	m_vecTitle.push_back({ xLB + 2,yLB - 6 });
	m_vecTitle.push_back({ xLB + 2,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 9 });
	m_vecTitle.push_back({ xLB + 2,yLB - 10 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });

	//e

	xLB = m_rcTitle.left + 66;
	yLB = m_rcTitle.bottom - 18;

	m_vecTitle.push_back({ xLB + 0,yLB - 2 });
	m_vecTitle.push_back({ xLB + 0,yLB - 3 });
	m_vecTitle.push_back({ xLB + 0,yLB - 4 });
	m_vecTitle.push_back({ xLB + 0,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 4 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 4 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 4 });
	m_vecTitle.push_back({ xLB + 6,yLB - 5 });
	m_vecTitle.push_back({ xLB + 6,yLB - 6 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 7,yLB - 4 });
	m_vecTitle.push_back({ xLB + 7,yLB - 5 });

	//P

	xLB = m_rcTitle.left + 40;
	yLB = m_rcTitle.bottom - 2;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 12 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 3 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 9 });
	m_vecTitle.push_back({ xLB + 1,yLB - 10 });
	m_vecTitle.push_back({ xLB + 1,yLB - 11 });
	m_vecTitle.push_back({ xLB + 1,yLB - 12 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 6 });
	m_vecTitle.push_back({ xLB + 2,yLB - 12 });
	m_vecTitle.push_back({ xLB + 3,yLB - 6 });
	m_vecTitle.push_back({ xLB + 3,yLB - 12 });
	m_vecTitle.push_back({ xLB + 4,yLB - 6 });
	m_vecTitle.push_back({ xLB + 4,yLB - 12 });
	m_vecTitle.push_back({ xLB + 5,yLB - 6 });
	m_vecTitle.push_back({ xLB + 5,yLB - 12 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 6,yLB - 12 });
	m_vecTitle.push_back({ xLB + 7,yLB - 8 });
	m_vecTitle.push_back({ xLB + 7,yLB - 9 });
	m_vecTitle.push_back({ xLB + 7,yLB - 10 });
	m_vecTitle.push_back({ xLB + 7,yLB - 11 });

	//i

	xLB = m_rcTitle.left + 50;
	yLB = m_rcTitle.bottom - 2;

	m_vecTitle.push_back({ xLB + 0,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 1 });
	m_vecTitle.push_back({ xLB + 2,yLB - 2 });
	m_vecTitle.push_back({ xLB + 2,yLB - 3 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 5 });
	m_vecTitle.push_back({ xLB + 2,yLB - 6 });
	m_vecTitle.push_back({ xLB + 2,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 11 });
	m_vecTitle.push_back({ xLB + 2,yLB - 12 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 11 });
	m_vecTitle.push_back({ xLB + 3,yLB - 12 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });

	//x

	xLB = m_rcTitle.left + 57;
	yLB = m_rcTitle.bottom - 2;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 2,yLB - 2 });
	m_vecTitle.push_back({ xLB + 2,yLB - 6 });
	m_vecTitle.push_back({ xLB + 2,yLB - 7 });
	m_vecTitle.push_back({ xLB + 3,yLB - 3 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 5 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 1 });
	m_vecTitle.push_back({ xLB + 4,yLB - 2 });
	m_vecTitle.push_back({ xLB + 4,yLB - 6 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 1 });
	m_vecTitle.push_back({ xLB + 5,yLB - 7 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 0 });
	m_vecTitle.push_back({ xLB + 6,yLB - 8 });

	//e

	xLB = m_rcTitle.left + 66;
	yLB = m_rcTitle.bottom - 2;

	m_vecTitle.push_back({ xLB + 0,yLB - 2 });
	m_vecTitle.push_back({ xLB + 0,yLB - 3 });
	m_vecTitle.push_back({ xLB + 0,yLB - 4 });
	m_vecTitle.push_back({ xLB + 0,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 2,yLB - 4 });
	m_vecTitle.push_back({ xLB + 2,yLB - 8 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 4 });
	m_vecTitle.push_back({ xLB + 3,yLB - 8 });
	m_vecTitle.push_back({ xLB + 4,yLB - 0 });
	m_vecTitle.push_back({ xLB + 4,yLB - 4 });
	m_vecTitle.push_back({ xLB + 4,yLB - 8 });
	m_vecTitle.push_back({ xLB + 5,yLB - 0 });
	m_vecTitle.push_back({ xLB + 5,yLB - 4 });
	m_vecTitle.push_back({ xLB + 5,yLB - 8 });
	m_vecTitle.push_back({ xLB + 6,yLB - 1 });
	m_vecTitle.push_back({ xLB + 6,yLB - 4 });
	m_vecTitle.push_back({ xLB + 6,yLB - 5 });
	m_vecTitle.push_back({ xLB + 6,yLB - 6 });
	m_vecTitle.push_back({ xLB + 6,yLB - 7 });
	m_vecTitle.push_back({ xLB + 7,yLB - 4 });
	m_vecTitle.push_back({ xLB + 7,yLB - 5 });

	//l

	xLB = m_rcTitle.left + 76;
	yLB = m_rcTitle.bottom - 2;

	m_vecTitle.push_back({ xLB + 0,yLB - 0 });
	m_vecTitle.push_back({ xLB + 0,yLB - 12 });
	m_vecTitle.push_back({ xLB + 1,yLB - 0 });
	m_vecTitle.push_back({ xLB + 1,yLB - 1 });
	m_vecTitle.push_back({ xLB + 1,yLB - 2 });
	m_vecTitle.push_back({ xLB + 1,yLB - 3 });
	m_vecTitle.push_back({ xLB + 1,yLB - 4 });
	m_vecTitle.push_back({ xLB + 1,yLB - 5 });
	m_vecTitle.push_back({ xLB + 1,yLB - 6 });
	m_vecTitle.push_back({ xLB + 1,yLB - 7 });
	m_vecTitle.push_back({ xLB + 1,yLB - 8 });
	m_vecTitle.push_back({ xLB + 1,yLB - 9 });
	m_vecTitle.push_back({ xLB + 1,yLB - 10 });
	m_vecTitle.push_back({ xLB + 1,yLB - 11 });
	m_vecTitle.push_back({ xLB + 1,yLB - 12 });
	m_vecTitle.push_back({ xLB + 2,yLB - 0 });
	m_vecTitle.push_back({ xLB + 3,yLB - 0 });

	GetScrn().Set(true, WHITE, &m_vecTitle);
	
	//
	//Menu
	//

	SetRect(&m_rcMenu, 36, 80, 120, 160);

	//
	//Main Menu
	//

	//Game

	SetRect(&m_rcGame, 40, 90, 72, 100);

	xLB = m_rcGame.left + 2;
	yLB = m_rcGame.bottom - 3;

	m_vecGame.push_back({ xLB + 0,yLB - 1 });
	m_vecGame.push_back({ xLB + 0,yLB - 2 });
	m_vecGame.push_back({ xLB + 0,yLB - 3 });
	m_vecGame.push_back({ xLB + 0,yLB - 4 });
	m_vecGame.push_back({ xLB + 1,yLB - 0 });
	m_vecGame.push_back({ xLB + 1,yLB - 5 });
	m_vecGame.push_back({ xLB + 2,yLB - 0 });
	m_vecGame.push_back({ xLB + 2,yLB - 5 });
	m_vecGame.push_back({ xLB + 3,yLB - 0 });
	m_vecGame.push_back({ xLB + 3,yLB - 2 });
	m_vecGame.push_back({ xLB + 3,yLB - 5 });
	m_vecGame.push_back({ xLB + 4,yLB - 1 });
	m_vecGame.push_back({ xLB + 4,yLB - 2 });
	m_vecGame.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcGame.left + 8;
	yLB = m_rcGame.bottom - 3;

	m_vecGame.push_back({ xLB + 0,yLB - 1 });
	m_vecGame.push_back({ xLB + 1,yLB - 0 });
	m_vecGame.push_back({ xLB + 1,yLB - 2 });
	m_vecGame.push_back({ xLB + 1,yLB - 4 });
	m_vecGame.push_back({ xLB + 2,yLB - 0 });
	m_vecGame.push_back({ xLB + 2,yLB - 2 });
	m_vecGame.push_back({ xLB + 2,yLB - 4 });
	m_vecGame.push_back({ xLB + 3,yLB - 0 });
	m_vecGame.push_back({ xLB + 3,yLB - 2 });
	m_vecGame.push_back({ xLB + 3,yLB - 4 });
	m_vecGame.push_back({ xLB + 4,yLB - 0 });
	m_vecGame.push_back({ xLB + 4,yLB - 1 });
	m_vecGame.push_back({ xLB + 4,yLB - 2 });
	m_vecGame.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcGame.left + 14;
	yLB = m_rcGame.bottom - 3;

	m_vecGame.push_back({ xLB + 0,yLB - 0 });
	m_vecGame.push_back({ xLB + 0,yLB - 1 });
	m_vecGame.push_back({ xLB + 0,yLB - 2 });
	m_vecGame.push_back({ xLB + 0,yLB - 3 });
	m_vecGame.push_back({ xLB + 0,yLB - 4 });
	m_vecGame.push_back({ xLB + 1,yLB - 4 });
	m_vecGame.push_back({ xLB + 2,yLB - 0 });
	m_vecGame.push_back({ xLB + 2,yLB - 1 });
	m_vecGame.push_back({ xLB + 2,yLB - 2 });
	m_vecGame.push_back({ xLB + 2,yLB - 3 });
	m_vecGame.push_back({ xLB + 3,yLB - 4 });
	m_vecGame.push_back({ xLB + 4,yLB - 0 });
	m_vecGame.push_back({ xLB + 4,yLB - 1 });
	m_vecGame.push_back({ xLB + 4,yLB - 2 });
	m_vecGame.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcGame.left + 20;
	yLB = m_rcGame.bottom - 3;

	m_vecGame.push_back({ xLB + 0,yLB - 1 });
	m_vecGame.push_back({ xLB + 0,yLB - 2 });
	m_vecGame.push_back({ xLB + 0,yLB - 3 });
	m_vecGame.push_back({ xLB + 1,yLB - 0 });
	m_vecGame.push_back({ xLB + 1,yLB - 2 });
	m_vecGame.push_back({ xLB + 1,yLB - 4 });
	m_vecGame.push_back({ xLB + 2,yLB - 0 });
	m_vecGame.push_back({ xLB + 2,yLB - 2 });
	m_vecGame.push_back({ xLB + 2,yLB - 4 });
	m_vecGame.push_back({ xLB + 3,yLB - 0 });
	m_vecGame.push_back({ xLB + 3,yLB - 2 });
	m_vecGame.push_back({ xLB + 3,yLB - 4 });
	m_vecGame.push_back({ xLB + 4,yLB - 2 });
	m_vecGame.push_back({ xLB + 4,yLB - 3 });

//	//Setting
//
//	SetRect(&m_rcSetting, 40, 120, 77, 130);
//
//	xLB = m_rcSetting.left + 2;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 5 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 5 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 5 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 4 });
//
//	xLB = m_rcSetting.left + 8;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 3 });
//
//	xLB = m_rcSetting.left + 14;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 5 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 4 });
//
//	xLB = m_rcSetting.left + 18;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 5 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 4 });
//
//	xLB = m_rcSetting.left + 22;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 6 });
//
//	xLB = m_rcSetting.left + 24;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 3 });
//
//	xLB = m_rcSetting.left + 30;
//	yLB = m_rcSetting.bottom - 3;
//
//	m_vecSetting.push_back({ xLB + 0,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 0,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 1,yLB + 1 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 1,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 2,yLB + 1 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 2,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 3,yLB + 1 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 3,yLB - 4 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 0 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 1 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 2 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 3 });
//	m_vecSetting.push_back({ xLB + 4,yLB - 4 });

	//Extra

	SetRect(&m_rcExtra, 40, 115, 72, 125);
	
	xLB = m_rcExtra.left + 2;
	yLB = m_rcExtra.bottom - 3;

	m_vecExtra.push_back({ xLB + 0,yLB - 0 });
	m_vecExtra.push_back({ xLB + 0,yLB - 1 });
	m_vecExtra.push_back({ xLB + 0,yLB - 2 });
	m_vecExtra.push_back({ xLB + 0,yLB - 3 });
	m_vecExtra.push_back({ xLB + 0,yLB - 4 });
	m_vecExtra.push_back({ xLB + 0,yLB - 5 });
	m_vecExtra.push_back({ xLB + 1,yLB - 0 });
	m_vecExtra.push_back({ xLB + 1,yLB - 3 });
	m_vecExtra.push_back({ xLB + 1,yLB - 5 });
	m_vecExtra.push_back({ xLB + 2,yLB - 0 });
	m_vecExtra.push_back({ xLB + 2,yLB - 3 });
	m_vecExtra.push_back({ xLB + 2,yLB - 5 });
	m_vecExtra.push_back({ xLB + 3,yLB - 0 });
	m_vecExtra.push_back({ xLB + 3,yLB - 3 });
	m_vecExtra.push_back({ xLB + 3,yLB - 5 });
	m_vecExtra.push_back({ xLB + 4,yLB - 0 });
	m_vecExtra.push_back({ xLB + 4,yLB - 3 });
	m_vecExtra.push_back({ xLB + 4,yLB - 5 });

	xLB = m_rcExtra.left + 8;
	yLB = m_rcExtra.bottom - 3;

	m_vecExtra.push_back({ xLB + 0,yLB - 0 });
	m_vecExtra.push_back({ xLB + 0,yLB - 4 });
	m_vecExtra.push_back({ xLB + 1,yLB - 1 });
	m_vecExtra.push_back({ xLB + 1,yLB - 3 });
	m_vecExtra.push_back({ xLB + 2,yLB - 2 });
	m_vecExtra.push_back({ xLB + 3,yLB - 1 });
	m_vecExtra.push_back({ xLB + 3,yLB - 3 });
	m_vecExtra.push_back({ xLB + 4,yLB - 0 });
	m_vecExtra.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcExtra.left + 14;
	yLB = m_rcExtra.bottom - 3;

	m_vecExtra.push_back({ xLB + 0,yLB - 4 });
	m_vecExtra.push_back({ xLB + 1,yLB - 1 });
	m_vecExtra.push_back({ xLB + 1,yLB - 2 });
	m_vecExtra.push_back({ xLB + 1,yLB - 3 });
	m_vecExtra.push_back({ xLB + 1,yLB - 4 });
	m_vecExtra.push_back({ xLB + 1,yLB - 5 });
	m_vecExtra.push_back({ xLB + 2,yLB - 0 });
	m_vecExtra.push_back({ xLB + 2,yLB - 4 });

	xLB = m_rcExtra.left + 18;
	yLB = m_rcExtra.bottom - 3;

	m_vecExtra.push_back({ xLB + 0,yLB - 0 });
	m_vecExtra.push_back({ xLB + 0,yLB - 1 });
	m_vecExtra.push_back({ xLB + 0,yLB - 2 });
	m_vecExtra.push_back({ xLB + 0,yLB - 3 });
	m_vecExtra.push_back({ xLB + 0,yLB - 4 });
	m_vecExtra.push_back({ xLB + 1,yLB - 3 });
	m_vecExtra.push_back({ xLB + 2,yLB - 4 });

	xLB = m_rcExtra.left + 21;
	yLB = m_rcExtra.bottom - 3;

	m_vecExtra.push_back({ xLB + 0,yLB - 1 });
	m_vecExtra.push_back({ xLB + 1,yLB - 0 });
	m_vecExtra.push_back({ xLB + 1,yLB - 2 });
	m_vecExtra.push_back({ xLB + 1,yLB - 4 });
	m_vecExtra.push_back({ xLB + 2,yLB - 0 });
	m_vecExtra.push_back({ xLB + 2,yLB - 2 });
	m_vecExtra.push_back({ xLB + 2,yLB - 4 });
	m_vecExtra.push_back({ xLB + 3,yLB - 0 });
	m_vecExtra.push_back({ xLB + 3,yLB - 2 });
	m_vecExtra.push_back({ xLB + 3,yLB - 4 });
	m_vecExtra.push_back({ xLB + 4,yLB - 0 });
	m_vecExtra.push_back({ xLB + 4,yLB - 1 });
	m_vecExtra.push_back({ xLB + 4,yLB - 2 });
	m_vecExtra.push_back({ xLB + 4,yLB - 3 });

	//Quit

	SetRect(&m_rcQuit, 40, 140, 72, 150);

	xLB = m_rcQuit.left + 2;
	yLB = m_rcQuit.bottom - 3;

	m_vecQuit.push_back({ xLB + 0,yLB - 1 });
	m_vecQuit.push_back({ xLB + 0,yLB - 2 });
	m_vecQuit.push_back({ xLB + 0,yLB - 3 });
	m_vecQuit.push_back({ xLB + 0,yLB - 4 });
	m_vecQuit.push_back({ xLB + 1,yLB - 0 });
	m_vecQuit.push_back({ xLB + 1,yLB - 5 });
	m_vecQuit.push_back({ xLB + 2,yLB - 0 });
	m_vecQuit.push_back({ xLB + 2,yLB - 2 });
	m_vecQuit.push_back({ xLB + 2,yLB - 5 });
	m_vecQuit.push_back({ xLB + 3,yLB - 0 });
	m_vecQuit.push_back({ xLB + 3,yLB - 1 });
	m_vecQuit.push_back({ xLB + 3,yLB - 5 });
	m_vecQuit.push_back({ xLB + 4,yLB + 1 });
	m_vecQuit.push_back({ xLB + 4,yLB - 1 });
	m_vecQuit.push_back({ xLB + 4,yLB - 2 });
	m_vecQuit.push_back({ xLB + 4,yLB - 3 });
	m_vecQuit.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcQuit.left + 8;
	yLB = m_rcQuit.bottom - 3;

	m_vecQuit.push_back({ xLB + 0,yLB - 1 });
	m_vecQuit.push_back({ xLB + 0,yLB - 2 });
	m_vecQuit.push_back({ xLB + 0,yLB - 3 });
	m_vecQuit.push_back({ xLB + 0,yLB - 4 });
	m_vecQuit.push_back({ xLB + 1,yLB - 0 });
	m_vecQuit.push_back({ xLB + 2,yLB - 0 });
	m_vecQuit.push_back({ xLB + 3,yLB - 1 });
	m_vecQuit.push_back({ xLB + 4,yLB - 0 });
	m_vecQuit.push_back({ xLB + 4,yLB - 1 });
	m_vecQuit.push_back({ xLB + 4,yLB - 2 });
	m_vecQuit.push_back({ xLB + 4,yLB - 3 });
	m_vecQuit.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcQuit.left + 14;
	yLB = m_rcQuit.bottom - 3;

	m_vecQuit.push_back({ xLB + 0,yLB - 0 });
	m_vecQuit.push_back({ xLB + 0,yLB - 1 });
	m_vecQuit.push_back({ xLB + 0,yLB - 2 });
	m_vecQuit.push_back({ xLB + 0,yLB - 3 });
	m_vecQuit.push_back({ xLB + 0,yLB - 4 });
	m_vecQuit.push_back({ xLB + 0,yLB - 6 });

	xLB = m_rcQuit.left + 16;
	yLB = m_rcQuit.bottom - 3;

	m_vecQuit.push_back({ xLB + 0,yLB - 4 });
	m_vecQuit.push_back({ xLB + 1,yLB - 1 });
	m_vecQuit.push_back({ xLB + 1,yLB - 2 });
	m_vecQuit.push_back({ xLB + 1,yLB - 3 });
	m_vecQuit.push_back({ xLB + 1,yLB - 4 });
	m_vecQuit.push_back({ xLB + 1,yLB - 5 });
	m_vecQuit.push_back({ xLB + 2,yLB - 0 });
	m_vecQuit.push_back({ xLB + 2,yLB - 4 });

	//
	//GameSelect
	//

	//Pong

	SetRect(&m_rcPong, 40, 90, 92, 100);

	xLB = m_rcPong.left + 2;
	yLB = m_rcPong.bottom - 3;

	m_vecPong.push_back({ xLB + 0,yLB - 0 });
	m_vecPong.push_back({ xLB + 0,yLB - 1 });
	m_vecPong.push_back({ xLB + 0,yLB - 2 });
	m_vecPong.push_back({ xLB + 0,yLB - 3 });
	m_vecPong.push_back({ xLB + 0,yLB - 4 });
	m_vecPong.push_back({ xLB + 0,yLB - 5 });
	m_vecPong.push_back({ xLB + 1,yLB - 2 });
	m_vecPong.push_back({ xLB + 1,yLB - 5 });
	m_vecPong.push_back({ xLB + 2,yLB - 2 });
	m_vecPong.push_back({ xLB + 2,yLB - 5 });
	m_vecPong.push_back({ xLB + 3,yLB - 2 });
	m_vecPong.push_back({ xLB + 3,yLB - 5 });
	m_vecPong.push_back({ xLB + 4,yLB - 3 });
	m_vecPong.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcPong.left + 8;
	yLB = m_rcPong.bottom - 3;

	m_vecPong.push_back({ xLB + 0,yLB - 1 });
	m_vecPong.push_back({ xLB + 0,yLB - 2 });
	m_vecPong.push_back({ xLB + 0,yLB - 3 });
	m_vecPong.push_back({ xLB + 1,yLB - 0 });
	m_vecPong.push_back({ xLB + 1,yLB - 4 });
	m_vecPong.push_back({ xLB + 2,yLB - 0 });
	m_vecPong.push_back({ xLB + 2,yLB - 4 });
	m_vecPong.push_back({ xLB + 3,yLB - 0 });
	m_vecPong.push_back({ xLB + 3,yLB - 4 });
	m_vecPong.push_back({ xLB + 4,yLB - 1 });
	m_vecPong.push_back({ xLB + 4,yLB - 2 });
	m_vecPong.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcPong.left + 14;
	yLB = m_rcPong.bottom - 3;

	m_vecPong.push_back({ xLB + 0,yLB - 0 });
	m_vecPong.push_back({ xLB + 0,yLB - 1 });
	m_vecPong.push_back({ xLB + 0,yLB - 2 });
	m_vecPong.push_back({ xLB + 0,yLB - 3 });
	m_vecPong.push_back({ xLB + 0,yLB - 4 });
	m_vecPong.push_back({ xLB + 1,yLB - 4 });
	m_vecPong.push_back({ xLB + 2,yLB - 4 });
	m_vecPong.push_back({ xLB + 3,yLB - 4 });
	m_vecPong.push_back({ xLB + 4,yLB - 0 });
	m_vecPong.push_back({ xLB + 4,yLB - 1 });
	m_vecPong.push_back({ xLB + 4,yLB - 2 });
	m_vecPong.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcPong.left + 20;
	yLB = m_rcPong.bottom - 3;

	m_vecPong.push_back({ xLB + 0,yLB - 2 });
	m_vecPong.push_back({ xLB + 0,yLB - 3 });
	m_vecPong.push_back({ xLB + 1,yLB + 1 });
	m_vecPong.push_back({ xLB + 1,yLB - 1 });
	m_vecPong.push_back({ xLB + 1,yLB - 4 });
	m_vecPong.push_back({ xLB + 2,yLB + 1 });
	m_vecPong.push_back({ xLB + 2,yLB - 1 });
	m_vecPong.push_back({ xLB + 2,yLB - 4 });
	m_vecPong.push_back({ xLB + 3,yLB + 1 });
	m_vecPong.push_back({ xLB + 3,yLB - 1 });
	m_vecPong.push_back({ xLB + 3,yLB - 4 });
	m_vecPong.push_back({ xLB + 4,yLB - 0 });
	m_vecPong.push_back({ xLB + 4,yLB - 1 });
	m_vecPong.push_back({ xLB + 4,yLB - 2 });
	m_vecPong.push_back({ xLB + 4,yLB - 3 });
	m_vecPong.push_back({ xLB + 4,yLB - 4 });

	//Racing

	SetRect(&m_rcRacing, 40, 107, 92, 117);

	xLB = m_rcRacing.left + 2;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 0 });
	m_vecRacing.push_back({ xLB + 0,yLB - 1 });
	m_vecRacing.push_back({ xLB + 0,yLB - 2 });
	m_vecRacing.push_back({ xLB + 0,yLB - 3 });
	m_vecRacing.push_back({ xLB + 0,yLB - 4 });
	m_vecRacing.push_back({ xLB + 0,yLB - 5 });
	m_vecRacing.push_back({ xLB + 1,yLB - 2 });
	m_vecRacing.push_back({ xLB + 1,yLB - 5 });
	m_vecRacing.push_back({ xLB + 2,yLB - 2 });
	m_vecRacing.push_back({ xLB + 2,yLB - 5 });
	m_vecRacing.push_back({ xLB + 3,yLB - 2 });
	m_vecRacing.push_back({ xLB + 3,yLB - 5 });
	m_vecRacing.push_back({ xLB + 4,yLB - 0 });
	m_vecRacing.push_back({ xLB + 4,yLB - 1 });
	m_vecRacing.push_back({ xLB + 4,yLB - 3 });
	m_vecRacing.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcRacing.left + 8;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 1 });
	m_vecRacing.push_back({ xLB + 1,yLB - 0 });
	m_vecRacing.push_back({ xLB + 1,yLB - 2 });
	m_vecRacing.push_back({ xLB + 1,yLB - 4 });
	m_vecRacing.push_back({ xLB + 2,yLB - 0 });
	m_vecRacing.push_back({ xLB + 2,yLB - 2 });
	m_vecRacing.push_back({ xLB + 2,yLB - 4 });
	m_vecRacing.push_back({ xLB + 3,yLB - 0 });
	m_vecRacing.push_back({ xLB + 3,yLB - 2 });
	m_vecRacing.push_back({ xLB + 3,yLB - 4 });
	m_vecRacing.push_back({ xLB + 4,yLB - 0 });
	m_vecRacing.push_back({ xLB + 4,yLB - 1 });
	m_vecRacing.push_back({ xLB + 4,yLB - 2 });
	m_vecRacing.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcRacing.left + 14;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 1 });
	m_vecRacing.push_back({ xLB + 0,yLB - 2 });
	m_vecRacing.push_back({ xLB + 0,yLB - 3 });
	m_vecRacing.push_back({ xLB + 1,yLB - 0 });
	m_vecRacing.push_back({ xLB + 1,yLB - 4 });
	m_vecRacing.push_back({ xLB + 2,yLB - 0 });
	m_vecRacing.push_back({ xLB + 2,yLB - 4 });
	m_vecRacing.push_back({ xLB + 3,yLB - 0 });
	m_vecRacing.push_back({ xLB + 3,yLB - 4 });
	m_vecRacing.push_back({ xLB + 4,yLB - 1 });
	m_vecRacing.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcRacing.left + 20;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 0 });
	m_vecRacing.push_back({ xLB + 0,yLB - 1 });
	m_vecRacing.push_back({ xLB + 0,yLB - 2 });
	m_vecRacing.push_back({ xLB + 0,yLB - 3 });
	m_vecRacing.push_back({ xLB + 0,yLB - 4 });
	m_vecRacing.push_back({ xLB + 0,yLB - 6 });

	xLB = m_rcRacing.left + 22;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 0 });
	m_vecRacing.push_back({ xLB + 0,yLB - 1 });
	m_vecRacing.push_back({ xLB + 0,yLB - 2 });
	m_vecRacing.push_back({ xLB + 0,yLB - 3 });
	m_vecRacing.push_back({ xLB + 0,yLB - 4 });
	m_vecRacing.push_back({ xLB + 1,yLB - 4 });
	m_vecRacing.push_back({ xLB + 2,yLB - 4 });
	m_vecRacing.push_back({ xLB + 3,yLB - 4 });
	m_vecRacing.push_back({ xLB + 4,yLB - 0 });
	m_vecRacing.push_back({ xLB + 4,yLB - 1 });
	m_vecRacing.push_back({ xLB + 4,yLB - 2 });
	m_vecRacing.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcRacing.left + 28;
	yLB = m_rcRacing.bottom - 3;

	m_vecRacing.push_back({ xLB + 0,yLB - 2 });
	m_vecRacing.push_back({ xLB + 0,yLB - 3 });
	m_vecRacing.push_back({ xLB + 1,yLB + 1 });
	m_vecRacing.push_back({ xLB + 1,yLB - 1 });
	m_vecRacing.push_back({ xLB + 1,yLB - 4 });
	m_vecRacing.push_back({ xLB + 2,yLB + 1 });
	m_vecRacing.push_back({ xLB + 2,yLB - 1 });
	m_vecRacing.push_back({ xLB + 2,yLB - 4 });
	m_vecRacing.push_back({ xLB + 3,yLB + 1 });
	m_vecRacing.push_back({ xLB + 3,yLB - 1 });
	m_vecRacing.push_back({ xLB + 3,yLB - 4 });
	m_vecRacing.push_back({ xLB + 4,yLB - 0 });
	m_vecRacing.push_back({ xLB + 4,yLB - 1 });
	m_vecRacing.push_back({ xLB + 4,yLB - 2 });
	m_vecRacing.push_back({ xLB + 4,yLB - 3 });
	m_vecRacing.push_back({ xLB + 4,yLB - 4 });

	//Tetris

	SetRect(&m_rcTetris, 40, 124, 92, 134);

	xLB = m_rcTetris.left + 2;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 5 });
	m_vecTetris.push_back({ xLB + 1,yLB - 5 });
	m_vecTetris.push_back({ xLB + 2,yLB - 0 });
	m_vecTetris.push_back({ xLB + 2,yLB - 1 });
	m_vecTetris.push_back({ xLB + 2,yLB - 2 });
	m_vecTetris.push_back({ xLB + 2,yLB - 3 });
	m_vecTetris.push_back({ xLB + 2,yLB - 4 });
	m_vecTetris.push_back({ xLB + 2,yLB - 5 });
	m_vecTetris.push_back({ xLB + 3,yLB - 5 });
	m_vecTetris.push_back({ xLB + 4,yLB - 5 });

	xLB = m_rcTetris.left + 8;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 1 });
	m_vecTetris.push_back({ xLB + 0,yLB - 2 });
	m_vecTetris.push_back({ xLB + 0,yLB - 3 });
	m_vecTetris.push_back({ xLB + 1,yLB - 0 });
	m_vecTetris.push_back({ xLB + 1,yLB - 2 });
	m_vecTetris.push_back({ xLB + 1,yLB - 4 });
	m_vecTetris.push_back({ xLB + 2,yLB - 0 });
	m_vecTetris.push_back({ xLB + 2,yLB - 2 });
	m_vecTetris.push_back({ xLB + 2,yLB - 4 });
	m_vecTetris.push_back({ xLB + 3,yLB - 0 });
	m_vecTetris.push_back({ xLB + 3,yLB - 2 });
	m_vecTetris.push_back({ xLB + 3,yLB - 4 });
	m_vecTetris.push_back({ xLB + 4,yLB - 2 });
	m_vecTetris.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcTetris.left + 14;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 4 });
	m_vecTetris.push_back({ xLB + 1,yLB - 1 });
	m_vecTetris.push_back({ xLB + 1,yLB - 2 });
	m_vecTetris.push_back({ xLB + 1,yLB - 3 });
	m_vecTetris.push_back({ xLB + 1,yLB - 4 });
	m_vecTetris.push_back({ xLB + 1,yLB - 5 });
	m_vecTetris.push_back({ xLB + 2,yLB - 0 });
	m_vecTetris.push_back({ xLB + 2,yLB - 4 });

	xLB = m_rcTetris.left + 18;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 0 });
	m_vecTetris.push_back({ xLB + 0,yLB - 1 });
	m_vecTetris.push_back({ xLB + 0,yLB - 2 });
	m_vecTetris.push_back({ xLB + 0,yLB - 3 });
	m_vecTetris.push_back({ xLB + 0,yLB - 4 });
	m_vecTetris.push_back({ xLB + 1,yLB - 3 });
	m_vecTetris.push_back({ xLB + 2,yLB - 4 });
	m_vecTetris.push_back({ xLB + 3,yLB - 4 });

	xLB = m_rcTetris.left + 23;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 0 });
	m_vecTetris.push_back({ xLB + 0,yLB - 1 });
	m_vecTetris.push_back({ xLB + 0,yLB - 2 });
	m_vecTetris.push_back({ xLB + 0,yLB - 3 });
	m_vecTetris.push_back({ xLB + 0,yLB - 4 });
	m_vecTetris.push_back({ xLB + 0,yLB - 6 });

	xLB = m_rcTetris.left + 25;
	yLB = m_rcTetris.bottom - 3;

	m_vecTetris.push_back({ xLB + 0,yLB - 0 });
	m_vecTetris.push_back({ xLB + 0,yLB - 3 });
	m_vecTetris.push_back({ xLB + 1,yLB - 0 });
	m_vecTetris.push_back({ xLB + 1,yLB - 2 });
	m_vecTetris.push_back({ xLB + 1,yLB - 4 });
	m_vecTetris.push_back({ xLB + 2,yLB - 0 });
	m_vecTetris.push_back({ xLB + 2,yLB - 2 });
	m_vecTetris.push_back({ xLB + 2,yLB - 4 });
	m_vecTetris.push_back({ xLB + 3,yLB - 0 });
	m_vecTetris.push_back({ xLB + 3,yLB - 2 });
	m_vecTetris.push_back({ xLB + 3,yLB - 4 });
	m_vecTetris.push_back({ xLB + 4,yLB - 1 });
	m_vecTetris.push_back({ xLB + 4,yLB - 4 });
	
	//Snake

	SetRect(&m_rcSnake, 40, 141, 92, 151);

	xLB = m_rcSnake.left + 2;
	yLB = m_rcSnake.bottom - 3;

	m_vecSnake.push_back({ xLB + 0,yLB - 1 });
	m_vecSnake.push_back({ xLB + 0,yLB - 4 });
	m_vecSnake.push_back({ xLB + 1,yLB - 0 });
	m_vecSnake.push_back({ xLB + 1,yLB - 3 });
	m_vecSnake.push_back({ xLB + 1,yLB - 5 });
	m_vecSnake.push_back({ xLB + 2,yLB - 0 });
	m_vecSnake.push_back({ xLB + 2,yLB - 3 });
	m_vecSnake.push_back({ xLB + 2,yLB - 5 });
	m_vecSnake.push_back({ xLB + 3,yLB - 0 });
	m_vecSnake.push_back({ xLB + 3,yLB - 2 });
	m_vecSnake.push_back({ xLB + 3,yLB - 5 });
	m_vecSnake.push_back({ xLB + 4,yLB - 1 });
	m_vecSnake.push_back({ xLB + 4,yLB - 4 });

	xLB = m_rcSnake.left + 8;
	yLB = m_rcSnake.bottom - 3;

	m_vecSnake.push_back({ xLB + 0,yLB - 0 });
	m_vecSnake.push_back({ xLB + 0,yLB - 1 });
	m_vecSnake.push_back({ xLB + 0,yLB - 2 });
	m_vecSnake.push_back({ xLB + 0,yLB - 3 });
	m_vecSnake.push_back({ xLB + 0,yLB - 4 });
	m_vecSnake.push_back({ xLB + 1,yLB - 4 });
	m_vecSnake.push_back({ xLB + 2,yLB - 4 });
	m_vecSnake.push_back({ xLB + 3,yLB - 4 });
	m_vecSnake.push_back({ xLB + 4,yLB - 0 });
	m_vecSnake.push_back({ xLB + 4,yLB - 1 });
	m_vecSnake.push_back({ xLB + 4,yLB - 2 });
	m_vecSnake.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcSnake.left + 14;
	yLB = m_rcSnake.bottom - 3;

	m_vecSnake.push_back({ xLB + 0,yLB - 1 });
	m_vecSnake.push_back({ xLB + 1,yLB - 0 });
	m_vecSnake.push_back({ xLB + 1,yLB - 2 });
	m_vecSnake.push_back({ xLB + 1,yLB - 4 });
	m_vecSnake.push_back({ xLB + 2,yLB - 0 });
	m_vecSnake.push_back({ xLB + 2,yLB - 2 });
	m_vecSnake.push_back({ xLB + 2,yLB - 4 });
	m_vecSnake.push_back({ xLB + 3,yLB - 0 });
	m_vecSnake.push_back({ xLB + 3,yLB - 2 });
	m_vecSnake.push_back({ xLB + 3,yLB - 4 });
	m_vecSnake.push_back({ xLB + 4,yLB - 0 });
	m_vecSnake.push_back({ xLB + 4,yLB - 1 });
	m_vecSnake.push_back({ xLB + 4,yLB - 2 });
	m_vecSnake.push_back({ xLB + 4,yLB - 3 });

	xLB = m_rcSnake.left + 20;
	yLB = m_rcSnake.bottom - 3;

	m_vecSnake.push_back({ xLB + 0,yLB - 0 });
	m_vecSnake.push_back({ xLB + 0,yLB - 1 });
	m_vecSnake.push_back({ xLB + 0,yLB - 2 });
	m_vecSnake.push_back({ xLB + 0,yLB - 3 });
	m_vecSnake.push_back({ xLB + 0,yLB - 4 });
	m_vecSnake.push_back({ xLB + 0,yLB - 5 });
	m_vecSnake.push_back({ xLB + 1,yLB - 2 });
	m_vecSnake.push_back({ xLB + 2,yLB - 1 });
	m_vecSnake.push_back({ xLB + 2,yLB - 3 });
	m_vecSnake.push_back({ xLB + 3,yLB - 0 });
	m_vecSnake.push_back({ xLB + 3,yLB - 4 });

	xLB = m_rcSnake.left + 25;
	yLB = m_rcSnake.bottom - 3;

	m_vecSnake.push_back({ xLB + 0,yLB - 1 });
	m_vecSnake.push_back({ xLB + 0,yLB - 2 });
	m_vecSnake.push_back({ xLB + 0,yLB - 3 });
	m_vecSnake.push_back({ xLB + 1,yLB - 0 });
	m_vecSnake.push_back({ xLB + 1,yLB - 2 });
	m_vecSnake.push_back({ xLB + 1,yLB - 4 });
	m_vecSnake.push_back({ xLB + 2,yLB - 0 });
	m_vecSnake.push_back({ xLB + 2,yLB - 2 });
	m_vecSnake.push_back({ xLB + 2,yLB - 4 });
	m_vecSnake.push_back({ xLB + 3,yLB - 0 });
	m_vecSnake.push_back({ xLB + 3,yLB - 2 });
	m_vecSnake.push_back({ xLB + 3,yLB - 4 });
	m_vecSnake.push_back({ xLB + 4,yLB - 2 });
	m_vecSnake.push_back({ xLB + 4,yLB - 3 });
	
	//Despair logo
	
	SetRect(&m_rcDespair, CXPIX / 2, 0, CXPIX, CYPIX);

	xLB = m_rcDespair.left + 18;
	yLB = m_rcDespair.bottom / 2 + 21;

	m_vecDespair.push_back({ xLB + 0,yLB - 8 });
	m_vecDespair.push_back({ xLB + 1,yLB - 9 });
	m_vecDespair.push_back({ xLB + 2,yLB - 9 });
	m_vecDespair.push_back({ xLB + 3,yLB - 10 });
	m_vecDespair.push_back({ xLB + 4,yLB - 10 });
	m_vecDespair.push_back({ xLB + 4,yLB - 11 });
	m_vecDespair.push_back({ xLB + 5,yLB - 10 });
	m_vecDespair.push_back({ xLB + 5,yLB - 12 });
	m_vecDespair.push_back({ xLB + 6,yLB - 11 });
	m_vecDespair.push_back({ xLB + 6,yLB - 13 });
	m_vecDespair.push_back({ xLB + 7,yLB - 11 });
	m_vecDespair.push_back({ xLB + 7,yLB - 14 });
	m_vecDespair.push_back({ xLB + 8,yLB - 11 });
	m_vecDespair.push_back({ xLB + 8,yLB - 15 });
	m_vecDespair.push_back({ xLB + 9,yLB - 12 });
	m_vecDespair.push_back({ xLB + 9,yLB - 16 });
	m_vecDespair.push_back({ xLB + 10,yLB - 12 });
	m_vecDespair.push_back({ xLB + 10,yLB - 17 });
	m_vecDespair.push_back({ xLB + 11,yLB - 12 });
	m_vecDespair.push_back({ xLB + 11,yLB - 18 });
	m_vecDespair.push_back({ xLB + 12,yLB - 13 });
	m_vecDespair.push_back({ xLB + 12,yLB - 19 });
	m_vecDespair.push_back({ xLB + 13,yLB - 13 });
	m_vecDespair.push_back({ xLB + 13,yLB - 20 });
	m_vecDespair.push_back({ xLB + 14,yLB - 13 });
	m_vecDespair.push_back({ xLB + 14,yLB - 21 });
	m_vecDespair.push_back({ xLB + 15,yLB - 13 });
	m_vecDespair.push_back({ xLB + 15,yLB - 22 });
	m_vecDespair.push_back({ xLB + 16,yLB - 14 });
	m_vecDespair.push_back({ xLB + 16,yLB - 23 });
	m_vecDespair.push_back({ xLB + 17,yLB - 14 });
	m_vecDespair.push_back({ xLB + 17,yLB - 24 });
	m_vecDespair.push_back({ xLB + 18,yLB - 14 });
	m_vecDespair.push_back({ xLB + 18,yLB - 25 });
	m_vecDespair.push_back({ xLB + 19,yLB - 14 });
	m_vecDespair.push_back({ xLB + 19,yLB - 26 });
	m_vecDespair.push_back({ xLB + 20,yLB - 14 });
	m_vecDespair.push_back({ xLB + 20,yLB - 27 });
	m_vecDespair.push_back({ xLB + 21,yLB - 14 });
	m_vecDespair.push_back({ xLB + 21,yLB - 28 });
	m_vecDespair.push_back({ xLB + 21,yLB - 29 });
	m_vecDespair.push_back({ xLB + 22,yLB - 13 });
	m_vecDespair.push_back({ xLB + 22,yLB - 27 });
	m_vecDespair.push_back({ xLB + 22,yLB - 28 });
	m_vecDespair.push_back({ xLB + 22,yLB - 29 });
	m_vecDespair.push_back({ xLB + 22,yLB - 30 });
	m_vecDespair.push_back({ xLB + 22,yLB - 31 });
	m_vecDespair.push_back({ xLB + 23,yLB - 13 });
	m_vecDespair.push_back({ xLB + 23,yLB - 24 });
	m_vecDespair.push_back({ xLB + 23,yLB - 25 });
	m_vecDespair.push_back({ xLB + 23,yLB - 26 });
	m_vecDespair.push_back({ xLB + 24,yLB - 12 });
	m_vecDespair.push_back({ xLB + 24,yLB - 22 });
	m_vecDespair.push_back({ xLB + 24,yLB - 23 });
	m_vecDespair.push_back({ xLB + 25,yLB - 11 });
	m_vecDespair.push_back({ xLB + 25,yLB - 20 });
	m_vecDespair.push_back({ xLB + 25,yLB - 21 });
	m_vecDespair.push_back({ xLB + 26,yLB - 9 });
	m_vecDespair.push_back({ xLB + 26,yLB - 10 });
	m_vecDespair.push_back({ xLB + 26,yLB - 19 });
	m_vecDespair.push_back({ xLB + 27,yLB - 7 });
	m_vecDespair.push_back({ xLB + 27,yLB - 8 });
	m_vecDespair.push_back({ xLB + 27,yLB - 18 });
	m_vecDespair.push_back({ xLB + 28,yLB - 3 });
	m_vecDespair.push_back({ xLB + 28,yLB - 4 });
	m_vecDespair.push_back({ xLB + 28,yLB - 5 });
	m_vecDespair.push_back({ xLB + 28,yLB - 6 });
	m_vecDespair.push_back({ xLB + 28,yLB - 18 });
	m_vecDespair.push_back({ xLB + 29,yLB - 5 });
	m_vecDespair.push_back({ xLB + 29,yLB - 17 });
	m_vecDespair.push_back({ xLB + 30,yLB - 6 });
	m_vecDespair.push_back({ xLB + 30,yLB - 17 });
	m_vecDespair.push_back({ xLB + 31,yLB - 7 });
	m_vecDespair.push_back({ xLB + 31,yLB - 17 });
	m_vecDespair.push_back({ xLB + 32,yLB - 8 });
	m_vecDespair.push_back({ xLB + 32,yLB - 17 });
	m_vecDespair.push_back({ xLB + 33,yLB - 9 });
	m_vecDespair.push_back({ xLB + 33,yLB - 17 });
	m_vecDespair.push_back({ xLB + 34,yLB - 9 });
	m_vecDespair.push_back({ xLB + 34,yLB - 17 });
	m_vecDespair.push_back({ xLB + 35,yLB - 10 });
	m_vecDespair.push_back({ xLB + 35,yLB - 17 });
	m_vecDespair.push_back({ xLB + 36,yLB - 10 });
	m_vecDespair.push_back({ xLB + 36,yLB - 17 });
	m_vecDespair.push_back({ xLB + 37,yLB - 11 });
	m_vecDespair.push_back({ xLB + 37,yLB - 18 });
	m_vecDespair.push_back({ xLB + 38,yLB - 11 });
	m_vecDespair.push_back({ xLB + 38,yLB - 18 });
	m_vecDespair.push_back({ xLB + 39,yLB - 11 });
	m_vecDespair.push_back({ xLB + 39,yLB - 18 });
	m_vecDespair.push_back({ xLB + 40,yLB - 11 });
	m_vecDespair.push_back({ xLB + 40,yLB - 18 });
	m_vecDespair.push_back({ xLB + 41,yLB - 12 });
	m_vecDespair.push_back({ xLB + 41,yLB - 18 });
	m_vecDespair.push_back({ xLB + 42,yLB - 12 });
	m_vecDespair.push_back({ xLB + 42,yLB - 19 });
	m_vecDespair.push_back({ xLB + 43,yLB - 12 });
	m_vecDespair.push_back({ xLB + 43,yLB - 19 });
	m_vecDespair.push_back({ xLB + 44,yLB - 12 });
	m_vecDespair.push_back({ xLB + 44,yLB - 19 });
	m_vecDespair.push_back({ xLB + 45,yLB - 12 });
	m_vecDespair.push_back({ xLB + 45,yLB - 19 });
	m_vecDespair.push_back({ xLB + 46,yLB - 12 });
	m_vecDespair.push_back({ xLB + 46,yLB - 20 });
	m_vecDespair.push_back({ xLB + 47,yLB - 12 });
	m_vecDespair.push_back({ xLB + 47,yLB - 20 });
	m_vecDespair.push_back({ xLB + 48,yLB - 12 });
	m_vecDespair.push_back({ xLB + 48,yLB - 20 });
	m_vecDespair.push_back({ xLB + 49,yLB - 12 });
	m_vecDespair.push_back({ xLB + 49,yLB - 21 });
	m_vecDespair.push_back({ xLB + 50,yLB - 12 });
	m_vecDespair.push_back({ xLB + 50,yLB - 21 });
	m_vecDespair.push_back({ xLB + 51,yLB - 11 });
	m_vecDespair.push_back({ xLB + 51,yLB - 22 });
	m_vecDespair.push_back({ xLB + 52,yLB - 11 });
	m_vecDespair.push_back({ xLB + 52,yLB - 22 });
	m_vecDespair.push_back({ xLB + 53,yLB - 11 });
	m_vecDespair.push_back({ xLB + 53,yLB - 23 });
	m_vecDespair.push_back({ xLB + 54,yLB - 10 });
	m_vecDespair.push_back({ xLB + 54,yLB - 23 });
	m_vecDespair.push_back({ xLB + 55,yLB - 10 });
	m_vecDespair.push_back({ xLB + 55,yLB - 24 });
	m_vecDespair.push_back({ xLB + 56,yLB - 9 });
	m_vecDespair.push_back({ xLB + 56,yLB - 25 });
	m_vecDespair.push_back({ xLB + 57,yLB - 8 });
	m_vecDespair.push_back({ xLB + 57,yLB - 26 });
	m_vecDespair.push_back({ xLB + 58,yLB - 6 });
	m_vecDespair.push_back({ xLB + 58,yLB - 7 });
	m_vecDespair.push_back({ xLB + 58,yLB - 27 });
	m_vecDespair.push_back({ xLB + 59,yLB - 1 });
	m_vecDespair.push_back({ xLB + 59,yLB - 2 });
	m_vecDespair.push_back({ xLB + 59,yLB - 3 });
	m_vecDespair.push_back({ xLB + 59,yLB - 4 });
	m_vecDespair.push_back({ xLB + 59,yLB - 5 });
	m_vecDespair.push_back({ xLB + 59,yLB - 28 });
	m_vecDespair.push_back({ xLB + 60,yLB - 2 });
	m_vecDespair.push_back({ xLB + 60,yLB - 29 });
	m_vecDespair.push_back({ xLB + 61,yLB - 3 });
	m_vecDespair.push_back({ xLB + 61,yLB - 30 });
	m_vecDespair.push_back({ xLB + 62,yLB - 4 });
	m_vecDespair.push_back({ xLB + 62,yLB - 5 });
	m_vecDespair.push_back({ xLB + 62,yLB - 31 });
	m_vecDespair.push_back({ xLB + 63,yLB - 6 });
	m_vecDespair.push_back({ xLB + 63,yLB - 7 });
	m_vecDespair.push_back({ xLB + 63,yLB - 32 });
	m_vecDespair.push_back({ xLB + 64,yLB - 8 });
	m_vecDespair.push_back({ xLB + 64,yLB - 9 });
	m_vecDespair.push_back({ xLB + 64,yLB - 33 });
	m_vecDespair.push_back({ xLB + 65,yLB - 10 });
	m_vecDespair.push_back({ xLB + 65,yLB - 11 });
	m_vecDespair.push_back({ xLB + 65,yLB - 34 });
	m_vecDespair.push_back({ xLB + 66,yLB - 12 });
	m_vecDespair.push_back({ xLB + 66,yLB - 13 });
	m_vecDespair.push_back({ xLB + 66,yLB - 35 });
	m_vecDespair.push_back({ xLB + 67,yLB - 14 });
	m_vecDespair.push_back({ xLB + 67,yLB - 15 });
	m_vecDespair.push_back({ xLB + 67,yLB - 16 });
	m_vecDespair.push_back({ xLB + 67,yLB - 36 });
	m_vecDespair.push_back({ xLB + 68,yLB - 17 });
	m_vecDespair.push_back({ xLB + 68,yLB - 18 });
	m_vecDespair.push_back({ xLB + 68,yLB - 19 });
	m_vecDespair.push_back({ xLB + 68,yLB - 37 });
	m_vecDespair.push_back({ xLB + 69,yLB - 20 });
	m_vecDespair.push_back({ xLB + 69,yLB - 21 });
	m_vecDespair.push_back({ xLB + 69,yLB - 22 });
	m_vecDespair.push_back({ xLB + 69,yLB - 23 });
	m_vecDespair.push_back({ xLB + 69,yLB - 24 });
	m_vecDespair.push_back({ xLB + 69,yLB - 25 });
	m_vecDespair.push_back({ xLB + 69,yLB - 38 });
	m_vecDespair.push_back({ xLB + 70,yLB - 26 });
	m_vecDespair.push_back({ xLB + 70,yLB - 27 });
	m_vecDespair.push_back({ xLB + 70,yLB - 28 });
	m_vecDespair.push_back({ xLB + 70,yLB - 29 });
	m_vecDespair.push_back({ xLB + 70,yLB - 30 });
	m_vecDespair.push_back({ xLB + 70,yLB - 31 });
	m_vecDespair.push_back({ xLB + 70,yLB - 39 });
	m_vecDespair.push_back({ xLB + 70,yLB - 40 });
	m_vecDespair.push_back({ xLB + 71,yLB - 32 });
	m_vecDespair.push_back({ xLB + 71,yLB - 33 });
	m_vecDespair.push_back({ xLB + 71,yLB - 34 });
	m_vecDespair.push_back({ xLB + 71,yLB - 35 });
	m_vecDespair.push_back({ xLB + 71,yLB - 36 });
	m_vecDespair.push_back({ xLB + 71,yLB - 37 });
	m_vecDespair.push_back({ xLB + 71,yLB - 38 });
	m_vecDespair.push_back({ xLB + 71,yLB - 39 });
	m_vecDespair.push_back({ xLB + 71,yLB - 40 });
	m_vecDespair.push_back({ xLB + 71,yLB - 41 });

	for (size_t i = 0; i < m_vecDespair.size(); i++)
	{
		m_vecDespairOffset.push_back((rand() % 6 + 3) * (rand() % 2 * 2 - 1));

		while ((i >= 1 && m_vecDespairOffset[i] == m_vecDespairOffset[i - 1]) || 
			(i >= 2 && m_vecDespairOffset[i] == m_vecDespairOffset[i - 2]) ||
			(i >= 3 && m_vecDespairOffset[i] == m_vecDespairOffset[i - 3]))
			m_vecDespairOffset[i] = (rand() % 6 + 3) * (rand() % 2 * 2 - 1);
	}
}

void SceneMenu::SetButton(int iMenu, int iButton, BUTTONSTATE state, bool bRefresh)
{
	RECT *rcButton;
	std::vector<PIX> *vecButton;

	switch (iMenu)
	{
	case 0:
		switch (iButton)
		{
		case 0:
			rcButton = &m_rcGame;
			vecButton = &m_vecGame;
			break;
		case 1:
			rcButton = &m_rcExtra;
			vecButton = &m_vecExtra;
			break;
		case 2:
			rcButton = &m_rcQuit;
			vecButton = &m_vecQuit;
			break;
		}
		break;
	case 1:
		switch (iButton)
		{
		case 0:
			rcButton = &m_rcPong;
			vecButton = &m_vecPong;
			break;
		case 1:
			rcButton = &m_rcRacing;
			vecButton = &m_vecRacing;
			break;
		case 2:
			rcButton = &m_rcTetris;
			vecButton = &m_vecTetris;
			break;
		case 3:
			rcButton = &m_rcSnake;
			vecButton = &m_vecSnake;
			break;
		}
		break;
	}

	switch (state)
	{
	case NORMAL:
		GetScrn().Set(false, WHITE, rcButton);
		GetScrn().Set(true, WHITE, vecButton);
		break;
	case FOCUS:
		GetScrn().Set(true, WHITE, rcButton);
		if (m_iMenu == 1)
		{
			RECT rcRevise;
			SetRect(&rcRevise, rcButton->left + 36, rcButton->top, rcButton->right, rcButton->bottom);
			GetScrn().Set(false, WHITE, &rcRevise);
			SetRect(&rcRevise, rcButton->left + 38, rcButton->top, rcButton->left + 39, rcButton->bottom);
			GetScrn().Set(true, WHITE, &rcRevise);
		}
//		for (int y = rcButton->top; y < rcButton->bottom - 1; y++)
//		{
//			GetScrn().Set(false, WHITE, rcButton->right - 2, y);
//			GetScrn().Set(false, WHITE, rcButton->right - 5, y);
//			GetScrn().Set(false, WHITE, rcButton->right - 9, y);
//		}
//		for (int x = rcButton->right - 9; x < rcButton->right; x++)
//		{
//			GetScrn().Set(false, WHITE, x, rcButton->top);
//
//			if (x >= rcButton->right - 5)
//			{
//				GetScrn().Set(false, WHITE, x, rcButton->top + 1);
//				GetScrn().Set(false, WHITE, x, rcButton->top + 2);
//				GetScrn().Set(false, WHITE, x, rcButton->bottom - 1);
//			}
//
//			if (x >= rcButton->right - 2)
//			{
//				GetScrn().Set(false, WHITE, x, rcButton->top + 3);
//				GetScrn().Set(false, WHITE, x, rcButton->top + 4);
//				GetScrn().Set(false, WHITE, x, rcButton->top + 5);
//				GetScrn().Set(false, WHITE, x, rcButton->bottom - 2);
//				GetScrn().Set(false, WHITE, x, rcButton->bottom - 3);
//			}
//		}
		GetScrn().Set(false, WHITE, vecButton);
		break;
	case PRESSED:
		GetScrn().Set(true, WHITE, rcButton);
		GetScrn().Set(false, WHITE, vecButton);
		break;
	case HIDDEN:		
		GetScrn().Set(false, WHITE, rcButton);
		break;
	}

	if (bRefresh)
		GetScrn().Refresh(BRUSH, rcButton);
}

void SceneMenu::SetButton(BUTTONSTATE state, bool bRefresh)
{
	SetButton(m_iMenu, m_iFocus, state, bRefresh);
}

void SceneMenu::SetMenu(int iMenu)
{
	switch (iMenu)
	{
	case 0:
		GetScrn().Set(true, WHITE, &m_vecGame);
		GetScrn().Set(true, WHITE, &m_vecExtra);
		GetScrn().Set(true, WHITE, &m_vecQuit);
		break;
	case 1:
		GetScrn().Set(true, WHITE, &m_vecPong);
		GetScrn().Set(true, WHITE, &m_vecRacing);
		GetScrn().Set(true, WHITE, &m_vecTetris);
		GetScrn().Set(true, WHITE, &m_vecSnake);
		break;
	case 2:
		GetScrn().Set(true, WHITE, &m_vecPong);
		GetScrn().Set(true, WHITE, &m_vecRacing);
		GetScrn().Set(true, WHITE, &m_vecTetris);
		GetScrn().Set(true, WHITE, &m_vecSnake);
		break;
	}

	GetScrn().Refresh(BRUSH, &m_rcMenu);
}

void SceneMenu::SetMenu()
{
	SetMenu(m_iMenu);
}

void SceneMenu::SetStage(int iStage, bool bOn)
{
	RECT rcButton;

	switch (m_iFocus)
	{
	case 0:
		rcButton = m_rcPong;
		break;
	case 1:
		rcButton = m_rcRacing;
		break;
	case 2:
		rcButton = m_rcTetris;
		break;
	case 3:
		rcButton = m_rcSnake;
		break;
	}

	rcButton.left = rcButton.left + 36 + 2 * iStage;
	rcButton.right = rcButton.left + 1;
	rcButton.top += m_iStage - 1;

	GetScrn().Set(bOn, WHITE, &rcButton);
	GetScrn().Refresh(BRUSH, &rcButton);
}