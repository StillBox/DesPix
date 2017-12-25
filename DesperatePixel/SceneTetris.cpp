#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "SceneTetris.h"

SceneTetris::SceneTetris()
{

}

SceneTetris::~SceneTetris()
{

}

void SceneTetris::Init(int iInitVal)
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

	m_fRate = 1.0f;
	m_bTraject = true;
	m_bHighFPS = GetSettings()._iHighFPS;
	m_bRefresh = false;

	for (auto &bUp : m_bKeyUp)
		bUp = false;
	for (auto &fUp : m_fKeyUp)
		fUp = 100.0f;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			m_iColorStock[x][y] = -1;
			m_iColorPreStock[x][y] = -1;
		}
	}

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);
	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 9 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);

	//Sound
	m_pBGM = new Sound();
	m_pBGM->Register(ID_BGM_TETRIS, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_TETRIS, 255);
	m_pBGM->AddSoundFromRsrc(IDR_START, 0);
	m_pBGM->AddSoundFromRsrc(IDR_CLEAR, 0);
	m_pBGM->AddSoundFromRsrc(IDR_LOSE, 0);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_TETRIS, ID_SUBMIX_SOUNDEFFECT);
	m_pSE->AddSoundFromRsrc(IDR_BOOM0, 0);
	m_pSE->AddSoundFromRsrc(IDR_BOOM1, 0);
	m_pSE->AddSoundFromRsrc(IDR_BOOM2, 0);
	m_pSE->AddSoundFromRsrc(IDR_BOOM4, 0);
	m_pSE->AddSoundFromRsrc(IDR_BOOM3, 0);
	m_pSE->AddSoundFromRsrc(IDR_FLUSH, 0);

	m_bBGMPlay = false;
	m_iSECount = 0;
}

void SceneTetris::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneTetris::OnResetDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_vecFont[0]);
	D3DXCreateFont(DXUTGetD3DDevice(), 9 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_vecFont[1]);

	SetPixRect(&m_rcScreen, 0, 0, CXPIX, CYPIX);
	SetPixRect(&m_rcTextLeft, 0, 19, 64, 148);
	SetPixRect(&m_rcTextRTop, 176, 16, CXPIX, 32);
	SetPixRect(&m_rcTextRBottom, 176, 64, CXPIX, 148);
}

void SceneTetris::OnLostDevice()
{
	for (int i = 0; i < 2; i++)
		SAFE_RELEASE(m_vecFont[i]);
}

