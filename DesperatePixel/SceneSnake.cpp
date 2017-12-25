#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "SceneSnake.h"

SceneSnake::SceneSnake()
{

}

SceneSnake::~SceneSnake()
{

}

void SceneSnake::Init(int iInitVal)
{
	GetScrn().Set(false, WHITE);
	GetScrn().Refresh(BRUSH, NULL);

	m_bSceneOver = false;
	m_bFieldInit = false;
	m_bStageInit = false;
	m_bStageShow = false;
	m_bPlay = false;
	m_bBeginShow = true;
	m_bEndShow = false;
	m_bNextSent = false;
	m_bWin = false;
	m_bLose = false;
	m_bEnding = false;
	m_timeShow = 0.0f;

	m_iStage = iInitVal;
	m_iStageOffset = 0;
	m_pxEnemy = 0;
	m_pxFriend = CXPIX / 4;

	m_fRate = 1.0f;
	m_bRefresh = false;
	m_bHighFPS = GetSettings()._iHighFPS;

	for (auto &bUp : m_bKeyUp)
		bUp = false;
	for (auto &fUp : m_fKeyUp)
		fUp = 100.0f;

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);
	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 10 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);

	//Sound
	m_pBGM = new Sound();
	m_pBGM->Register(ID_BGM_SNAKE, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE1, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE2, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE3, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE4, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE5, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE6, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE7, 0);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE8, 0);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_SNAKE, ID_SUBMIX_SOUNDEFFECT);
	m_pSE->AddSoundFromRsrc(IDR_BOOM3);
	m_pSE->AddSoundFromRsrc(IDR_GET);

	m_bFade = false;
	m_timeFade = 2000.0f;
	m_iCurBGM = m_iStage - (m_iStage > 4 ? 0 : 1);
}

void SceneSnake::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneSnake::OnResetDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 10 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);

	if (m_iStage != 7)
	{
		SetPixRect(&m_rcTextTop, 0, 0, CXPIX, m_pyField - 1);
		SetPixRect(&m_rcTextBottom, 0, CYPIX - m_pyField + 1, CXPIX, CYPIX);
	}
	else
	{
		SetPixRect(&m_rcTextTop, 0, 0, CXPIX, CYPIX / 2);
		SetPixRect(&m_rcTextBottom, 0, CYPIX / 2, CXPIX, CYPIX);
	}
}

