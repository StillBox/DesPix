#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "SceneRacing.h"

SceneRacing::SceneRacing()
{

}

SceneRacing::~SceneRacing()
{

}

void SceneRacing::Init(int iInitVal)
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
	m_bPlay = false;
	m_bWin = false;
	m_bLose = false;
	m_timeShow = 0.0f;

	m_iStage = iInitVal;

	m_timeUpdate = 0.0f;
	m_fRollSpeed = 0.08f;
	m_fEnemySpeed = 0.04f;
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
	m_pBGM->Register(ID_BGM_RACING, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_RACING, 255);
	m_pBGM->AddSoundFromRsrc(IDR_START, 0);
	m_pBGM->AddSoundFromRsrc(IDR_CLEAR, 0);
	m_pBGM->AddSoundFromRsrc(IDR_LOSE, 0);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_RACING, ID_SUBMIX_SOUNDEFFECT);

	m_bBGMPlay = false;
}

void SceneRacing::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneRacing::OnResetDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 9 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX); 
	SetRect(&m_rcTextCenter, m_rcField.left, CYPIX / 2 - 30, m_rcField.right, CYPIX / 2 + 30);
	SetPixRect(&m_rcTextLTop, 0, 0, 60, CYPIX / 2);
	SetPixRect(&m_rcTextLBottom, 0, CYPIX / 2, 60, CYPIX);
}

