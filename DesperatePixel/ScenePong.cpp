#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "ScenePong.h"

ScenePong::ScenePong()
{

}

ScenePong::~ScenePong()
{

}

void ScenePong::Init(int iInitVal)
{
	GetScrn().Set(false, WHITE);
	GetScrn().Refresh(BRUSH, NULL);

	if (iInitVal == 1 && !GetSettings()._iSkipGuide)
	{
		m_bGuide = true;
		m_bGuideOver = false;
	}
	else
	{
		m_bGuide = false;
		m_bGuideOver = true;
	}

	m_bSceneOver = false;
	m_bFieldInit = false;
	m_bStageInit = false;
	m_bStageShow = false;
	m_bRoundShow = false;
	m_bPlay = false;
	m_bServe = false;
	m_bWin = false;
	m_bLose = false;
	m_timeShow = 0.0f;

	m_iStage = iInitVal;
	m_iRound = 1;

	m_timeUpdate = 0.0f;
	m_fRate = 1.0f;

	m_bRefresh = true;
	m_bHighFPS = GetSettings()._iHighFPS;
	m_bTraject = false;

	for (auto &bUp : m_bKeyUp)
		bUp = false;

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);
	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 9 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);
	
	//Sound
	m_pBGM = new Sound();
	m_pBGM->Register(ID_BGM_PONG, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_PONG, 255);
	m_pBGM->AddSoundFromRsrc(IDR_START, 0);
	m_pBGM->AddSoundFromRsrc(IDR_CLEAR, 0);
	m_pBGM->AddSoundFromRsrc(IDR_LOSE, 0);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_PONG, ID_SUBMIX_SOUNDEFFECT);
	m_pSE->AddSoundFromRsrc(IDR_PING1, 0);
	m_pSE->AddSoundFromRsrc(IDR_PING2, 0);
	m_pSE->AddSoundFromRsrc(IDR_BOOM3, 0);

	m_bBGMPlay = false;
}

void ScenePong::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void ScenePong::OnResetDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 10 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);
	
	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);
	SetPixRect(&m_rcTextTop, m_iSize, 0, CXPIX - m_iSize, m_iBound - m_iSize);
	SetPixRect(&m_rcTextBottom, m_iSize, CYPIX - m_iBound + m_iSize, CXPIX - m_iSize, CYPIX);
}