void SceneSnake::OnLostDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneSnake::Update(double fTime, float fElapsedTime)
{
	GetInput();

	if (GAME_PAUSE == GetMode())
	{
		if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
		{
			if (m_bKeyUp[5])
			{
				SetMode(GAME_PLAY);
				DXAudioPlay();
			}
			m_bKeyUp[5] = false;
		}
		else
			m_bKeyUp[5] = true;

		if (IsKeyDown(DIK_ESCAPE))
		{
			if (m_bKeyUp[6])
			{
				SetMode(GAME_PLAY);
				DXAudioPlay();
			}
			m_bKeyUp[6] = false;
		}
		else
			m_bKeyUp[6] = true;

		if (IsKeyDown(DIK_X))
		{
			SetMode(GAME_MENU);
			GetScrn().Set(false, WHITE);
			m_bSceneOver = true;
			DXAudioEnd();
		}

		return;
	}

	if (m_bFade)
	{
		m_timeFade -= fElapsedTime;

		if (m_timeFade > 0.0f)
			m_pBGM->SetVol(m_timeFade / 2000.0f);

		if (m_timeFade <= 0.0f)
		{
			m_timeFade = 2000.0f;
			m_pBGM->End();
			m_bFade = false;
		}
	}

	if (GetScrn().IsRefOver())
	{
		SoundState state;
		m_pBGM->GetState(&state);
		if (0 == state.SourceCount || 0 == state.SourceBufferCount[0])
			m_pBGM->Play(m_iCurBGM);

		if (m_bHighFPS != (bool)GetSettings()._iHighFPS)
		{
			m_bHighFPS = GetSettings()._iHighFPS;
			Refresh();
			return;
		}

		if (m_bSceneOver)
		{
			SetScene(MENU);
			SetInitVal(1);
			return;
		}

		if (m_bLose)
		{
			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (m_bKeyUp[5])
				{
					m_bFieldInit = false;
					m_bStageInit = false;
					m_bStageShow = false;
					m_bPlay = false;
					m_bEndShow = false;
					m_bLose = false;
					m_timeShow = 0.0f;
				}
				m_bKeyUp[5] = false;
			}
			else
				m_bKeyUp[5] = true;

			if (IsKeyDown(DIK_X))
			{
				SetMode(GAME_MENU);
				GetScrn().Set(false, WHITE);
				m_bSceneOver = true;
			}

			return;
		}

		if (m_bWin)
		{
			if (GetSaveData()._iStage[3] <= m_iStage)
			{
				GetSaveData()._iStage[3] = min(7, m_iStage + 1);
				WriteSaveData();
			}

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (m_bKeyUp[5])
				{
					if (m_iStage == 7)
					{
						SetMode(GAME_MENU);
						GetScrn().Set(false, WHITE);
						m_bSceneOver = true;
					}
					else
					{
						m_iStage++;
						m_bFieldInit = false;
						m_bStageInit = false;
						m_bStageShow = false;
						m_bPlay = false;
						m_bBeginShow = true;
						m_bEndShow = false;
						m_bWin = false;
						m_timeShow = 0.0f;
						if (m_iStage != 5)
							m_bFade = true;

						RECT rcNew = { 0,0,0,0 };
						switch (m_iStage)
						{
						case 2:
							SetRect(&rcNew, CXPIX / 2 - 1, CYPIX / 2 - 1, CXPIX / 2 + 1, CYPIX / 2 + 1);
							break;
						case 3:
							SetRect(&rcNew, CXPIX / 2 - 2, CYPIX / 2 - 2, CXPIX / 2 + 2, CYPIX / 2 + 2);
							break;
						case 4:
							SetRect(&rcNew, CXPIX / 2 - 2, CYPIX / 2 - 2, CXPIX / 2 + 2, CYPIX / 2 + 2);
							break;
						case 5:
							SetRect(&rcNew, CXPIX / 2 - 2, CYPIX / 2 - 2, CXPIX / 2 + 2, CYPIX / 2 + 2);
							break;
						case 6:
							SetRect(&rcNew, CXPIX / 2 - 4, CYPIX / 2 - 4, CXPIX / 2 + 4, CYPIX / 2 + 4);
							break;
						case 7:
							SetRect(&rcNew, CXPIX / 2, CYPIX / 2 - 2, CXPIX / 2 + 4, CYPIX / 2 + 2);
							break;
						}
						GetScrn().Set(false, WHITE);
						m_bHighFPS ?
							GetScrn().SetFrame(true, WHITE, &rcNew) :
							GetScrn().Set(true, WHITE, &rcNew);
						ExpandRect(&m_rcField, m_iSize);
						GetScrn().Refresh(SHRINK, &m_rcField);
					}
				}
				m_bKeyUp[5] = false;
			}
			else
				m_bKeyUp[5] = true;

			if (IsKeyDown(DIK_X))
			{
				SetMode(GAME_MENU);
				GetScrn().Set(false, WHITE);
				m_bSceneOver = true;
			}

			return;
		}

		if (m_bEnding)
		{
			m_timeShow += fElapsedTime;
			if (m_timeShow >= 3000.0f)
			{
				m_timeShow = 3000.0f;
				if (IsKeyDown())
				{
					SetMode(GAME_MENU);
					GetScrn().Set(false, WHITE);
					m_bSceneOver = true;
				}
			}
			return;
		}

		if (!m_bFieldInit)
		{
			InitField();
		}
		else
		{
			if (!m_bStageInit)
			{
				InitStage();
				m_bStageShow = true;
				m_bRefresh = true;
			}

			if (m_bStageShow)
			{
				if (m_bBeginShow)
				{
					if (m_bNextSent)
					{
						m_timeShow -= fElapsedTime;
						if (m_timeShow < 0.0f)
						{
							m_iSent++;
							m_bNextSent = false;
							if (m_iSent >= (int)m_vecBegin[m_iStage + m_iStageOffset - 1].size())
							{
								m_bBeginShow = false;
								m_timeShow = 0.0f;
							}
							if (m_iStage == 7)
							{
								if (m_iSent < 5)
									m_pxEnemy = m_iSent * 3;
								else if (m_iStageOffset != 0 && m_iSent < 10)
									m_pxFriend = CXPIX / 4 - (m_iSent - 4) * 8;
							}
							m_bRefresh = true;
						}
					}
					else
					{
						m_timeShow += fElapsedTime;
						if (m_timeShow >= 500.0f)
						{
							m_timeShow = 500.0f;
							if (IsKeyDown())
								m_bNextSent = true;
						}
					}
				}
				else
				{
					m_timeShow += fElapsedTime;
					if (m_timeShow >= 1000.0f)
					{
						m_bStageShow = false;
						m_bPlay = true;
						m_timeShow = 0.0f;
					}
				}
			}

			if (m_bPlay)
			{
				int iDir = 0;
				int iDirBlack = 0;
				
				if (m_iStage == 4)
				{
					if (m_iMajorCtrl == -1)
					{
						if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_RIGHT))
							m_iMajorCtrl = 0;
						if (IsKeyDown(DIK_W) || IsKeyDown(DIK_S) || IsKeyDown(DIK_A) || IsKeyDown(DIK_D))
							m_iMajorCtrl = 1;
					}
					if (m_iMinorCtrl == -1)
					{
						if (m_iMajorCtrl == 0 &&
							(IsKeyDown(DIK_W) || IsKeyDown(DIK_S) || IsKeyDown(DIK_A) || IsKeyDown(DIK_D)))
							m_iMinorCtrl = 1;
						if (m_iMajorCtrl == 1 &&
							(IsKeyDown(DIK_UP) || IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_RIGHT)))
							m_iMinorCtrl = 0;
					}
				}

				if (IsKeyDown(DIK_UP))
				{
					if (m_bKeyUp[0])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 0)
								iDir = -1;

							if (m_iMinorCtrl == 0)
								iDirBlack = -1;
							else if (m_iMinorCtrl == -1)
								iDirBlack = 1;							 
						}
						else
							iDir = -1;
					}
					m_bKeyUp[0] = false;
				}
				else
					m_bKeyUp[0] = true;

				if (IsKeyDown(DIK_DOWN))
				{
					if (m_bKeyUp[1])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 0)
								iDir = 1;

							if (m_iMinorCtrl == 0)
								iDirBlack = 1;
							else if (m_iMinorCtrl == -1)
								iDirBlack = -1;
						}
						else
							iDir = 1;
					}
					m_bKeyUp[1] = false;
				}
				else
					m_bKeyUp[1] = true;

				if (IsKeyDown(DIK_LEFT))
				{
					if (m_bKeyUp[2])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 0)
								iDir = -2;

							if (m_iMinorCtrl == 0)
								iDirBlack = -2;
							else if (m_iMinorCtrl == -1)
								iDirBlack = 2;
						}
						else
							iDir = -2;
					}
					m_bKeyUp[2] = false;
				}
				else
					m_bKeyUp[2] = true;

				if (IsKeyDown(DIK_RIGHT))
				{
					if (m_bKeyUp[3])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 0)
								iDir = 2;

							if (m_iMinorCtrl == 0)
								iDirBlack = 2;
							else if (m_iMinorCtrl == -1)
								iDirBlack = -2;
						}
						else
							iDir = 2;
					}
					m_bKeyUp[3] = false;
				}
				else
					m_bKeyUp[3] = true;

				if (IsKeyDown(DIK_W))
				{
					if (m_bKeyUp[8])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 1)
								iDir = -1;

							if (m_iMinorCtrl == 1)
								iDirBlack = -1;
							else if (m_iMinorCtrl == -1)
								iDirBlack = 1;
						}
						else
							iDir = -1;
					}
					m_bKeyUp[8] = false;
				}
				else
					m_bKeyUp[8] = true;

				if (IsKeyDown(DIK_S))
				{
					if (m_bKeyUp[9])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 1)
								iDir = 1;

							if (m_iMinorCtrl == 1)
								iDirBlack = 1;
							else if (m_iMinorCtrl == -1)
								iDirBlack = -1;
						}
						else
							iDir = 1;
					}
					m_bKeyUp[9] = false;
				}
				else
					m_bKeyUp[9] = true;

				if (IsKeyDown(DIK_A))
				{
					if (m_bKeyUp[10])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 1)
								iDir = -2;

							if (m_iMinorCtrl == 1)
								iDirBlack = -2;
							else if (m_iMinorCtrl == -1)
								iDirBlack = 2;
						}
						else
							iDir = -2;
					}
					m_bKeyUp[10] = false;
				}
				else
					m_bKeyUp[10] = true;

				if (IsKeyDown(DIK_D))
				{
					if (m_bKeyUp[11])
					{
						if (m_iStage == 4)
						{
							if (m_iMajorCtrl == 1)
								iDir = 2;

							if (m_iMinorCtrl == 1)
								iDirBlack = 2;
							else if (m_iMinorCtrl == -1)
								iDirBlack = -2;
						}
						else
							iDir = 2;
					}
					m_bKeyUp[11] = false;
				}
				else
					m_bKeyUp[11] = true;

				if (IsKeyDown(DIK_LCONTROL) || IsKeyDown(DIK_RCONTROL))
					m_fRate = 3.0f;
				else
					m_fRate = 1.0f;

				switch (m_iStage)
				{
				case 1:
					m_timeUpdate += fElapsedTime;
					if (m_timeUpdate >= 20000.0f)
					{
						m_bEnding = true;
						m_timeUpdate = 0.0f;
					}

					if (iDir != 0)
					{
						if (Move(&m_sWhite, iDir))
						{
							m_bRefresh = true;
							m_timeUpdate = 0.0f;
						}
						else
						{
							m_bLose = true;
							m_pSE->Play(0);
						}
					}

					if (m_vecFood.size() == 0)
					{
						if (!NewFood())
							m_bWin = true;
					}

					break;

				case 2:
					if (iDir != 0)
					{
						if (abs(iDir) == abs(m_sWhite._iDir))
							iDir = 0;
					}
					else
					{
						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;
							iDir = m_sWhite._iDir;
						}
					}

					if (iDir != 0)
					{
						if (Move(&m_sWhite, iDir))
						{
							m_bRefresh = true;
							m_timeUpdate = 0.0f;
						}
						else
						{
							int x = m_sWhite._vecBody.rbegin()->x;
							int y = m_sWhite._vecBody.rbegin()->y;
							if (m_sWhite._vecBody.size() >= 20)
							{
								if ((x == 0 && iDir == -2) ||
									(x == m_cxField - 1 && iDir == 2) ||
									(y == 0 && iDir == -1) ||
									(y == m_cyField - 1 && iDir == 1))
									m_bWin = true;
								else
								{
									m_bLose = true;
									m_pSE->Play(0);
								}
							}
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}
					}

					if (m_vecFood.size() == 0)
					{
						if (!NewFood())
							m_bWin = true;
					}

					if (m_sWhite._vecBody.size() >= 21)
					{
						if (!m_bEndShow)
							m_bEndShow = true;
					}

					break;

				case 3:
					if (iDir != 0)
					{
						if (abs(iDir) == abs(m_sWhite._iDir))
							iDir = 0;
					}
					else
					{
						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;
							iDir = m_sWhite._iDir;
						}
					}

					if (m_vecFood.size() == 0)
					{
						if (!m_bEndShow)
							m_bEndShow = true;
					}

					if (iDir != 0)
					{
						if (Move(&m_sWhite, iDir))
						{
							m_bRefresh = true;
							m_timeUpdate = 0.0f;
						}
						else
						{
							int x = m_sWhite._vecBody.rbegin()->x;
							int y = m_sWhite._vecBody.rbegin()->y;
							if (m_vecFood.size() == 0)
							{
								if ((x == 0 && iDir == -2) ||
									(x == m_cxField - 1 && iDir == 2) ||
									(y == 0 && iDir == -1) ||
									(y == m_cyField - 1 && iDir == 1))
									m_bWin = true;
								else
								{
									m_bLose = true;
									m_pSE->Play(0);
								}
							}
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}
					}

					break;

				case 4:
					if (m_vecFood.size() != 0)
					{
						//White
						if (iDir != 0 && iDir != m_sWhite._iDir && iDir != -m_sWhite._iDir)
						{
							m_timeUpdate = 0.0f;
							if (Move(&m_sWhite, iDir))
								m_bRefresh = true;
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}

						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;

							if (Move(&m_sWhite, m_sWhite._iDir))
								m_bRefresh = true;
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}

						//Black
						if (iDirBlack != 0 && iDirBlack != m_sBlack._iDir && iDirBlack != -m_sBlack._iDir)
						{
							if (m_sBlack._vecBody.size() == 0)
							{
								POINT newBody;
								newBody.x = m_cxField / 2;
								newBody.y = m_cyField / 2;
								m_sBlack._vecBody.push_back(newBody);
								SetValid(false, newBody);
							}
														
							m_timeEnemyUpdate = 0.0f;
							if (Move(&m_sBlack, iDirBlack))
								m_bRefresh = true;
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}

						m_timeEnemyUpdate += fElapsedTime * m_fRate;
						if (m_timeEnemyUpdate >= m_timePeriod)
						{
							m_timeEnemyUpdate -= m_timePeriod;

							if (Move(&m_sBlack, m_sBlack._iDir))
								m_bRefresh = true;
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}

						//Judge
						if (m_sWhite._vecBody.size() > 10 || m_sBlack._vecBody.size() > 10)
							m_bLose = true;
					}
					else
					{
						if (m_iScore != m_iScoreBlack)
							m_bLose = true;
						else
						{
							if (!m_bEndShow)
							{
								m_iCurBGM++;
								m_bFade = true;
								m_bEndShow = true;
							}
						}
					}

					if (m_bEndShow)
					{
						if (iDir != 0)
						{
							if (Move(&m_sWhite, iDir))
								m_bRefresh = true;
							else
							{
								m_bWin = true;
								m_iStageOffset = 3;
							}
						}

						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;

							if (Move(&m_sBlack, m_sBlack._iDir))
								m_bRefresh = true;
							else
								m_bWin = true;
						}
					}


					break;

				case 5:
					if (iDir != 0)
					{
						if (abs(iDir) == abs(m_sWhite._iDir))
							iDir = 0;
					}
					else
					{
						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;
							iDir = m_sWhite._iDir;
						}
					}

					if (iDir != 0)
					{
						if (Move(&m_sWhite, iDir))
						{
							m_bRefresh = true;
							m_timeUpdate = 0.0f;
						}
						else
						{
							int x = m_sWhite._vecBody.rbegin()->x;
							int y = m_sWhite._vecBody.rbegin()->y;
							if (m_sEnemy.size() == 0)
							{
								if ((x == 0 && iDir == -2) ||
									(x == m_cxField - 1 && iDir == 2) ||
									(y == 0 && iDir == -1) ||
									(y == m_cyField - 1 && iDir == 1))
									m_bWin = true;
								else
								{
									m_bLose = true;
									m_pSE->Play(0);
								}
							}
							else
							{
								m_bLose = true;
								m_pSE->Play(0);
							}
						}
					}

					if (m_sEnemy.size() > 0)
					{
						m_timeEnemyUpdate += fElapsedTime;
						if (m_timeEnemyUpdate >= m_timePeriod)
						{
							m_bRefresh = true;
							m_timeEnemyUpdate -= m_timePeriod;

							auto iter = m_sEnemy.begin();
							while (iter != m_sEnemy.end())
							{
								if (iter->_vecBody.size() == 0)
									iter = m_sEnemy.erase(iter);
								else
									iter++;
							}

							for (auto &enemy : m_sEnemy)
							{
								if (enemy._vecBody.size() == 0)
									break;

								int x = enemy._vecBody.rbegin()->x;
								int y = enemy._vecBody.rbegin()->y;

								int xTar = 0, yTar = 0, iDist = CXPIX + CYPIX;
								
								if (enemy._vecBody.size() > m_sWhite._vecBody.size())
								{
									xTar = m_sWhite._vecBody.rbegin()->x;
									yTar = m_sWhite._vecBody.rbegin()->y;
									iDist = abs(xTar - x) + abs(yTar - y);
								}
								
								for (auto food : m_vecFood)
								{
									if (abs(food.x - x) + abs(food.y - y) < iDist)
									{
										xTar = food.x;
										yTar = food.y;
										iDist = abs(xTar - x) + abs(yTar - y);
									}
								}

								int iEnemyDir = 0;
								if (yTar < y)
									iEnemyDir = -1;
								else if (yTar > y)
									iEnemyDir = 1;

								if (iEnemyDir == 0)
								{
									iEnemyDir = xTar < x ? -2 : 2;
								}
								else
								{
									if (rand() % 2 == 0)
										iEnemyDir = xTar < x ? -2 : xTar > x ? 2 : iEnemyDir;
								}

								for (int i = 0; i < 4; i++)
								{
									int xNew = x, yNew = y;
									xNew += iEnemyDir == -2 ? -1 : iEnemyDir == 2 ? 1 : 0;
									yNew += iEnemyDir == -1 ? -1 : iEnemyDir == 1 ? 1 : 0;

									bool bSafe = true;

									if (xNew < 0 || xNew >= m_cxField || yNew < 0 || yNew >= m_cyField)
										bSafe = false;
									else
									{
										for (auto &_enemy : m_sEnemy)
										{
											for (size_t j = 1; j < _enemy._vecBody.size(); j++)
											{
												if (xNew == _enemy._vecBody[j].x && yNew == _enemy._vecBody[j].y)
												{
													bSafe = false;
													break;
												}
											}
										}
									}

									if (bSafe)
										break;
									else
									{
										iEnemyDir++;
										if (iEnemyDir == 0)
											iEnemyDir++;
										if (iEnemyDir == 3)
											iEnemyDir = -2;
									}
								}

								if (iEnemyDir == -enemy._iDir)
									iEnemyDir = enemy._iDir;

								Move(&enemy, iEnemyDir);
							}
						}
					}

					if (m_sEnemy.size() == 0)
						m_bEndShow = true;

					if (m_vecFood.size() < 5)
						NewFood();

					break;

				case 6:
					if (iDir != 0)
					{
						if (abs(iDir) == abs(m_sWhite._iDir))
							iDir = 0;
					}
					else
					{
						m_timeUpdate += fElapsedTime * m_fRate;
						if (m_timeUpdate >= m_timePeriod)
						{
							m_timeUpdate -= m_timePeriod;
							iDir = m_sWhite._iDir;
						}
					}

					if (iDir != 0)
					{
						if (Move(&m_sWhite, iDir))
						{
							m_bRefresh = true;
							m_timeUpdate = 0.0f;
						}
						else
						{
							m_bLose = true;
							m_pSE->Play(0);
						}
					}

					if (m_vecFood.size() == 0)
					{
						if (!NewFood())
							m_bWin = true;
					}

					break;

				case 7:
					if (iDir == 2)
					{
						if (Move(&m_sWhite, iDir))
							m_bRefresh = true;
					}

					m_timeUpdate += fElapsedTime;
					if (m_timeUpdate >= m_timePeriod)
					{
						m_timeUpdate -= m_timePeriod;
						if (m_iStageOffset == 0)
							m_pxEnemy++;
						else
						{
							if (m_pxEnemy < m_pxFriend)
							{
								m_pxEnemy++;
								m_pxFriend--;
							}
							else
							{
								int iMove = rand() % 2 * 2 - 1;
								if ((iMove < 0 && m_pxEnemy <= 0) ||
									(iMove > 0 && m_pxFriend >= m_sWhite._vecBody[0].x - 2))
									iMove = 0;
									
								m_pxEnemy += iMove;
								m_pxFriend += iMove;
							}
						}
						m_bRefresh = true;
					}

					if (m_pxEnemy > m_sWhite._vecBody.rbegin()->x)
					{
						m_iStageOffset = 0;
						m_bEnding = true;
					}

					if (m_sWhite._vecBody.rbegin()->x == m_cxField - 1)
						m_bEnding = true;
				}
			}

			if (m_bRefresh)
			{
				GetScrn().Set(false, WHITE, &m_rcField);

				RECT rcNew = { 0,0,0,0 };

				if (m_bHighFPS)
				{
					if (m_iStage == 7)
					{
						SetGridRect(&rcNew, -1, 2, m_pxEnemy, m_cyField - 2, m_iSize);
						GetScrn().SetFrame(true, WARLOCK, &rcNew);

						SetGridRect(&rcNew, m_pxFriend, 2, CXPIX + 1, m_cyField - 2, m_iSize);
						GetScrn().SetFrame(true, GRAY(6), &rcNew);

						int xWhite = m_sWhite._vecBody[0].x;
						int yWhite = m_sWhite._vecBody[0].y;
						if (m_pxFriend <= xWhite - 2)
						{
							SetGridRect(&rcNew, xWhite - 2, yWhite - 2, xWhite + 3, yWhite + 3, m_iSize);
							GetScrn().SetFrame(true, GRAY(6), &rcNew);
						}
					}
				}
				else
				{
					for (int x = 0; x < m_cxField; x++)
					{
						for (int y = 0; y < m_cyField; y++)
						{
							if (m_iColorField[x][y] != -1)
							{
								rcNew.left = m_pxField + x * m_iSize;
								rcNew.top = m_pyField + y * m_iSize;
								rcNew.right = rcNew.left + m_iSize;
								rcNew.bottom = rcNew.top + m_iSize;

								if (m_iStage == 7 && y >= 2 && y < m_cyField - 2)
								{
									if (x < m_pxEnemy)
										GetScrn().Set(true, WARLOCK, &rcNew);
									else if (x >= m_pxFriend)
									{
										int xWhite = m_sWhite._vecBody[0].x;
										int yWhite = m_sWhite._vecBody[0].y;
										if (x < xWhite - 2 || x > xWhite + 2 ||
											y < yWhite - 2 || y > yWhite + 2)
											GetScrn().Set(true, GRAY(6), &rcNew);
										else
											GetScrn().Set(true, m_colorMap[m_iColorField[x][y]], &rcNew);
									}
									else
										GetScrn().Set(true, m_colorMap[m_iColorField[x][y]], &rcNew);
								}
								else
									GetScrn().Set(true, m_colorMap[m_iColorField[x][y]], &rcNew);
							}
						}
					}
				}

				Show(&m_sWhite);
				Show(&m_sBlack);

				for (auto enemy : m_sEnemy)
					Show(&enemy);

				for (auto food : m_vecFood)
				{
					rcNew.left = m_pxField + food.x * m_iSize;
					rcNew.top = m_pyField + food.y * m_iSize;
					rcNew.right = rcNew.left + m_iSize;
					rcNew.bottom = rcNew.top + m_iSize;
					m_bHighFPS ?
						GetScrn().SetFrame(true, DKNIGHT, &rcNew) :
						GetScrn().Set(true, DKNIGHT, &rcNew);
				}

				for (auto poison : m_vecPoison)
				{
					rcNew.left = m_pxField + poison.x * m_iSize;
					rcNew.top = m_pyField + poison.y * m_iSize;
					rcNew.right = rcNew.left + m_iSize;
					rcNew.bottom = rcNew.top + m_iSize;
					m_bHighFPS ?
						GetScrn().SetFrame(true, DHUNTER, &rcNew) :
						GetScrn().Set(true, DHUNTER, &rcNew);
				}

				GetScrn().Refresh(IMM, NULL);
				m_bRefresh = false;
			}
		}
	}

	if (!GetScrn().IsRefOver())
	{
		GetScrn().Update(fTime, fElapsedTime);
	}

	if (IsKeyDown(DIK_ESCAPE))
	{
		if (m_bKeyUp[6])
		{
			SetMode(GAME_PAUSE);
			DXAudioStop();
		}
		m_bKeyUp[6] = false;
	}
	else
		m_bKeyUp[6] = true;
}