void SceneTetris::Update(double fTime, float fElapsedTime)
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
			if (GetSaveData()._iStage[2] <= m_iStage)
			{
				GetSaveData()._iStage[2] = min(6, m_iStage + 1);
				WriteSaveData();
			}

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (m_bKeyUp[5])
				{
					if (m_iStage == 6)
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

						for (int x = 0; x < m_cxField; x++)
						{
							for (int y = 0; y < m_cyField; y++)
							{
								m_iColorPreStock[x][y] = m_iColorStock[x][y];
							}
						}
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
					m_bRefresh = true;
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

				if (!m_bClear)
				{
					for (auto y : m_vecClear)
					{
						for (int x = 0; x < m_cxField; x++)
						{
							m_iColorStock[x][y] = -1;
							RECT rcNew = { 0,0,0,0 };
							rcNew.left = (m_ixField + x) * m_iSize;
							rcNew.top = (m_iyField + y) * m_iSize;
							rcNew.right = rcNew.left + m_iSize;
							rcNew.bottom = rcNew.top + m_iSize;
							GetScrn().Set(false, WHITE, &rcNew);
						}
					}
					GetScrn().Refresh(BRUSH, &m_rcField);
					m_bClear = true;
					m_pSE->Play(5);
					return;
				}

				for (auto yClear : m_vecClear)
				{
					for (int y = yClear; y >= 0; y--)
					{
						for (int x = 0; x < m_cxField; x++)
						{
							m_iColorStock[x][y] = y == 0 ? -1 : m_iColorStock[x][y - 1];
						}
					}
					m_bRefresh = true;

					m_iScore++;
					if (m_iScore >= m_iTarScore)
					{
						m_bBGMPlay = false;
						m_pBGM->End();
						m_pBGM->Play(2);
						m_bWin = true;
					}
				}
				m_vecClear.clear();

				if (m_bNewPiece)
				{
					m_posCur.x = m_cxField / 2 - 2;
					m_posCur.y = 0;
					m_iRotCur = 0;
					m_iTypeCur = m_iTypeNext;
					m_iColorCur = m_iColorNext;

					if (!SetPos(m_posCur.x, m_posCur.y, m_iRotCur))
					{
						m_bBGMPlay = false;
						m_pBGM->End();
						m_pBGM->Play(3);
						m_bLose = true;
					}

					m_iTypeNext = rand() % 7;
					m_iColorNext = rand() % 12;

					m_bRefresh = true;
					m_bNewPiece = false;
				}

				for (auto &fUp : m_fKeyUp)
				{
					if (fUp <= 0.0f)
						fUp = 0.0f;
					else
						fUp -= fElapsedTime;
				}			

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
					m_fRate = 10.0f;
				else
					m_fRate = 1.0f;

				m_timeUpdate += fElapsedTime * m_fRate;

				if (m_timeUpdate >= m_timePeriod)
				{
					if (MoveDown())
					{
						m_pSE->Play(m_iSECount);
						m_iSECount = (m_iSECount + 1) % 4;
					}
					else
					{
						m_pSE->Play(4);
						m_iSECount = 0;
					}

					m_bRefresh = true;
					m_timeUpdate -= m_timePeriod;
				}

				if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
				{
					if (m_bKeyUp[2] || m_fKeyUp[2] == 0.0f)
					{
						MoveLeft();
						m_bRefresh = true;

						m_fKeyUp[2] = m_timePeriod * 0.125f;
						if (m_bKeyUp[2])
							m_fKeyUp[2] = 400.0f;
					}
					m_bKeyUp[2] = false;
				}
				else
					m_bKeyUp[2] = true;
				

				if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
				{
					if (m_bKeyUp[3] || m_fKeyUp[3] == 0.0f)
					{
						MoveRight();
						m_bRefresh = true;

						m_fKeyUp[3] = m_timePeriod * 0.125f;
						if (m_bKeyUp[3])
							m_fKeyUp[3] = 400.0f;
					}
					m_bKeyUp[3] = false;
				}
				else
					m_bKeyUp[3] = true;

				if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
				{
					if (m_bKeyUp[0] || m_fKeyUp[0] == 0.0f)
					{
						Rotate();
						m_bRefresh = true;

						m_fKeyUp[0] = 100.0f;
						if (m_bKeyUp[0])
							m_fKeyUp[0] = 400.0f;
					}
					m_bKeyUp[0] = false;
				}
				else
					m_bKeyUp[0] = true;

				if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
				{
					if (m_bKeyUp[1])
					{
						while (MoveDown());
						m_bRefresh = true;
						m_timeUpdate = 0.0f;
						m_pSE->Play(4);
						m_iSECount = 0;
					}
					m_bKeyUp[1] = false;
				}
				else
					m_bKeyUp[1] = true;
			}

			for (int y = 0; y < m_cyField; y++)
			{
				bool bClear = true;
				for (int x = 0; x < m_cxField; x++)
				{
					if (m_iColorStock[x][y] == -1)
					{
						bClear = false;
						break;
					}
				}
				if (bClear)
				{
					m_vecClear.push_back(y);
					for (int x = 0; x < m_cxField; x++)
						m_iColorStock[x][y] = 12;
					m_bClear = false;
					m_bRefresh = true;
				}
			}

			if (m_bRefresh)
			{
				GetScrn().Set(false, WHITE, &m_rcField);

				RECT rcNew = { 0,0,0,0 };

				for (int y = 0; y < m_cyField; y++)
				{
					for (int x = 0; x < m_cxField; x++)
					{
						if (m_iColorStock[x][y] != -1)
						{
							rcNew.left = (m_ixField + x) * m_iSize;
							rcNew.top = (m_iyField + y) * m_iSize;
							rcNew.right = rcNew.left + m_iSize;
							rcNew.bottom = rcNew.top + m_iSize;
							m_bHighFPS ?
								GetScrn().SetFrame(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew) :
								GetScrn().Set(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew);
						}
					}
				}

				for (int i = 0; i < 4; i++)
				{
					int x = m_posCur.x + m_xPiece[m_iTypeCur][m_iRotCur][i];
					int y = m_posCur.y + m_yPiece[m_iTypeCur][m_iRotCur][i];

					rcNew.left = (m_ixField + x) * m_iSize;
					rcNew.top = (m_iyField + y) * m_iSize;
					rcNew.right = rcNew.left + m_iSize;
					rcNew.bottom = rcNew.top + m_iSize;
					if (!m_bNewPiece)
					{
						m_bHighFPS ?
							GetScrn().SetFrame(true, m_colorPiece[m_iColorCur][0], &rcNew) :
							GetScrn().Set(true, m_colorPiece[m_iColorCur][0], &rcNew);
					}

					if (m_bTraject)
					{
						int yTraject = y + 1;
						while (m_iColorStock[x][yTraject] == -1 && yTraject < m_cyField)
						{
							OffsetRect(&rcNew, 0, m_iSize);
							if (m_bHighFPS)
							{
								for (int y = rcNew.top; y < rcNew.bottom; y++)
								{
									GetScrn().Set(true, GRAY(3), rcNew.left, y);
									GetScrn().Set(true, GRAY(3), rcNew.right - 1, y);
								}
							}
							else
								GetScrn().Set(true, GRAY(3), &rcNew);
							yTraject++;
						}
					}
				}

				GetScrn().Set(false, WHITE, &m_rcNext);

				for (int i = 0; i < 4; i++)
				{
					rcNew.left = m_rcNext.left + m_xPiece[m_iTypeNext][0][i] * 8;
					rcNew.top = m_rcNext.top + m_yPiece[m_iTypeNext][0][i] * 8;
					rcNew.right = rcNew.left + 8;
					rcNew.bottom = rcNew.top + 8;
					m_bHighFPS ?
						GetScrn().SetFrame(true, m_colorPiece[m_iColorNext][0], &rcNew) :
						GetScrn().Set(true, m_colorPiece[m_iColorNext][0], &rcNew);
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

void SceneTetris::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);

	int iLang = GetSettings()._iLanguage;
	ID3DXFont *pFont = m_vecFont[iLang];

	if (m_bGuide)
	{
		D3DXCOLOR color = WHITE;
		color.a = m_timeShow / 500.0f;
		RECT rcGuide = m_rcScreen;
		rcGuide.left = (11 + iLang) * rcGuide.right / 40;

		pFont->DrawText(NULL, iLang ?
			L"目标：\n\n将方块排列成完整的\n一行或多行并消除\n\n\
			控制：\n\n方向键 或 WSAD\n左/右  - 移动\n上     - 旋转\n下     - 落下\n\nCtrl   - 加速\nTab    - 简单模式\n" :
			L"Target:\n\nCreate lines without gaps \nto destroy the pieces.\n\n\
			Control:\n\nArrow Keys or WSAD\nLeft and Right  - Move\n\
			Up - Rotate\nDown - Fall\n\nCtrl  - Accelerate\nTab  - Easy Mode",
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

		pFont->DrawText(NULL, iLang ? L"下一个" : L"NEXT", -1, &m_rcTextRTop, DT_CENTER | DT_VCENTER, WHITE);

		swprintf_s(szBuffer, iLang ? L"第%d关" : L"Stage %d", m_iStage);
		pFont->DrawText(NULL, szBuffer, -1, &m_rcTextLeft, DT_CENTER | DT_TOP, WHITE);

		swprintf_s(szBuffer, iLang ? L"分数 %02d\n\n目标 %02d" : 
			L"Score %02d\n\nTarget %02d", m_iScore, m_iTarScore);
		pFont->DrawText(NULL, szBuffer, -1, &m_rcTextRBottom, DT_CENTER | DT_BOTTOM, WHITE);

		if (!m_bWin && !m_bLose)
		{
			pFont->DrawText(NULL, iLang ? L"Esc - 暂停" : 
				L"Esc\nPause", -1, &m_rcTextLeft, DT_CENTER | DT_BOTTOM, WHITE);
		}
	}

	if (m_bWin)
	{
		if (m_iStage < 6)
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

void SceneTetris::Refresh()
{
	GetScrn().Set(false, WHITE);
	SetGridRect(&m_rcField, m_ixField, m_iyField, m_ixField + m_cxField, m_iyField + m_cyField, m_iSize);

	if (m_bHighFPS)
	{
		ExpandRect(&m_rcField, 8);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 7);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 1);

		SetGridRect(&m_rcNext, 23, 4, 29, 8, 8);
		GetScrn().SetFrame(true, GRAY(10), &m_rcNext);
		SetGridRect(&m_rcNext, 24, 5, 28, 7, 8);
		ExpandRect(&m_rcNext, 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcNext);
		ShrinkRect(&m_rcNext, 1);
	}
	else
	{
		ExpandRect(&m_rcField, 8);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 8);
		GetScrn().Set(false, WHITE, &m_rcField);

		SetGridRect(&m_rcNext, 23, 4, 29, 8, 8);
		GetScrn().Set(true, GRAY(10), &m_rcNext);
		SetGridRect(&m_rcNext, 24, 5, 28, 7, 8);
		GetScrn().Set(false, WHITE, &m_rcNext);
	}

	RECT rcNew = { 0,0,0,0 };
	for (int y = 0; y < m_cyField; y++)
	{
		for (int x = 0; x < m_cxField; x++)
		{
			if (m_iColorStock[x][y] != -1)
			{
				rcNew.left = (m_ixField + x) * m_iSize;
				rcNew.top = (m_iyField + y) * m_iSize;
				rcNew.right = rcNew.left + m_iSize;
				rcNew.bottom = rcNew.top + m_iSize;
				m_bHighFPS ?
					GetScrn().SetFrame(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew) :
					GetScrn().Set(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew);
			}
		}
	}

	m_bRefresh = true;
	GetScrn().Refresh(IMM, NULL);
}