void SceneRacing::OnLostDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneRacing::Update(double fTime, float fElapsedTime)
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
			if (m_bHighFPS != (bool)GetSettings()._iHighFPS)
			{
				m_bHighFPS = GetSettings()._iHighFPS;
				Refresh();
				return;
			}

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
					m_bFieldInit = false;
					m_bStageInit = false;
					m_bStageShow = false;
					m_bPlay = false;
					m_bLose = false;
					m_timeShow = 0.0f;
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
			if (GetSaveData()._iStage[1] <= m_iStage)
			{
				GetSaveData()._iStage[1] = min(5, m_iStage + 1);
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
						m_iStage++;
						m_bFieldInit = false;
						m_bStageInit = false;
						m_bStageShow = false;
						m_bPlay = false;
						m_bWin = false;
						m_timeShow = 0.0f;
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
					m_bPlay = true;
					m_timeShow = 0.0f;
				}
			}

			if (m_bPlay)
			{
				if (!m_bBGMPlay)
				{
					m_pBGM->Play(0);
					m_bBGMPlay = true;
				}

				//Player

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

				if (IsKeyDown(DIK_LCONTROL) || IsKeyDown(DIK_RCONTROL))
					m_fRate = 3.0f;
				else
					m_fRate = 1.0f;
				
				if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
				{
					if (m_bKeyUp[2])
					{
						if (m_rcPlayer.left > m_rcField.left)
						{
							m_rcPlayer.left -= m_iSize;
							m_rcPlayer.right -= m_iSize;
							m_bRefresh = true;
						}
					}
					m_bKeyUp[2] = false;
				}
				else
					m_bKeyUp[2] = true;

				if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
				{
					if (m_bKeyUp[3])
					{
						if (m_rcPlayer.right < m_rcField.right)
						{
							m_rcPlayer.left += m_iSize;
							m_rcPlayer.right += m_iSize;
							m_bRefresh = true;
						}
					}
					m_bKeyUp[3] = false;
				}
				else
					m_bKeyUp[3] = true;

				//Sign

				m_fSignRoll += m_fRollSpeed * fElapsedTime * m_fRate;

				if (m_fSignRoll >= 1.0f)
				{
					int iSignRoll = (int)m_fSignRoll;

					for (auto &point : m_vecSign)
					{
						point.y += iSignRoll;
					}

					if (m_vecSign[0].y >= 0)
					{
						for (auto &point : m_vecSign)
						{
							point.y -= 6 * m_iSize;
						}
					}
						
					m_fSignRoll -= (float)iSignRoll;
					m_bRefresh = true;
				}

				//Enemy

				m_fEnemyRoll += m_fEnemySpeed * fElapsedTime * m_fRate;

				if (m_fEnemyRoll >= 1.0f)
				{
					int iEnemyRoll = (int)m_fEnemyRoll;

					auto iter = m_vecEnemy.begin();
					while (iter != m_vecEnemy.end())
					{
						iter->y += iEnemyRoll;

						if (iter->x == m_rcPlayer.left &&
							iter->y > m_rcPlayer.top - m_iSize &&
							iter->y < m_rcPlayer.bottom)
						{
							m_bLose = true;
							m_bBGMPlay = false;
							m_pBGM->End();
							m_pBGM->Play(3);
						}

						if (iter->y >= CYPIX)
							iter = m_vecEnemy.erase(iter);
						else
							iter++;
					}

					m_iGapEnemy += iEnemyRoll;
					while (m_iGapEnemy >= m_cEnemyGap)
					{
						if (m_iCounter > 0)
						{
							m_iCounter -= 1;

							for (int i = 0; i < m_cEnemy; i++)
							{
								int x = m_rcField.left + rand() % m_cWidth * m_iSize;
								int y = 0;
								m_vecEnemy.push_back({ x,y });
							}
						}
						m_iGapEnemy -= m_cEnemyGap;
					}

					if (m_iCounter == 0 && m_vecEnemy.size() == 0)
					{
						m_bWin = true;
						m_bBGMPlay = false;
						m_pBGM->End();
						m_pBGM->Play(2);
					}

					m_fEnemyRoll -= (float)iEnemyRoll;
					m_bRefresh = true;
				}
			}

			if (m_bRefresh)
			{
				GetScrn().Set(false, WHITE, &m_rcLSign);
				GetScrn().Set(false, WHITE, &m_rcRSign);
				GetScrn().Set(false, WHITE, &m_rcField);
				GetScrn().Set(false, WHITE, &m_rcProc);

				m_bHighFPS ? GetScrn().SetFrame(true, WHITE, &m_rcPlayer) :
					GetScrn().Set(true, WHITE, &m_rcPlayer);

				RECT rcNew;

				if (GetSettings()._iContinuous)
				{
					for (auto point : m_vecSign)
					{
						rcNew.left = point.x;
						rcNew.right = point.x + m_iSize;
						rcNew.top = point.y;
						rcNew.bottom = point.y + 4 * m_iSize;

						m_bHighFPS ? GetScrn().SetFrame(true, GRAY(8), &rcNew) :
							GetScrn().Set(true, GRAY(8), &rcNew);
					}

					for (auto point : m_vecEnemy)
					{
						rcNew.left = point.x;
						rcNew.right = point.x + m_iSize;
						rcNew.top = point.y;
						rcNew.bottom = point.y + m_iSize;
						m_bHighFPS ? GetScrn().SetFrame(true, GRAY((11 + (point.x / m_iSize) % 3 * 2)), &rcNew) :
							GetScrn().Set(true, GRAY((11 + (point.x / m_iSize) % 3 * 2)), &rcNew);
					}

					{
						rcNew.left = m_rcProc.left;
						rcNew.right = m_rcProc.right;
						rcNew.top = m_iCounter;
						rcNew.bottom = m_iCounter + m_iSize;
						m_bHighFPS ? GetScrn().SetFrame(true, WHITE, &rcNew) :
							GetScrn().Set(true, WHITE, &rcNew);
					}

					if (m_bWin || m_bLose)
						GetScrn().Set(false, WHITE, &m_rcTextCenter);
				}
				else
				{
					for (auto point : m_vecSign)
					{
						rcNew.left = point.x;
						rcNew.right = point.x + m_iSize;
						rcNew.top = point.y / m_iSize * m_iSize;
						rcNew.bottom = rcNew.top + 4 * m_iSize;

						m_bHighFPS ? GetScrn().SetFrame(true, GRAY(8), &rcNew) :
							GetScrn().Set(true, GRAY(8), &rcNew);
					}

					for (auto point : m_vecEnemy)
					{
						rcNew.left = point.x;
						rcNew.right = point.x + m_iSize;
						rcNew.top = point.y / m_iSize * m_iSize;
						rcNew.bottom = rcNew.top + m_iSize;
						m_bHighFPS ? GetScrn().SetFrame(true, GRAY((11 + (point.x / m_iSize) % 3 * 2)), &rcNew) :
							GetScrn().Set(true, GRAY((11 + (point.x / m_iSize) % 3 * 2)), &rcNew);
					}

					{
						rcNew.left = m_rcProc.left;
						rcNew.right = m_rcProc.right;
						rcNew.top = (m_iCounter + 3) / m_iSize * m_iSize;
						rcNew.bottom = rcNew.top + m_iSize;
						m_bHighFPS ? GetScrn().SetFrame(true, WHITE, &rcNew) :
							GetScrn().Set(true, WHITE, &rcNew);
					}

					if (m_bWin || m_bLose)
						GetScrn().Set(false, WHITE, &m_rcTextCenter);
				}

				if (m_bTraject)
				{
					int yEnd = 0;
					for (auto point : m_vecEnemy)
					{
						if (point.x == m_rcPlayer.left)
						{
							int yEnemyBottom = GetSettings()._iContinuous ? point.y + m_iSize :
								point.y / m_iSize * m_iSize + m_iSize;
							if (yEnemyBottom > yEnd && yEnemyBottom < m_rcPlayer.bottom)
								yEnd = yEnemyBottom;
						}
					}

					if (m_bHighFPS)
					{
						SetRect(&rcNew, m_rcPlayer.left, yEnd, m_rcPlayer.left + 1, m_rcPlayer.top);
						GetScrn().Set(true, GRAY(3), &rcNew);
						SetRect(&rcNew, m_rcPlayer.right - 1, yEnd, m_rcPlayer.right, m_rcPlayer.top);
						GetScrn().Set(true, GRAY(3), &rcNew);
					}
					else
					{
						SetRect(&rcNew, m_rcPlayer.left, yEnd, m_rcPlayer.right, m_rcPlayer.top);
						GetScrn().Set(true, GRAY(3), &rcNew);
					}
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

void SceneRacing::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);

	int iLang = GetSettings()._iLanguage;
	ID3DXFont *pFont = m_vecFont[iLang];

	if (m_bGuide)
	{
		D3DXCOLOR color = WHITE;
		color.a = m_timeShow / 500.0f;
		RECT rcGuide = m_rcScreen;
		rcGuide.left = 6 * rcGuide.right / 20;

		pFont->DrawText(NULL, iLang ?
			L"目标：\n\n避开对手并到达终点\n\n\
			控制：\n\n方向键 或 WSAD\n左/右  - 移动\n\nCtrl   - 加速\nTab    - 简单模式\n" :
			L"Target:\n\nAvoid the opponents \nand reach the goal.\n\n\
			Control:\n\nArrow Keys or WSAD\nLeft and Right  - Move\n\n\
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
		if (m_timeShow > 1000.0f)
			pFont->DrawText(NULL, iLang ? L"开始" : L"Start", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		else
		{
			WCHAR szBuffer[40];
			swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
			if ((int)(m_timeShow / 500.0f) % 2 == 0)
				pFont->DrawText(NULL, szBuffer, -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
		}
		return;
	}

	if (m_bStageInit && !m_bSceneOver)
	{
		WCHAR szBuffer[40];
		swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
		pFont->DrawText(NULL, szBuffer, -1, &m_rcTextLTop, DT_CENTER | DT_BOTTOM, WHITE);

		if (!m_bWin && !m_bLose)
		{
				pFont->DrawText(NULL, iLang ? L"Esc - 暂停" : 
					L"Esc\nPause", -1, &m_rcTextLBottom, DT_CENTER | DT_VCENTER, WHITE);
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
		pFont->DrawText(NULL, iLang ? L"失败\n\nEnter - 再试 \nX - 结束" :
			L"You Lose\n\nEnter - Retry\nX - End", -1, &m_rcScreen, DT_CENTER | DT_VCENTER, WHITE);
	}
}

void SceneRacing::Refresh()
{
	if (m_bHighFPS)
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, 60 + 2 * m_iSize, 0, 60 + 2 * m_iSize + 1, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize - 1, 0, 60 + 3 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, CXPIX - 60 - 3 * m_iSize, 0, CXPIX - 59 - 3 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, CXPIX - 61 - 2 * m_iSize, 0, CXPIX - 60 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize, 0, CXPIX - 60 - 3 * m_iSize, CYPIX);

		SetRect(&m_rcProc, CXPIX - 30 - 2 * m_iSize, 0, CXPIX - 29 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 31 - m_iSize, 0, CXPIX - 30 - m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30, 0, CXPIX - 29, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 31 + m_iSize, 0, CXPIX - 30 + m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30 - m_iSize, 0, CXPIX - 30, CYPIX);
	}
	else
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, 60 + 2 * m_iSize, 0, CXPIX - 60 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize, 0, CXPIX - 60 - 3 * m_iSize, CYPIX);
		GetScrn().Set(false, WHITE, &m_rcField);

		SetRect(&m_rcProc, CXPIX - 30 - 2 * m_iSize, 0, CXPIX - 30 - m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30, 0, CXPIX - 30 + m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30 - m_iSize, 0, CXPIX - 30, CYPIX);
	}

	for (auto point : m_vecSign)
	{
		RECT rcNewSign;
		rcNewSign.left = point.x;
		rcNewSign.right = point.x + m_iSize;
		rcNewSign.top = point.y;
		rcNewSign.bottom = point.y + 4 * m_iSize;

		m_bHighFPS ? GetScrn().SetFrame(true, GRAY(8), &rcNewSign) :
			GetScrn().Set(true, GRAY(8), &rcNewSign);
	}

	m_bRefresh = true;
	GetScrn().Refresh(IMM, NULL);
}