void SceneSnake::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);

	int iLang = GetSettings()._iLanguage;
	ID3DXFont *pFont = m_vecFont[iLang];

	if (GAME_PAUSE == GetMode())
	{
		pFont->DrawText(NULL, iLang ? L"暂停\n\nX - 结束" :
			L"PAUSE\n\nX - End", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bEnding)
	{
		D3DXCOLOR color = WHITE;
		color.a = m_timeShow / 3000.0f;
		pFont->DrawText(NULL, m_vecEnd[m_iStage + m_iStageOffset - 1][iLang], -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, color);
		if (m_timeShow >= 3000.0f)
			pFont->DrawText(NULL, iLang ? L"返回菜单" : L"Back to Menu", -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bStageShow)
	{
		if (m_bBeginShow)
		{
			D3DXCOLOR color = WHITE;
			color.a = m_timeShow / 500.0f;
			pFont->DrawText(NULL, m_vecBegin[m_iStage + m_iStageOffset - 1][m_iSent][iLang], -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, color);
		}
		else
		{
			D3DXCOLOR color = WHITE;
			color.a = m_timeShow / 500.0f;
			WCHAR szBuffer[40];
			swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
			pFont->DrawText(NULL, szBuffer, -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, color);
		}
		return;
	}

	if (m_bWin)
	{
		pFont->DrawText(NULL, iLang ? L"完成" : 
			L"Complete", -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, WHITE);
		pFont->DrawText(NULL, iLang ? L"Enter - 下一关     X - 结束" : 
			L"Enter - Next     X - End", -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bLose)
	{
		pFont->DrawText(NULL, iLang ? L"失败" : 
			L"Failed", -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, WHITE);
		pFont->DrawText(NULL, iLang ? L"Enter - 重试     X - 结束" : 
			L"Enter - Retry     X - End", -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bPlay)
	{
		WCHAR szBuffer[40];
		if (m_iStage == 4)
		{
			swprintf_s(szBuffer, iLang ? L"能量: %d - %d" : L"Power: %d - %d", m_iScore, m_iScoreBlack);
			pFont->DrawText(NULL, szBuffer, -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);
		}
		else
		{
			swprintf_s(szBuffer, iLang ? L"能量: %d" : L"Power: %d", m_iScore);
			pFont->DrawText(NULL, szBuffer, -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);
		}
		if (m_bEndShow)
		{
			pFont->DrawText(NULL, m_vecEnd[m_iStage + m_iStageOffset - 1][iLang], -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, WHITE);
		}
		else
		{
			swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
			pFont->DrawText(NULL, szBuffer, -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, WHITE);
		}
	}
}

void SceneSnake::Refresh()
{
	GetScrn().Set(false, WHITE);
	SetRect(&m_rcField, m_pxField, m_pyField, m_pxField + m_cxField * m_iSize, m_pyField + m_cyField * m_iSize);
	RECT rcNew = { 0,0,0,0 };

	if (m_bHighFPS)
	{
		ExpandRect(&m_rcField, m_iSize);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, m_iSize - 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 1);
	}
	else
	{
		ExpandRect(&m_rcField, m_iSize);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, m_iSize);
		GetScrn().Set(false, WHITE, &m_rcField);
		for (int x = 0; x < m_cxField; x++)
		{
			for (int y = 0; y < m_cyField; y++)
			{
				if (m_iColorField[x][y] != -1)
				{
					rcNew.left = m_pxField + x * m_iSize;
					rcNew.top = m_pyField + y * m_iSize;
					rcNew.right = rcNew.left + m_iSize;
					rcNew.bottom = rcNew.top + m_iSize;
					GetScrn().Set(true, m_colorMap[m_iColorField[x][y]], &rcNew);
				}
			}
		}
	}
	m_bRefresh = true;
	GetScrn().Refresh(IMM, NULL);
}

void SceneSnake::InitField()
{
	switch (m_iStage)
	{
	case 1:
		m_iSize = 1;
		m_cxField = 4;
		m_cyField = 4;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int x = 0; x < m_cxField; x++)
		{
			for (int y = 0; y < m_cyField; y++)
			{
				m_iColorField[x][y] = -1;
			}
		}
		break;

	case 2:
		m_iSize = 2;
		m_cxField = 15;
		m_cyField = 15;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int x = 0; x < m_cxField; x++)
		{
			for (int y = 0; y < m_cyField; y++)
			{
				m_iColorField[x][y] = 1;
			}
		}
		break;

	case 3:
		m_iSize = 4;
		m_cxField = 27;
		m_cyField = 27;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int x = 0; x < m_cxField; x++)
		{
			static int ySea = 18;
			ySea += rand() % 3 - 1;

			for (int y = 0; y < m_cyField; y++)
			{
				m_iColorField[x][y] = y > (ySea + m_cyField * 2) / 3 ? 2 : y > ySea ? 1 : 3;
			}
		}
		break;

	case 4:
		m_iSize = 4;
		m_cxField = 31;
		m_cyField = 31;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int y = 0; y < m_cyField; y++)
		{
			static int xLand = 15;
			xLand += rand() % 3 - 1;
			for (int x = 0; x < m_cxField; x++)
			{
				m_iColorField[x][y] = x < xLand ? 0 : 3;
			}
		}
		break;

	case 5:
		m_iSize = 4;
		m_cxField = 31;
		m_cyField = 31;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int y = 0; y < m_cyField; y++)
		{
			static int xLand = 15;
			xLand += rand() % 3 - 1;
			for (int x = 0; x < m_cxField; x++)
			{
				m_iColorField[x][y] = x < xLand ? 0 : 3;
			}
		}
		break;

	case 6:
		m_iSize = 8;
		m_cxField = 5;
		m_cyField = 5;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int y = 0; y < m_cyField; y++)
		{
			for (int x = 0; x < m_cxField; x++)
			{
				m_iColorField[x][y] = 0;
			}
		}
		break;

	case 7:
		m_iSize = 4;
		m_cxField = CXPIX / 4;
		m_cyField = CYPIX / 4;
		m_pxField = (CXPIX - m_cxField * m_iSize) / 2;
		m_pyField = (CYPIX - m_cyField * m_iSize) / 2;
		for (int y = 0; y < m_cyField; y++)
		{
			static int xLRiver = CXPIX / 16;
			static int xRRiver = CXPIX  / 8;
			xLRiver += rand() % 4 - 1;
			xRRiver += rand() % 4 - 1;
			while (xRRiver < xLRiver + 6)
				xRRiver++;
			for (int x = 0; x < m_cxField; x++)
			{
				m_iColorField[x][y] = x < xLRiver - rand() % 5 - 5 ? 0 : x < xLRiver ? 3 :
					x < xLRiver + rand() % 3 + 1 ? 1 : x < xRRiver - rand() % 3 - 1 ? 2 :
					x < xRRiver ? 1 : x < xRRiver + rand() % 5 + 5 ? 3 : 0;
			}
		}
		break;
	}

	GetScrn().Set(false, WHITE);
	SetRect(&m_rcField, m_pxField, m_pyField, m_pxField + m_cxField * m_iSize, m_pyField + m_cyField * m_iSize);
	RECT rcNew = { 0,0,0,0 };

	if (m_bHighFPS)
	{
		ExpandRect(&m_rcField, m_iSize);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, m_iSize - 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 1);
	}
	else
	{
		ExpandRect(&m_rcField, m_iSize);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, m_iSize);
		GetScrn().Set(false, WHITE, &m_rcField);
		for (int x = 0; x < m_cxField; x++)
		{
			for (int y = 0; y < m_cyField; y++)
			{
				if (m_iColorField[x][y] != -1)
				{
					rcNew.left = m_pxField + x * m_iSize;
					rcNew.top = m_pyField + y * m_iSize;
					rcNew.right = rcNew.left + m_iSize;
					rcNew.bottom = rcNew.top + m_iSize;
					GetScrn().Set(true, m_colorMap[m_iColorField[x][y]], &rcNew);
				}
			}
		}
	}

	rcNew.left = m_pxField + m_cxField / 2 * m_iSize;
	rcNew.top = m_pyField + m_cyField / 2 * m_iSize;
	rcNew.right = rcNew.left + m_iSize;
	rcNew.bottom = rcNew.top + m_iSize;
	m_bHighFPS ?
		GetScrn().SetFrame(true, WHITE, &rcNew) :
		GetScrn().Set(true, WHITE, &rcNew);

	GetScrn().Refresh(m_iStage == 1 ? IMM : BRUSH, NULL);

	if (m_iStage != 7)
	{
		SetPixRect(&m_rcTextTop, 0, 0, CXPIX, m_pyField - 1);
		SetPixRect(&m_rcTextBottom, 0, CYPIX - m_pyField + 1, CXPIX, CYPIX);
	}
	else
	{
		SetPixRect(&m_rcTextTop, 0, 0, CXPIX, CYPIX / 2);
		SetPixRect(&m_rcTextBottom, 0, CYPIX / 2, CXPIX, CYPIX);
	}

	m_iCurBGM = m_iStage - (m_iStage > 4 ? 0 : 1);
	m_bFieldInit = true;
}