void SceneTetris::InitField()
{
	switch (m_iStage)
	{
	case 1:
		m_iSize = 8;
		m_ixField = 12;
		m_iyField = 6;
		m_cxField = 6;
		m_cyField = 15;
		m_timePeriod = 1000.0f;

		for (int y = 0; y < m_cyField; y++)
			for (int x = 0; x < m_cxField; x++)
				m_iColorStock[x][y] = -1;
		break;

	case 2:
		m_iSize = 8;
		m_ixField = 11;
		m_iyField = 5;
		m_cxField = 8;
		m_cyField = 16;
		m_timePeriod = 1000.0f;

		for (int y = m_cyField - 1; y >= 0; y--)
		{
			for (int x = m_cxField - 1; x >= 0; x--)
			{
				if (y >= 1 && x >= 1 && x <= m_cxField - 2)
					m_iColorStock[x][y] = m_iColorPreStock[x - 1][y - 1];
				else
					m_iColorStock[x][y] = -1;
			}
		}
		break;

	case 3:
		m_iSize = 8;
		m_ixField = 9;
		m_iyField = 3;
		m_cxField = 12;
		m_cyField = 18;
		m_timePeriod = 1000.0f;

		for (int y = m_cyField - 1; y >= 0; y--)
		{
			for (int x = m_cxField - 1; x >= 0; x--)
			{
				if (y >= 2 && x >= 2 && x <= m_cxField - 3)
					m_iColorStock[x][y] = m_iColorPreStock[x - 2][y - 2];
				else
					m_iColorStock[x][y] = -1;
			}
		}
		break;

	case 4:
		m_iSize = 4;
		m_ixField = 18;
		m_iyField = 6;
		m_cxField = 24;
		m_cyField = 36;
		m_timePeriod = 1000.0f;

		for (int y = m_cyField - 1; y >= 0; y--)
		{
			for (int x = m_cxField - 1; x >= 0; x--)
			{
				m_iColorStock[x][y] = m_iColorPreStock[x / 2][y / 2];
			}
		}
		break;

	case 5:
		m_iSize = 2;
		m_ixField = 36;
		m_iyField = 12;
		m_cxField = 48;
		m_cyField = 72;
		m_timePeriod = 500.0f;

		for (int y = m_cyField - 1; y >= 0; y--)
		{
			for (int x = m_cxField - 1; x >= 0; x--)
			{
				m_iColorStock[x][y] = m_iColorPreStock[x / 2][y / 2];
			}
		}
		break;

	case 6:
		m_iSize = 1;
		m_ixField = 72;
		m_iyField = 24;
		m_cxField = 96;
		m_cyField = 144;
		m_timePeriod = 500.0f;

		for (int y = m_cyField - 1; y >= 0; y--)
		{
			for (int x = m_cxField - 1; x >= 0; x--)
			{
				m_iColorStock[x][y] = m_iColorPreStock[x / 2][y / 2];
			}
		}
		break;
	}

	m_iTarScore = 10;

	GetScrn().Set(false, WHITE);
	SetGridRect(&m_rcField, m_ixField, m_iyField, m_ixField + m_cxField, m_iyField + m_cyField, m_iSize);

	if (m_bHighFPS)
	{
		ExpandRect(&m_rcField, 8);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 7);
		GetScrn().SetFrame(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 1);

		SetGridRect(&m_rcNext, 23, 4, 29, 8, 8);
		GetScrn().SetFrame(true, GRAY(10), &m_rcNext);
		SetGridRect(&m_rcNext, 24, 5, 28, 7, 8);
		ExpandRect(&m_rcNext, 1);
		GetScrn().SetFrame(true, GRAY(10), &m_rcNext);
		ShrinkRect(&m_rcNext, 1);
	}
	else
	{
		ExpandRect(&m_rcField, 8);
		GetScrn().Set(true, GRAY(10), &m_rcField);
		ShrinkRect(&m_rcField, 8);
		GetScrn().Set(false, WHITE, &m_rcField);

		SetGridRect(&m_rcNext, 23, 4, 29, 8, 8);
		GetScrn().Set(true, GRAY(10), &m_rcNext);
		SetGridRect(&m_rcNext, 24, 5, 28, 7, 8);
		GetScrn().Set(false, WHITE, &m_rcNext);
	}

	SetPixRect(&m_rcTextLeft, 0, 19, 64, 148);
	SetPixRect(&m_rcTextRTop, 176, 16, CXPIX, 32);
	SetPixRect(&m_rcTextRBottom, 176, 64, CXPIX, 148);

	RECT rcNew = { 0,0,0,0 };
	for (int y = 0; y < m_cyField; y++)
	{
		for (int x = 0; x < m_cxField; x++)
		{
			if (m_iColorStock[x][y] != -1)
			{
				rcNew.left = (m_ixField + x) * m_iSize;
				rcNew.top = (m_iyField + y) * m_iSize;
				rcNew.right = rcNew.left + m_iSize;
				rcNew.bottom = rcNew.top + m_iSize;
				m_bHighFPS ?
					GetScrn().SetFrame(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew) :
					GetScrn().Set(true, m_colorPiece[m_iColorStock[x][y]][1], &rcNew);
			}
		}
	}
	GetScrn().Refresh(BRUSH, NULL);

	m_bFieldInit = true;
}