void ScenePong::OnLostDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void ScenePong::Update(double fTime, float fElapsedTime)
{
	GetInput();
	
	if (GAME_PAUSE == GetMode())
	{
		if (m_bGuide)
			SetMode(GAME_PLAY);

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

	if (GetScrn().IsRefOver())
	{
		if (m_bHighFPS != (bool)GetSettings()._iHighFPS)
		{
			m_bHighFPS = GetSettings()._iHighFPS;
			Refresh();
			return;
		}

		if (m_bGuide)
		{
			if (!m_bGuideOver)
			{
				if (m_timeShow < 500.0f)
					m_timeShow += fElapsedTime;
				else if (m_timeShow > 500.0f)
					m_timeShow = 500.0f;

				if (IsKeyDown() || GetSettings()._iSkipGuide)
					m_bGuideOver = true;
			}
			else
			{
				if (m_timeShow > 0.0f)
					m_timeShow -= fElapsedTime;
				else
				{
					m_timeShow = 0.0f;
					m_bGuide = false;
				}
			}
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
					SetMode(GAME_PLAY);
					m_bFieldInit = false;
					m_bStageInit = false;
					m_bStageShow = false;
					m_bRoundShow = false;
					m_bPlay = false;
					m_bServe = false;
					m_bLose = false;
					DXAudioEnd();
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
				DXAudioEnd();
			}

			return;
		}

		if (m_bWin)
		{
			if (GetSaveData()._iStage[0] <= m_iStage)
			{
				GetSaveData()._iStage[0] = min(5, m_iStage + 1);
				WriteSaveData();
			}

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (m_bKeyUp[5])
				{
					if (m_iStage == 5)
					{
						SetMode(GAME_MENU);
						GetScrn().Set(false, WHITE);
						m_bSceneOver = true;
					}
					else
					{
						SetMode(GAME_PLAY);
						m_iStage++;
						m_bFieldInit = false;
						m_bStageInit = false;
						m_bStageShow = false;
						m_bRoundShow = false;
						m_bPlay = false;
						m_bServe = false;
						m_bWin = false;
					}
					DXAudioEnd();
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
				DXAudioEnd();
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
			}

			if (m_bStageShow)
			{
				m_timeShow += fElapsedTime;
				if (m_timeShow >= 2000.0f)
				{
					m_bStageShow = false;
					m_bRoundShow = true;
					m_timeShow = 0.0f;
				}
			}

			if (m_bRoundShow)
			{
				if (!m_bBGMPlay)
				{
					m_pBGM->Play(0);
					m_bBGMPlay = true;
				}

				m_timeShow += fElapsedTime;
				if (m_timeShow >= 2000.0f)
				{
					m_bRoundShow = false;
					m_bPlay = true;
					m_timeShow = 0.0f;
				}
			}

			if (m_bPlay)
			{
				if (!m_bServe)
				{
					m_fVPlayer = 0.0f;
					m_fSPlayer = (float)(CYPIX / 2);

					m_fVCom = 0.0f;
					m_fSCom = (float)(CYPIX / 2);

					if (m_iRound % 2 == 0)
					{
						m_fxBall = (float)(CXPIX * 2 / 3 - m_iSize);
						m_fyBall = (float)(CYPIX / 2);

						float angle = float(rand() % 200 - 100) / 300.0f * D3DX_PI;
						RandBallSpeed(-1, rand() % 2 == 0 ? 1 : -1);
					}
					else
					{
						m_fxBall = (float)(CXPIX / 3);
						m_fyBall = (float)(CYPIX / 2 - m_iSize);

						float angle = (float)(rand() % 200 - 100) / 300.0f * D3DX_PI;
						RandBallSpeed(1, rand() % 2 == 0 ? 1 : -1);
					}

					int ixBall = (int)round(m_fxBall);
					int iyBall = (int)round(m_fyBall);

					SetRect(&m_rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
					m_bHighFPS ? GetScrn().SetFrame(true, GRAY(13), &m_rcBall) :
						GetScrn().Set(true, GRAY(13), &m_rcBall);

					m_bServe = true;
					m_bRefresh = true;
				}

				//Player Update

				if (IsKeyDown(DIK_TAB))
				{
					if (m_bKeyUp[7])
					{
						m_bTraject = !m_bTraject;
						m_bRefresh = true;
					}
					m_bKeyUp[7] = false;
				}
				else
					m_bKeyUp[7] = true;

				if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
				{
					if (m_bKeyUp[0])
					{
						m_fVPlayer = -0.02f;
					}
					m_bKeyUp[0] = false;
				}
				else
					m_bKeyUp[0] = true;

				if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
				{
					if (m_bKeyUp[1])
					{
						m_fVPlayer = 0.02f;
					}
					m_bKeyUp[1] = false;
				}
				else
					m_bKeyUp[1] = true;

				if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
				{
					if (m_bKeyUp[2])
					{
						m_fVPlayer = 0.0f;
					}
					m_bKeyUp[2] = false;
				}
				else
					m_bKeyUp[2] = true;

				if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
				{
					if (m_bKeyUp[3])
					{
						m_fVPlayer = 0.0f;
					}
					m_bKeyUp[3] = false;
				}
				else
					m_bKeyUp[3] = true;
				
				if (IsKeyDown(DIK_LCONTROL) || IsKeyDown(DIK_RCONTROL))
					m_fRate = 3.0f;
				else
					m_fRate = 1.0f;
				
				if (m_fVPlayer < 0.0f)
				{
					m_fSPlayer += m_fVPlayer * fElapsedTime * m_fRate;
					if (m_fSPlayer <= (float)(m_rcField.top + m_iLPlayer * m_iSize))
						m_fSPlayer = (float)(m_rcField.top + m_iLPlayer * m_iSize);
					if (m_fxBall < (float)(m_rcPlayer.right) &&
						m_fyBall < (float)(m_rcPlayer.top) &&
						m_fSPlayer <= (float)(m_rcBall.bottom + m_iLPlayer * m_iSize))
						m_fSPlayer = (float)(m_rcBall.bottom + m_iLPlayer * m_iSize);
				}

				if (m_fVPlayer > 0.0f)
				{
					m_fSPlayer += m_fVPlayer * fElapsedTime * m_fRate;
					if (m_fSPlayer >= (float)(m_rcField.bottom - m_iLPlayer * m_iSize))
						m_fSPlayer = (float)(m_rcField.bottom - m_iLPlayer * m_iSize);
					if (m_fxBall < (float)(m_rcPlayer.right) &&
						m_fyBall > (float)(m_rcPlayer.bottom - m_iSize) &&
						m_fSPlayer >= (float)(m_rcBall.top - m_iLPlayer * m_iSize))
						m_fSPlayer = (float)(m_rcBall.top - m_iLPlayer * m_iSize);
				}

				//Com Update

				if (m_fyBall >= m_fSCom + (float)((m_iLCom - 1) * m_iSize))
					m_fVCom = 0.02f;
				else if (m_fyBall <= m_fSCom - (float)(m_iLCom * m_iSize))
					m_fVCom = -0.02f;
				else
					m_fVCom = 0.0f;

				if (m_fVCom < 0.0f)
				{
					m_fSCom += m_fVCom * fElapsedTime * m_fRate;
					if (m_fSCom <= (float)(m_rcField.top + m_iLCom * m_iSize))
						m_fSCom = (float)(m_rcField.top + m_iLCom * m_iSize);
					if (m_fxBall > (float)(m_rcCom.left - m_iSize) &&
						m_fyBall < (float)(m_rcCom.top) &&
						m_fSCom <= (float)(m_rcBall.bottom + m_iLCom * m_iSize))
						m_fSCom = (float)(m_rcBall.bottom + m_iLCom * m_iSize);
				}

				if (m_fVCom > 0.0f)
				{
					m_fSCom += m_fVCom * fElapsedTime * m_fRate;
					if (m_fSCom >= (float)(m_rcField.bottom - m_iLCom * m_iSize))
						m_fSCom = (float)(m_rcField.bottom - m_iLCom * m_iSize);
					if (m_fxBall > (float)(m_rcCom.left - m_iSize) &&
						m_fyBall >(float)(m_rcCom.bottom - m_iSize) &&
						m_fSCom >= (float)(m_rcBall.top - m_iLCom * m_iSize))
						m_fSCom = (float)(m_rcBall.top - m_iLCom * m_iSize);
				}

				//Ball Update

				if (m_fVBall > 0.0f)
				{
					m_fyBall += m_fVBall * fElapsedTime * m_fRate;
					if (m_fyBall >= (float)(m_rcField.bottom - m_iSize))
					{
						m_fyBall = (float)(m_rcField.bottom - m_iSize);
						m_fVBall = -m_fVBall;
						m_pSE->Play(0);
					}
					if (m_fxBall < (float)(m_rcPlayer.right) &&
						m_fyBall < (float)(m_rcPlayer.top) &&
						m_fyBall >= (float)(m_rcPlayer.top - m_iSize))
					{
						m_fyBall = (float)(m_rcPlayer.top - m_iSize) - EPSILON;
						m_fUBall = -0.04f;
						m_fVBall = -0.03f;
						m_pSE->Play(1);
					}
					if (m_fxBall >(float)(m_rcCom.left - m_iSize) &&
						m_fyBall < (float)(m_rcCom.top) &&
						m_fyBall >= (float)(m_rcCom.top - m_iSize))
					{
						m_fyBall = (float)(m_rcCom.top - m_iSize) - EPSILON;
						m_fUBall = 0.04f;
						m_fVBall = -0.03f;
						m_pSE->Play(1);
					}
				}
				else if (m_fVBall < 0.0f)
				{
					m_fyBall += m_fVBall * fElapsedTime * m_fRate;
					if (m_fyBall <= (float)m_rcField.top)
					{
						m_fyBall = (float)m_rcField.top;
						m_fVBall = -m_fVBall;
						m_pSE->Play(0);
					}
					if (m_fxBall < (float)(m_rcPlayer.right) &&
						m_fyBall >(float)(m_rcPlayer.bottom - m_iSize) &&
						m_fyBall <= (float)(m_rcPlayer.bottom))
					{
						m_fyBall = (float)(m_rcPlayer.bottom) + EPSILON;
						m_fUBall = -0.04f;
						m_fVBall = 0.03f;
						m_pSE->Play(1);
					}
					if (m_fxBall >(float)(m_rcCom.left - m_iSize) &&
						m_fyBall > (float)(m_rcCom.bottom - m_iSize) &&
						m_fyBall <= (float)(m_rcCom.bottom))
					{
						m_fyBall = (float)(m_rcCom.bottom) + EPSILON;
						m_fUBall = 0.04f;
						m_fVBall = 0.03f;
						m_pSE->Play(1);
					}
				}

				if (m_fUBall > 0.0f)
				{
					m_fxBall += m_fUBall * fElapsedTime * m_fRate;

					if (m_fxBall >= (float)(m_rcCom.left - m_iSize) &&
						m_fyBall > (float)(m_fSCom - (float)((m_iLCom + 1) * m_iSize)) &&
						m_fyBall < (float)(m_fSCom + (float)(m_iLCom * m_iSize)))
					{
						m_fxBall = (float)(m_rcCom.left - m_iSize) - EPSILON;
						RandBallSpeed(-1, m_fyBall >= m_fSCom ? 1 : -1);
						m_pSE->Play(1);
					}

					if (m_fxBall >= (float)(m_rcField.right - m_iSize))
					{
						m_fxBall = (float)(m_rcField.right - m_iSize);
						m_pSE->Play(2);

						m_iScorePlayer++;
						if (m_iScorePlayer == m_iTarScore)
						{
							m_bWin = true;
							m_bBGMPlay = false;
							m_pBGM->End();
							m_pBGM->Play(2);
						}
						else
						{
							m_iRound++;
							m_bPlay = false;
							m_bRoundShow = true;
							m_bServe = false;
						}
					}
				}
				else if (m_fUBall < 0.0f)
				{
					m_fxBall += m_fUBall * fElapsedTime * m_fRate;

					if (m_fxBall <= (float)(m_rcPlayer.right) &&
						m_fyBall > (float)(m_fSPlayer - (float)((m_iLPlayer + 1) * m_iSize)) &&
						m_fyBall < (float)(m_fSPlayer + (float)(m_iLPlayer * m_iSize)))
					{
						m_fxBall = (float)(m_rcPlayer.right) + EPSILON;
						RandBallSpeed(1, m_fyBall >= m_fSPlayer ? 1 : -1);
						m_pSE->Play(1);
					}

					if (m_fxBall <= (float)(m_rcField.left))
					{
						m_fxBall = (float)(m_rcField.left);
						m_pSE->Play(2);

						m_iScoreCom++;
						if (m_iScoreCom == m_iTarScore)
						{
							m_bLose = true;
							m_bBGMPlay = false;
							m_pBGM->End();
							m_pBGM->Play(3);
						}
						else
						{
							m_iRound++;
							m_bPlay = false;
							m_bRoundShow = true;
							m_bServe = false;
						}
					}
				}

				if (GetSettings()._iContinuous)
				{
					int ixBall = (int)round(m_fxBall);
					int iyBall = (int)round(m_fyBall);

					if (ixBall != m_rcBall.left || iyBall != m_rcBall.top)
					{
						GetScrn().Set(false, WHITE, &m_rcBall);
						SetRect(&m_rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
						m_bRefresh = true;
					}

					int iSPlayer = (int)round(m_fSPlayer);
					if (iSPlayer != (m_rcPlayer.top + m_rcPlayer.bottom) / 2)
					{
						GetScrn().Set(false, WHITE, &m_rcPlayer);
						SetRect(&m_rcPlayer, m_rcField.left + m_iSize, iSPlayer - m_iLPlayer * m_iSize,
							m_rcField.left + 2 * m_iSize, iSPlayer + m_iLPlayer * m_iSize);
						m_bRefresh = true;
					}

					int iSCom = (int)round(m_fSCom);
					if (iSCom != (m_rcCom.top + m_rcCom.bottom) / 2)
					{
						GetScrn().Set(false, WHITE, &m_rcCom);
						SetRect(&m_rcCom, m_rcField.right - 2 * m_iSize, iSCom - m_iLCom * m_iSize,
							m_rcField.right - m_iSize, iSCom + m_iLCom * m_iSize);
						m_bRefresh = true;
					}
				}
				else
				{
					int ixBall = (int)round(m_fxBall / (float)m_iSize) * m_iSize;
					int iyBall = (int)round(m_fyBall / (float)m_iSize) * m_iSize;

					if (ixBall != m_rcBall.left || iyBall != m_rcBall.top)
					{
						GetScrn().Set(false, WHITE, &m_rcBall);
						SetRect(&m_rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
						m_bRefresh = true;
					}

					int iSPlayer = (int)round(m_fSPlayer / (float)m_iSize) * m_iSize;
					if (iSPlayer != (m_rcPlayer.top + m_rcPlayer.bottom) / 2)
					{
						GetScrn().Set(false, WHITE, &m_rcPlayer);
						SetRect(&m_rcPlayer, m_rcField.left + m_iSize, iSPlayer - m_iLPlayer * m_iSize,
							m_rcField.left + 2 * m_iSize, iSPlayer + m_iLPlayer * m_iSize);
						m_bRefresh = true;
					}

					int iSCom = (int)round(m_fSCom / (float)m_iSize) * m_iSize;
					if (iSCom != (m_rcCom.top + m_rcCom.bottom) / 2)
					{
						GetScrn().Set(false, WHITE, &m_rcCom);
						SetRect(&m_rcCom, m_rcField.right - 2 * m_iSize, iSCom - m_iLCom * m_iSize,
							m_rcField.right - m_iSize, iSCom + m_iLCom * m_iSize);
						m_bRefresh = true;
					}
				}
			}

			if (m_bRefresh)
			{
				GetScrn().Set(false, WHITE, &m_rcField);

				if (m_bServe)
				{
					if (m_bTraject)
					{
						float fxBall = m_fxBall;
						float fyBall = m_fyBall;
						float fU = m_fUBall;
						float fV = m_fVBall;
						int ixBall = (int)round(fxBall);
						int iyBall = (int)round(fyBall);
						RECT rcBall;

						if (fU < 0.0f)
						{
							while (ixBall > m_rcField.left)
							{
								fxBall += fU;
								fyBall += fV;
								ixBall = (int)round(fxBall);
								iyBall = (int)round(fyBall);

								if (fyBall <= (float)m_rcField.top)
								{
									fyBall = (float)m_rcField.top;
									fV = -fV;
								}
								if (fyBall >= (float)(m_rcField.bottom - m_iSize))
								{
									fyBall = (float)(m_rcField.bottom - m_iSize);
									fV = -fV;
								}

								if (m_bHighFPS)
								{
									GetScrn().Set(true, GRAY(3), ixBall, iyBall);
									GetScrn().Set(true, GRAY(3), ixBall + m_iSize - 1, iyBall);
									GetScrn().Set(true, GRAY(3), ixBall, iyBall + m_iSize - 1);
									GetScrn().Set(true, GRAY(3), ixBall + m_iSize - 1, iyBall + m_iSize - 1);
									if (ixBall == m_rcField.left)
									{
										SetRect(&rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
										GetScrn().SetFrame(true, GRAY(5), &rcBall);
									}
								}
								else
								{
									SetRect(&rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
									if (ixBall == m_rcField.left)
										GetScrn().Set(true, GRAY(5), &rcBall);
									else
										GetScrn().Set(true, GRAY(3), &rcBall);
								}
							}
						}
						else
						{
							while (ixBall < m_rcField.right - m_iSize)
							{
								fxBall += fU;
								fyBall += fV;
								ixBall = (int)round(fxBall);
								iyBall = (int)round(fyBall);

								if (fyBall <= (float)m_rcField.top)
								{
									fyBall = (float)m_rcField.top;
									fV = -fV;
								}
								if (fyBall >= (float)(m_rcField.bottom - m_iSize))
								{
									fyBall = (float)(m_rcField.bottom - m_iSize);
									fV = -fV;
								}

								if (m_bHighFPS)
								{
									GetScrn().Set(true, GRAY(3), ixBall, iyBall);
									GetScrn().Set(true, GRAY(3), ixBall + m_iSize - 1, iyBall);
									GetScrn().Set(true, GRAY(3), ixBall, iyBall + m_iSize - 1);
									GetScrn().Set(true, GRAY(3), ixBall + m_iSize - 1, iyBall + m_iSize - 1);
									if (ixBall == m_rcField.right - m_iSize)
									{
										SetRect(&rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
										GetScrn().SetFrame(true, GRAY(5), &rcBall);
									}
								}
								else
								{
									SetRect(&rcBall, ixBall, iyBall, ixBall + m_iSize, iyBall + m_iSize);
									if (ixBall == m_rcField.right - m_iSize)
										GetScrn().Set(true, GRAY(5), &rcBall);
									else
										GetScrn().Set(true, GRAY(3), &rcBall);
								}
							}
						}
					}
				}

				if (m_bHighFPS)
				{
					GetScrn().Set(false, WHITE, &m_rcBall);
					GetScrn().Set(false, WHITE, &m_rcPlayer);
					GetScrn().Set(false, WHITE, &m_rcCom);
					if (m_bServe)
						GetScrn().SetFrame(true, GRAY(13), &m_rcBall);
					GetScrn().SetFrame(true, WHITE, &m_rcPlayer);
					GetScrn().SetFrame(true, WHITE, &m_rcCom);
				}
				else
				{
					if (m_bServe)
						GetScrn().Set(true, GRAY(13), &m_rcBall);
					GetScrn().Set(true, WHITE, &m_rcPlayer);
					GetScrn().Set(true, WHITE, &m_rcCom);
				}

				GetScrn().Refresh(IMM, &m_rcField);
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

void ScenePong::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);

	int iLang = GetSettings()._iLanguage;
	ID3DXFont *pFont = m_vecFont[iLang];

	if (m_bGuide)
	{
		D3DXCOLOR color = WHITE;
		color.a = m_timeShow / 500.0f;
		RECT rcGuide = m_rcScreen;
		rcGuide.left = (5 + iLang) * rcGuide.right / 20;

		pFont->DrawText(NULL, iLang ? 
			L"目标：\n\n将球击至对方后侧得分\n\n\
			控制：\n\n方向键 或 WSAD\n上/下  - 移动\n左/右  - 停止\n\nCtrl   - 加速\nTab    - 简单模式":
			L"Target:\n\nGet points by striking the ball \nto field behind the opponent.\n\n\
			Control:\n\nArrow Keys or WSAD\nUp and Down  - Move\nLeft or Right  - Stop\n\n\
			Ctrl  - Accelerate\nTab  - Easy Mode", 
			-1, &rcGuide, DT_LEFT | DT_VCENTER, color);
	}

	if (GAME_PAUSE == GetMode())
	{
		pFont->DrawText(NULL, iLang ? L"暂停\n\nX - 结束" :
			L"PAUSE\n\nX - End", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bStageShow)
	{
		WCHAR szBuffer[40];
		swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
		if ((int)(m_timeShow / 500.0f) % 2 == 0)
			pFont->DrawText(NULL, szBuffer, -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		return;
	}

	if (m_bRoundShow)
	{
		WCHAR szBuffer[40];
		swprintf_s(szBuffer, iLang ? L"第%d回合" : L"Round %d", m_iRound);
		if (m_timeShow < 1000.0f)
			pFont->DrawText(NULL, szBuffer, -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		else
			pFont->DrawText(NULL, iLang ? L"开始" : L"Start", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
	}

	if (m_bStageInit && !m_bSceneOver)
	{
		WCHAR szBuffer[40];
		swprintf_s(szBuffer, L"%d - %d", m_iScorePlayer, m_iScoreCom);
		pFont->DrawText(NULL, szBuffer, -1, &m_rcTextTop, DT_CENTER | DT_VCENTER, WHITE);
		pFont->DrawText(NULL, iLang ? L"玩家" : L"Player", -1, &m_rcTextTop, DT_LEFT | DT_VCENTER, WHITE);
		pFont->DrawText(NULL, iLang ? L"电脑" : L"Com", -1, &m_rcTextTop, DT_RIGHT | DT_VCENTER, WHITE);

		swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
		pFont->DrawText(NULL, szBuffer, -1, &m_rcTextBottom, DT_CENTER | DT_VCENTER, WHITE);

		if (!m_bWin && !m_bLose)
		{
			pFont->DrawText(NULL, iLang ? L"Esc - 暂停" : 
				L"Esc - Pause", -1, &m_rcTextBottom, DT_LEFT | DT_VCENTER, WHITE);
		}
	}

	if (m_bWin)
	{
		if (m_iStage < 5)
			pFont->DrawText(NULL, iLang ? L"胜利\n\nEnter - 下一关\nX - 结束" :
				L"You Win\n\nEnter - Next\nX - End", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		else
			pFont->DrawText(NULL, iLang ? L"通关\n\n返回菜单" : 
				L"All Clear\n\nBack to Menu", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
	}

	if (m_bLose)
	{
		pFont->DrawText(NULL, iLang ? L"失败\n\nEnter - 再试\nX - 结束" :
			L"You Lose\n\nEnter - Retry\nX - End", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
	}
}

void ScenePong::Refresh()
{
	if (m_bHighFPS)
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, m_iSize, m_iBound - m_iSize, CXPIX - m_iSize, CYPIX - m_iBound + m_iSize);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize - 1, m_iBound - 1, CXPIX - 2 * m_iSize + 1, CYPIX - m_iBound + 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize, m_iBound, CXPIX - 2 * m_iSize, CYPIX - m_iBound);
	}
	else
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, m_iSize, m_iBound - m_iSize, CXPIX - m_iSize, CYPIX - m_iBound + m_iSize);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize, m_iBound, CXPIX - 2 * m_iSize, CYPIX - m_iBound);
		GetScrn().Set(false, WHITE, &m_rcField);
	}

	m_bRefresh = true;

	GetScrn().Refresh(IMM, NULL);
}

void ScenePong::InitField()
{
	m_iBound = 30;

	switch (m_iStage)
	{
	case 1:
		m_iSize = 10;
		break;
	case 2:
		m_iSize = 6;
		break;
	case 3:
		m_iSize = 4;
		m_iBound = 32;
		break;
	case 4:
		m_iSize = 2;
		break;
	case 5:
		m_iSize = 1;
		break;
	}

	if (m_bHighFPS)
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, m_iSize, m_iBound - m_iSize, CXPIX - m_iSize, CYPIX - m_iBound + m_iSize);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize - 1, m_iBound - 1, CXPIX - 2 * m_iSize + 1, CYPIX - m_iBound + 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize, m_iBound, CXPIX - 2 * m_iSize, CYPIX - m_iBound);
	}
	else
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, m_iSize, m_iBound - m_iSize, CXPIX - m_iSize, CYPIX - m_iBound + m_iSize);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 2 * m_iSize, m_iBound, CXPIX - 2 * m_iSize, CYPIX - m_iBound);
		GetScrn().Set(false, WHITE, &m_rcField);
	}

	SetPixRect(&m_rcTextTop, m_iSize, 0, CXPIX - m_iSize, m_iBound - m_iSize);
	SetPixRect(&m_rcTextBottom, m_iSize, CYPIX - m_iBound + m_iSize, CXPIX - m_iSize, CYPIX);

	GetScrn().Refresh(BRUSH, NULL);

	m_bFieldInit = true;
}

void ScenePong::InitStage()
{
	switch (m_iStage)
	{
	case 1:
		m_iLPlayer = 2;
		m_iLCom = 1;
		break;
	case 2:
		m_iLPlayer = 3;
		m_iLCom = 2;
		break;
	case 3:
		m_iLPlayer = 4;
		m_iLCom = 3;
		break;
	case 4:
		m_iLPlayer = 5;
		m_iLCom = 4;
		break;
	case 5:
		m_iLPlayer = 6;
		m_iLCom = 6;
		break;
	}

	m_fVPlayer = 0.0f;
	m_fSPlayer = (float)(CYPIX / 2);

	m_fVCom = 0.0f;
	m_fSCom = (float)(CYPIX / 2);

	m_iTarScore = 3;
	m_iRound = 1;
	m_iScorePlayer = 0;
	m_iScoreCom = 0;

	SetRect(&m_rcPlayer, m_rcField.left + m_iSize, CYPIX / 2 - m_iLPlayer * m_iSize,
		m_rcField.left + 2 * m_iSize, CYPIX / 2 + m_iLPlayer * m_iSize);

	SetRect(&m_rcCom, m_rcField.right - 2 * m_iSize, CYPIX / 2 - m_iLCom * m_iSize,
		m_rcField.right - m_iSize, CYPIX / 2 + m_iLCom * m_iSize);

	GetScrn().Set(false, WHITE, &m_rcField);

	if (m_bHighFPS)
	{
		GetScrn().SetFrame(true, WHITE, &m_rcPlayer);
		GetScrn().SetFrame(true, WHITE, &m_rcCom);
	}
	else
	{
		GetScrn().Set(true, WHITE, &m_rcPlayer);
		GetScrn().Set(true, WHITE, &m_rcCom);
	}

	m_pBGM->Play(1);

	m_bRefresh = true;
	m_bStageInit = true;
}

void ScenePong::RandBallSpeed(int u, int v)
{
	switch (rand() % 4)
	{
	case 0:
		m_fUBall = (float)u * 0.05f;
		m_fVBall = 0.0f;
		break;

	case 1:
		m_fUBall = (float)u * 0.045f;
		m_fVBall = (float)v * 0.02f;
		break;

	case 2:
		m_fUBall = (float)u * 0.04f;
		m_fVBall = (float)v * 0.03f;
		break;

	case 3:
		m_fUBall = (float)u * 0.03f;
		m_fVBall = (float)v * 0.04f;
		break;
	}
}