void SceneSnake::InitStage()
{
	m_vecValid.clear();

	if (m_iStage != 7)
	{
		for (int x = 0; x < m_cxField; x++)
		{
			for (int y = 0; y < m_cyField; y++)
			{
				SetValid(true, { x, y });
			}
		}
	}

	m_sWhite._vecBody.clear();
	m_sWhite._iDir = 0;
	m_sWhite._vecBody.push_back({ m_cxField / 2, m_cyField / 2 });
	SetValid(false, { m_cxField / 2, m_cyField / 2 });

	m_sBlack._vecBody.clear();
	m_sBlack._iDir = 0;
	m_sBlack._color = GRAY(6);

	m_sEnemy.clear();
	m_vecFood.clear();
	m_vecPoison.clear();

	switch (m_iStage)
	{
	case 1:
		NewFood();
		break;

	case 2:
		m_timePeriod = 500.0f;
		NewFood();
		break;

	case 3:
		m_timePeriod = 500.0f;
		while (m_vecFood.size() < 20)
			NewFood();
		for (int i = 0; i < 20; i++)
			NewPoison();
		break;

	case 4:
		m_timePeriod = 500.0f;
		m_iMajorCtrl = -1;
		m_iMinorCtrl = -1;
		for (int i = 0; i < 20; i++)
			NewPoison();
		while (m_vecFood.size() < 18)
			NewFood();
		break;

	case 5:
		m_timePeriod = 400.0f;
		for (int i = 0; i < 18; i++)
			NewEnemy();
		for (int i = 0; i < 5; i++)
			NewFood();
		break;

	case 6:
		m_timePeriod = 400.0f;
		{
			int x = rand() % 5;
			int y;
			if (x % 2 == 0) 
			{
				if (x == 2)
					y = rand() % 2 * 4;
				else
					y = rand() % 3 * 2;

			}
			else
				y = rand() % 2 * 2 + 1;
			m_vecPoison.push_back({ x, y });
			SetValid(false, { x, y });
		}
		NewFood();
		break;

	case 7:
		m_timePeriod = 50.0f;
		break;
	}

	m_iScore = 1;
	m_iScoreBlack = 1;
	m_timeUpdate = 0.0f;
	m_timeEnemyUpdate = 0.0f;
	m_iSent = 0;

	m_bStageInit = true;
}