void SceneRacing::InitField()
{
	switch (m_iStage)
	{
	case 1:
		m_iSize = 10;
		m_cEnemy = 2;
		m_cEnemyGap = 4 * m_iSize;
		m_iPlayerHeight = 3;
		break;
	case 2:
		m_iSize = 6;
		m_cEnemy = 4;
		m_cEnemyGap = 5 * m_iSize;
		m_iPlayerHeight = 5;
		break;
	case 3:
		m_iSize = 4;
		m_cEnemy = 5;
		m_cEnemyGap = 6 * m_iSize;
		m_iPlayerHeight = 8;
		break;
	case 4:
		m_iSize = 2;
		m_cEnemy = 8;
		m_cEnemyGap = 7 * m_iSize;
		m_iPlayerHeight = 15;
		break;
	case 5:
		m_iSize = 1;
		m_cEnemy = 18;
		m_cEnemyGap = 10 * m_iSize;
		m_iPlayerHeight = 30;
		break;
	}

	SetRect(&m_rcLSign, 60, 0, 60 + m_iSize, CYPIX);
	SetRect(&m_rcRSign, CXPIX - 60 - m_iSize, 0, CXPIX - 60, CYPIX);

	//Field & Process

	if (m_bHighFPS)
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, 60 + 2 * m_iSize, 0, 60 + 2 * m_iSize + 1, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize - 1, 0, 60 + 3 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, CXPIX - 60 - 3 * m_iSize, 0, CXPIX - 59 - 3 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, CXPIX - 61 - 2 * m_iSize, 0, CXPIX - 60 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize, 0, CXPIX - 60 - 3 * m_iSize, CYPIX);

		SetRect(&m_rcProc, CXPIX - 30 - 2 * m_iSize, 0, CXPIX - 29 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 31 - m_iSize, 0, CXPIX - 30 - m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30, 0, CXPIX - 29, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 31 + m_iSize, 0, CXPIX - 30 + m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30 - m_iSize, 0, CXPIX - 30, CYPIX);
	}
	else
	{
		GetScrn().Set(false, WHITE);
		SetRect(&m_rcField, 60 + 2 * m_iSize, 0, CXPIX - 60 - 2 * m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		SetRect(&m_rcField, 60 + 3 * m_iSize, 0, CXPIX - 60 - 3 * m_iSize, CYPIX);
		GetScrn().Set(false, WHITE, &m_rcField);

		SetRect(&m_rcProc, CXPIX - 30 - 2 * m_iSize, 0, CXPIX - 30 - m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30, 0, CXPIX - 30 + m_iSize, CYPIX);
		GetScrn().Set(true, GRAY(10), &m_rcProc);
		SetRect(&m_rcProc, CXPIX - 30 - m_iSize, 0, CXPIX - 30, CYPIX);
	}
	
	m_cWidth = (m_rcField.right - m_rcField.left) / m_iSize;
	
	//Rects for Texts

	SetRect(&m_rcTextCenter, m_rcField.left, CYPIX / 2 - 30, m_rcField.right, CYPIX / 2 + 30);
	SetPixRect(&m_rcTextLTop, 0, 0, 60, CYPIX / 2);
	SetPixRect(&m_rcTextLBottom, 0, CYPIX / 2, 60, CYPIX);

	//Signs

	m_vecSign.clear();
	for (int i = -8; i <= CYPIX / m_iSize; i += 6)
	{		
		int y = i * m_iSize;
		m_vecSign.push_back({ 60, y });
		m_vecSign.push_back({ CXPIX - 60 - m_iSize, y });
	}

	for (auto point : m_vecSign)
	{
		RECT rcNewSign;
		rcNewSign.left = point.x;
		rcNewSign.right = point.x + m_iSize;
		rcNewSign.top = point.y;
		rcNewSign.bottom = point.y + 4 * m_iSize;

		m_bHighFPS ? GetScrn().SetFrame(true, GRAY(8), &rcNewSign) :
			GetScrn().Set(true, GRAY(8), &rcNewSign);
	}

	GetScrn().Refresh(BRUSH, NULL);

	m_bFieldInit = true;
}

void SceneRacing::InitStage()
{
	m_fSignRoll = 0.0f;
	m_fEnemyRoll = 0.0f; 
	m_iGapEnemy = 0;
	m_iCounter = CYPIX - m_iSize;

	m_vecEnemy.clear();

	SetRect(&m_rcPlayer, CXPIX / 2 - m_iSize, CYPIX - m_iPlayerHeight * m_iSize, 
		CXPIX / 2, CYPIX - (m_iPlayerHeight - 1) * m_iSize);

	m_pBGM->Play(1);

	m_bStageInit = true;
}