void SceneTetris::InitStage()
{
	m_iScore = 0;
	m_timeUpdate = 0.0f;

	m_posCur.x = m_cxField / 2 - 2;
	m_posCur.y = 0;
	m_iRotCur = 0;
	m_iTypeCur = rand() % 7;
	m_iColorCur = rand() % 3;

	m_iTypeNext = rand() % 7;
	m_iColorNext = rand() % 12;

	m_bNewPiece = false;

	m_pBGM->Play(1);

	m_bStageInit = true;
	m_bClear = true;
}

bool SceneTetris::MoveLeft()
{
	return SetPos(m_posCur.x - 1, m_posCur.y, m_iRotCur);
}

bool SceneTetris::MoveRight()
{
	return SetPos(m_posCur.x + 1, m_posCur.y, m_iRotCur);
}

bool SceneTetris::MoveDown()
{
	if (SetPos(m_posCur.x, m_posCur.y + 1, m_iRotCur))
		return true;
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int x = m_posCur.x + m_xPiece[m_iTypeCur][m_iRotCur][i];
			int y = m_posCur.y + m_yPiece[m_iTypeCur][m_iRotCur][i];
			m_iColorStock[x][y] = m_iColorCur;
		}
		m_bNewPiece = true;
		return false;
	}
}

bool SceneTetris::Rotate()
{
	int iRot = (m_iRotCur + 1) % 4;

	if (SetPos(m_posCur.x, m_posCur.y, iRot))
		return true;
	else if (SetPos(m_posCur.x - 1, m_posCur.y, iRot))
		return true;
	else if (SetPos(m_posCur.x + 1, m_posCur.y, iRot))
		return true;
	else if (SetPos(m_posCur.x - 2, m_posCur.y, iRot))
		return true;
	else if (SetPos(m_posCur.x + 2, m_posCur.y, iRot))
		return true;
	else if (SetPos(m_posCur.x, m_posCur.y + 1, iRot))
		return true;
	else if (SetPos(m_posCur.x - 1, m_posCur.y + 1, iRot))
		return true;
	else if (SetPos(m_posCur.x + 1, m_posCur.y + 1, iRot))
		return true;
	else if (SetPos(m_posCur.x, m_posCur.y + 2, iRot))
		return true;
	else
		return false;
}

bool SceneTetris::SetPos(int x, int y, int iRot)
{
	for (int i = 0; i < 4; i++)
	{
		int xNew = x + m_xPiece[m_iTypeCur][iRot][i];
		int yNew = y + m_yPiece[m_iTypeCur][iRot][i];

		if (xNew < 0 || xNew >= m_cxField ||
			yNew < 0 || yNew >= m_cyField ||
			m_iColorStock[xNew][yNew] != -1)
			return false;
	}

	m_posCur.x = x;
	m_posCur.y = y;
	m_iRotCur = iRot;
	return true;
}