void SceneSnake::SetValid(bool bValid, POINT point)
{
	if (bValid)
	{
		for (auto p : m_vecValid)
		{
			if (point.x == p.x && point.y == p.y)
				return;
		}
		m_vecValid.push_back(point);
	} 
	else
	{
		auto iter = m_vecValid.begin();
		while (iter != m_vecValid.end())
		{
			if (point.x == iter->x && point.y == iter->y)
				iter = m_vecValid.erase(iter);
			else
				iter++;
		}
	}
}

bool SceneSnake::IsValid(POINT point)
{
	for (auto p : m_vecValid)
	{
		if (point.x == p.x && point.y == p.y)
			return true;
	}
	return false;
}

bool SceneSnake::NewEnemy()
{
	int cValid = (int)m_vecValid.size();

	if (cValid == 0)
		return false;
	else
	{
		Snake newEnemy;
		newEnemy._iDir = 0;
		newEnemy._color = rand() % 3 == 0 ? DRUID : rand() % 2 == 0 ? PALADIN : WARLOCK;
		
		POINT newBody = m_vecValid[rand() % cValid];
		newEnemy._vecBody.push_back(newBody);
		SetValid(false, newBody);

		for (int i = 0; i < (m_iStageOffset == 3 ? 6 : 2); i++)
		{
			if (rand() % 2 == 0)
				newBody.x += rand() % 2 == 0 ? 1 : -1;
			else
				newBody.y += rand() % 2 == 0 ? 1 : -1;

			if (IsValid(newBody))
			{
				newEnemy._vecBody.push_back(newBody);
				SetValid(false, newBody);
			}
		}
		m_sEnemy.push_back(newEnemy);
		return true;
	}
}

bool SceneSnake::NewFood()
{
	int cValid = (int)m_vecValid.size();

	if (cValid == 0)
		return false;
	else
	{
		POINT newFood = m_vecValid[rand() % cValid];

		if (m_iStage == 4)
		{
			for (auto poison : m_vecPoison)
			{
				if (newFood.x + poison.x == m_cxField - 1 &&
					newFood.y + poison.y == m_cyField - 1)
					return false;
			}
		}

		m_vecFood.push_back(newFood);
		SetValid(false, newFood);
		return true;
	}
}

bool SceneSnake::NewPoison()
{
	int cValid = (int)m_vecValid.size();

	if (cValid == 0)
		return false;
	else
	{
		POINT newPoison = m_vecValid[rand() % cValid];
		m_vecPoison.push_back(newPoison);
		SetValid(false, newPoison);
		return true;
	}
}

bool SceneSnake::Move(Snake *pSnake, int iDir)
{
	if (iDir + pSnake->_iDir == 0)
		return true;

	if (iDir == 0)
		return true;

	pSnake->_iDir = iDir;

	if (pSnake->_vecBody.size() == 0)
		return true;

	int xNew = pSnake->_vecBody.rbegin()->x;
	int yNew = pSnake->_vecBody.rbegin()->y;

	switch (iDir)
	{
	case -2:
		xNew--;
		break;
	case -1:
		yNew--;
		break;
	case 1:
		yNew++;
		break;
	case 2:
		xNew++;
		break;
	}

	bool bDestroy = false;

	if (xNew < 0 || xNew >= m_cxField || yNew < 0 || yNew >= m_cyField)
		bDestroy = true;

	if (!bDestroy)
	{
		for (auto poison : m_vecPoison)
		{
			if (poison.x == xNew && poison.y == yNew)
			{
				bDestroy = true;
				break;
			}
		}
	}

	if (!bDestroy)
	{
		for (size_t i = 1; i < pSnake->_vecBody.size(); i++)
		{
			if (pSnake->_vecBody[i].x == xNew && pSnake->_vecBody[i].y == yNew)
			{
				bDestroy = true;
				break;
			}
		}
	}

	if (!bDestroy)
	{
		if (pSnake != &m_sWhite)
		{
			for (auto body : m_sWhite._vecBody)
			{
				if (body.x == xNew && body.y == yNew)
				{
					if (pSnake->_vecBody.size() > m_sWhite._vecBody.size())
						m_bLose = true;
					else
						bDestroy = true;
					break;
				}
			}
		}
	}

	if (!bDestroy)
	{
		if (pSnake != &m_sBlack)
		{
			for (auto body : m_sBlack._vecBody)
			{
				if (body.x == xNew && body.y == yNew)
				{
					bDestroy = true;
					break;
				}
			}
		}
	}

	if (!bDestroy)
	{
		for (auto &enemy : m_sEnemy)
		{
			if (pSnake != &enemy)
			{
				for (auto body : enemy._vecBody)
				{
					if (body.x == xNew && body.y == yNew)
					{
						if (pSnake->_vecBody.size() > enemy._vecBody.size())
						{
							for (size_t i = 0; i < enemy._vecBody.size(); i++)
							{
								if (i % 2 == 0)
									m_vecFood.push_back({ enemy._vecBody[i].x, enemy._vecBody[i].y });
								else
									SetValid(true, { enemy._vecBody[i].x, enemy._vecBody[i].y });
							}
							enemy._vecBody.clear();
						}
						else
							bDestroy = true;
						break;
					}
				}
			}
			if (bDestroy)
				break;
		}
	}

	if (bDestroy)
	{
		if (m_iStage >= 5 && pSnake != &m_sWhite)
		{
			for (size_t i = 0; i < pSnake->_vecBody.size(); i++)
			{
				if (i % 2 == 0)
					m_vecFood.push_back({ pSnake->_vecBody[i].x, pSnake->_vecBody[i].y });
				else
					SetValid(true, { pSnake->_vecBody[i].x, pSnake->_vecBody[i].y });
			}
			pSnake->_vecBody.clear();
		}
		return false;
	}

	pSnake->_vecBody.push_back({ xNew, yNew });
	SetValid(false, { xNew, yNew });

	auto iter = m_vecFood.begin();
	while (iter != m_vecFood.end())
	{
		if (iter->x == xNew && iter->y == yNew)
		{
			if (pSnake == &m_sWhite)
			{
				m_iScore++;
				m_pSE->Play(1);
			}
			if (pSnake == &m_sBlack)
			{
				m_iScoreBlack++;
				m_pSE->Play(1);
			}
			m_vecFood.erase(iter);
			return true;
		}
		iter++;
	}

	SetValid(true, *(pSnake->_vecBody.begin()));
	pSnake->_vecBody.erase(pSnake->_vecBody.begin());

	return true;
}

void SceneSnake::Show(Snake *pSnake)
{
	if (pSnake->_vecBody.size() == 0)
		return;

	RECT rcNew = { 0,0,0,0 };
	D3DXCOLOR color = pSnake->_color;
	color.r *= 0.8f;
	color.g *= 0.8f;
	color.b *= 0.8f;

	for (auto body : pSnake->_vecBody)
	{
		rcNew.left = m_pxField + body.x * m_iSize;
		rcNew.top = m_pyField + body.y * m_iSize;
		rcNew.right = rcNew.left + m_iSize;
		rcNew.bottom = rcNew.top + m_iSize;

		m_bHighFPS ?
			GetScrn().SetFrame(true, color, &rcNew) :
			GetScrn().Set(true, color, &rcNew);
	}

	rcNew.left = m_pxField + pSnake->_vecBody.rbegin()->x * m_iSize;
	rcNew.top = m_pyField + pSnake->_vecBody.rbegin()->y * m_iSize;
	rcNew.right = rcNew.left + m_iSize;
	rcNew.bottom = rcNew.top + m_iSize;

	m_bHighFPS ?
		GetScrn().SetFrame(true, pSnake->_color, &rcNew) :
		GetScrn().Set(true, pSnake->_color, &rcNew);
}