#include "DXColor.h"
#include "DXInput.h"
#include "resource.h"
#include "SaveData.h"
#include "Settings.h"
#include "SceneExtra.h"

SceneExtra::SceneExtra()
{

}

SceneExtra::~SceneExtra()
{

}

void SceneExtra::Init(int iInitVal)
{
	GetScrn().Set(false, WHITE);
	GetScrn().Refresh(BRUSH, NULL);
	GetScrn().SetRefPeriod(2.0f);

	m_bPlay = false;
	m_iPlay = -1;
	m_iFocus = 0;
	m_bChange = true;
	m_bListShow = false;
	m_nList = 3 + GetSaveData()._iStage[3] + (GetSaveData()._iStage[3] > 4 ? 1 : 0);
	SetButton(m_iFocus);

	if (GetSaveData()._iStage[2] > 1)
	{
		SetAlterEgo();
		m_timeUpdateAE = 0.0f;
	}

	for (auto &bUp : m_bKeyUp)
		bUp = false;
	for (auto &fUp : m_fKeyUp)
		fUp = 100.0f;

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);

	//Sound
	m_pBGM = new Sound();
	m_pBGM->Register(ID_BGM_EXTRA, ID_SUBMIX_BACKGROUND);
	m_pBGM->AddSoundFromRsrc(IDR_TITLE, 255);
	m_pBGM->AddSoundFromRsrc(IDR_PONG, 255);
	m_pBGM->AddSoundFromRsrc(IDR_RACING, 255);
	m_pBGM->AddSoundFromRsrc(IDR_TETRIS, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE1, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE2, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE3, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE4, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE5, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE6, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE7, 255);
	m_pBGM->AddSoundFromRsrc(IDR_SNAKE8, 255);

	m_pSE = new Sound();
	m_pSE->Register(ID_SE_EXTRA, ID_SUBMIX_SOUNDEFFECT);
	m_pSE->AddSoundFromRsrc(IDR_SELECT);
}

void SceneExtra::End()
{
	SAFE_DELETE(m_pBGM);
	SAFE_DELETE(m_pSE);
	SAFE_RELEASE(m_pFont);
}

void SceneExtra::OnResetDevice()
{
	SAFE_RELEASE(m_pFont);

	D3DXCreateFont(DXUTGetD3DDevice(), 11 * GetSettings()._iPixSize, 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);

}

void SceneExtra::OnLostDevice()
{
	SAFE_RELEASE(m_pFont);

}

void SceneExtra::Update(double fTime, float fElapsedTime)
{
	GetInput();
	
	if (IsKeyDown(DIK_ESCAPE))
	{
		if (m_bKeyUp[6])
		{
			SetMode(GAME_MENU);
			SetScene(MENU);
			SetInitVal(0);
			GetScrn().Set(false, WHITE);
		}
		m_bKeyUp[6] = false;
		return;
	}
	else
		m_bKeyUp[6] = true;

	if (GetSaveData()._iStage[2] > 1 && m_bPlay && m_iPlay != 3)
	{
		m_timeUpdateAE += fElapsedTime;
		if (m_timeUpdateAE >= 0.0f)
		{
			GetScrn().Set(false, WHITE, &m_vecAE[m_vecAEIndex[m_iAEIndex]]);

			if (m_iAEIndex == 0 && m_iPlay <= 3)
				m_iAEIndex = 3;
			else
				m_iAEIndex++;
				
			if (m_iPlay <= 3)
			{
				if (m_iAEIndex > 10)
					m_iAEIndex = 3;
			}
			else
			{
				if (m_iAEIndex > 12)
					m_iAEIndex = 1;
			}

			GetScrn().Set(true, WHITE, &m_vecAE[m_vecAEIndex[m_iAEIndex]]);
			GetScrn().Refresh(IMM, &m_rcAE);
			m_timeUpdateAE -= m_vecSPB[m_iPlay];
		}
	}

	if (GetScrn().IsRefOver())
	{
		if (!m_bListShow)
		{
			m_bListShow = true;
			if (GetSaveData()._iStage[2] > 1)
			{
				GetScrn().Set(true, WHITE, &m_vecAE[0]);
				GetScrn().Refresh(IMM, &m_rcAE);
			}
			return;
		}

		if (m_bChange)
		{
			SetButton(m_iFocus);
			GetScrn().Set(true, WHITE, &m_rcButton);
			GetScrn().Refresh(BRUSH, &m_rcButton);
			m_bChange = false;
		}
		else
		{
			if (IsKeyDown(DIK_UP))
			{
				if (m_iFocus != 0)
				{
					m_pSE->Play(0);
					GetScrn().Set(false, WHITE, &m_rcButton);
					GetScrn().Refresh(BRUSH, &m_rcButton);
					m_iFocus--;
					m_bChange = true;
				}
			}
			if (IsKeyDown(DIK_DOWN))
			{
				if (m_iFocus < m_nList)
				{
					m_pSE->Play(0);
					GetScrn().Set(false, WHITE, &m_rcButton);
					GetScrn().Refresh(BRUSH, &m_rcButton);
					m_iFocus++;
					m_bChange = true;
				}
			}
			if (IsKeyDown(DIK_RIGHT))
			{
				if (m_iFocus <= 3)
				{
					m_pSE->Play(0);
					GetScrn().Set(false, WHITE, &m_rcButton);
					GetScrn().Refresh(BRUSH, &m_rcButton);
					m_iFocus =min(m_iFocus == 0 ? 4 : m_iFocus + 3, m_nList);
					m_bChange = true;
				}
			}
			if (IsKeyDown(DIK_LEFT))
			{
				if (m_iFocus >= 4)
				{
					m_pSE->Play(0);
					GetScrn().Set(false, WHITE, &m_rcButton);
					GetScrn().Refresh(BRUSH, &m_rcButton);
					m_iFocus = m_iFocus >= 7 ? 3 : m_iFocus - 3;
					m_bChange = true;
				}
			}
			if (IsKeyDown(DIK_RETURN))
			{
				if (m_bKeyUp[5])
				{
					if (m_iFocus == m_iPlay)
					{
						if (m_bPlay)
						{
							m_pBGM->Stop();
							m_bPlay = false;
						}
						else
						{
							m_pBGM->Play();
							m_bPlay = true;
						}
					}
					else
					{
						m_bPlay = true;
						m_pBGM->End();
						m_pBGM->Play(m_iFocus);
						m_iPlay = m_iFocus;
						if (GetSaveData()._iStage[2] > 1)
						{
							GetScrn().Set(false, WHITE, &m_vecAE[m_vecAEIndex[m_iAEIndex]]);
							GetScrn().Set(true, WHITE, &m_vecAE[0]);
							GetScrn().Refresh(IMM, &m_rcAE);
							m_iAEIndex = 0;
							m_timeUpdateAE = 0.0f;
						}
					}

					m_bKeyUp[5] = false;
				}
			}
			else
				m_bKeyUp[5] = true;
		}
	}

	if (!GetScrn().IsRefOver())
	{
		GetScrn().Update(fTime, fElapsedTime);
	}
}

void SceneExtra::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);

	if (m_bListShow)
	{
		for (size_t i = 0; i < m_vecList.size(); i++)
		{
			RECT rcTitle;
			if (i < 4)
				SetPixRect(&rcTitle, 37, 20 + 16 * i, 100, 32 + 16 * i);
			else
				SetPixRect(&rcTitle, CXPIX / 2 + 21, 20 + 16 * (i - 3), CXPIX / 2 + 80, 32 + 16 * (i - 3));

			if (i<= m_nList)
				m_pFont->DrawText(NULL, m_vecList[i], -1, &rcTitle, DT_LEFT | DT_TOP,
					i == m_iFocus ? GRAY(2) : WHITE);
			else
				m_pFont->DrawText(NULL, m_vecList[i], -1, &rcTitle, DT_LEFT | DT_TOP, GRAY(6));

		}
	}
}

void SceneExtra::SetButton(int index)
{
	if (index < 4)
		SetRect(&m_rcButton, 36, 20 + 16 * index, 81, 32 + 16 * index);
	else
		SetRect(&m_rcButton, CXPIX / 2 + 20, 20 + 16 * (index - 3), CXPIX / 2 + 58, 32 + 16 * (index - 3));
}

void SceneExtra::SetAlterEgo()
{
	SetRect(&m_rcAE, 40, 120, 72, 152);

	VECPIX vecAE;
	int xLB, yLB;

	//vecAE0

	vecAE.clear();

	//Foot

	xLB = m_rcAE.left + 11;
	yLB = m_rcAE.bottom - 1;

	for (int x = 0; x < 9; x++)
	{
		if (x == 0 || x == 3 || x == 5 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 3 });
		}

		if (x == 1 || x == 2 || x == 6 || x == 7)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}
	}

	//Body

	xLB = m_rcAE.left + 8;
	yLB = m_rcAE.bottom - 5;

	for (int x = 0; x < 15; x++)
	{
		if (x != 0 && x!= 14)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if (x == 1 || x == 3 || x == 11 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 6 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x != 1 && x != 3 && x != 11 && x != 13)
			vecAE.push_back({ xLB + x, yLB - 3 });

		if (x <= 2 || x == 5 || x == 7 || x == 9 || x >= 12)
			vecAE.push_back({ xLB + x, yLB - 7 });
	}

	vecAE.push_back({ xLB + 0, yLB - 2 });
	vecAE.push_back({ xLB + 2, yLB - 6 });
	vecAE.push_back({ xLB + 7, yLB - 4 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 14, yLB - 2 });

	//Head

	xLB = m_rcAE.left + 4;
	yLB = m_rcAE.bottom - 13;

	for (int x = 0; x < 23; x++)
	{
		if (x == 3 || (x >= 7 && x <= 15) || x == 19)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if ((x >= 4 && x <= 6) || (x >= 16 && x <= 18))
			vecAE.push_back({ xLB + x, yLB - 1 });

		if ((x >= 1 && x <= 3) || x == 6 || (x >= 10 && x <= 12) || x == 16 || x == 19)
			vecAE.push_back({ xLB + x, yLB - 2 });

		if (x == 4 || x == 6 || x == 16 || x == 18)
		{
			vecAE.push_back({ xLB + x, yLB - 3 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x == 8 || x == 9 || x == 13 || x == 14)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		vecAE.push_back({ xLB + 0, yLB - 6 });

		if (x <= 1)
			vecAE.push_back({ xLB + x, yLB - x - 8 });

		if (x <= 2)
			vecAE.push_back({ xLB + x, yLB - x - 3 });
		
		if (x >= 1 && x <= 4)
		{
			vecAE.push_back({ xLB + x, yLB - x - 4 });
			vecAE.push_back({ xLB + x, yLB - x - 6 });
			vecAE.push_back({ xLB + x, yLB - x - 9 });
			vecAE.push_back({ xLB + x, yLB - x - 11 });
		}

		if ((x >= 6 && x <= 8) || (x >= 10 && x <= 15))
			vecAE.push_back({ xLB + x, yLB - 16 });
		
		if (x >= 19 && x <= 21)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 8 });
		}
	}

	vecAE.push_back({ xLB + 5, yLB - 15 });
	vecAE.push_back({ xLB + 6, yLB - 7 });
	vecAE.push_back({ xLB + 6, yLB - 8 });
	vecAE.push_back({ xLB + 6, yLB - 9 });
	vecAE.push_back({ xLB + 7, yLB - 7 });
	vecAE.push_back({ xLB + 8, yLB - 9 });
	vecAE.push_back({ xLB + 8, yLB - 10 });
	vecAE.push_back({ xLB + 8, yLB - 14 });
	vecAE.push_back({ xLB + 9, yLB - 8 });
	vecAE.push_back({ xLB + 9, yLB - 15 });
	vecAE.push_back({ xLB + 10, yLB - 7 });
	vecAE.push_back({ xLB + 11, yLB - 6 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 12, yLB - 9 });
	vecAE.push_back({ xLB + 12, yLB - 10 });
	vecAE.push_back({ xLB + 13, yLB - 8 });
	vecAE.push_back({ xLB + 14, yLB - 7 });
	vecAE.push_back({ xLB + 15, yLB - 6 });
	vecAE.push_back({ xLB + 15, yLB - 15 });
	vecAE.push_back({ xLB + 16, yLB - 8 });
	vecAE.push_back({ xLB + 16, yLB - 9 });
	vecAE.push_back({ xLB + 16, yLB - 12 });
	vecAE.push_back({ xLB + 16, yLB - 15 });
	vecAE.push_back({ xLB + 17, yLB - 7 });
	vecAE.push_back({ xLB + 17, yLB - 11 });
	vecAE.push_back({ xLB + 17, yLB - 14 });
	vecAE.push_back({ xLB + 18, yLB - 9 });
	vecAE.push_back({ xLB + 18, yLB - 10 });
	vecAE.push_back({ xLB + 18, yLB - 13 });
	vecAE.push_back({ xLB + 19, yLB - 11 });
	vecAE.push_back({ xLB + 19, yLB - 12 });
	vecAE.push_back({ xLB + 20, yLB - 3 });
	vecAE.push_back({ xLB + 20, yLB - 7 });
	vecAE.push_back({ xLB + 20, yLB - 10 });
	vecAE.push_back({ xLB + 21, yLB - 6 });
	vecAE.push_back({ xLB + 21, yLB - 9 });
	vecAE.push_back({ xLB + 22, yLB - 5 });

	m_vecAE.push_back(vecAE);

	//vecAE1

	vecAE.clear();

	//Foot

	xLB = m_rcAE.left + 11;
	yLB = m_rcAE.bottom - 1;

	for (int x = 0; x < 9; x++)
	{
		if (x == 0 || x == 3 || x == 5 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}

		if (x == 1 || x == 2 || x == 6 || x == 7)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}
	}
	
	//Body

	xLB = m_rcAE.left + 8;
	yLB = m_rcAE.bottom - 4;

	for (int x = 0; x < 15; x++)
	{
		if (x != 0 && x != 14)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if (x == 1 || x == 3 || x == 11 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 6 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x != 1 && x != 3 && x != 11 && x != 13)
			vecAE.push_back({ xLB + x, yLB - 3 });

		if (x <= 2 || x == 5 || x == 7 || x == 9 || x >= 12)
			vecAE.push_back({ xLB + x, yLB - 7 });
	}

	vecAE.push_back({ xLB + 0, yLB - 2 });
	vecAE.push_back({ xLB + 2, yLB - 6 });
	vecAE.push_back({ xLB + 7, yLB - 4 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 14, yLB - 2 });

	//Head

	xLB = m_rcAE.left + 4;
	yLB = m_rcAE.bottom - 12;

	for (int x = 0; x < 23; x++)
	{
		if (x == 3 || (x >= 6 && x <= 15) || x == 19)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if ((x >= 3 && x <= 5) || (x >= 16 && x <= 18))
			vecAE.push_back({ xLB + x, yLB - 1 });

		if ((x >= 1 && x <= 2) || x == 5 || (x >= 9 && x <= 11) || x == 16 || x == 19)
			vecAE.push_back({ xLB + x, yLB - 2 });

		if (x == 3 || x == 5 || x == 16 || x == 18)
		{
			vecAE.push_back({ xLB + x, yLB - 3 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 7 || x == 8 || x == 12 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x <= 1)
		{
			vecAE.push_back({ xLB + x, yLB - x - 3 });
			vecAE.push_back({ xLB + x, yLB - x - 8 });
		}

		if (x <= 3)
			vecAE.push_back({ xLB + x, yLB - x - 6 });

		if (x >= 1 && x <= 3)
			vecAE.push_back({ xLB + x, yLB - x - 4 });

		if (x >= 1 && x <= 4)
		{
			vecAE.push_back({ xLB + x, yLB - x - 9 });
			vecAE.push_back({ xLB + x, yLB - x - 11 });
		}

		if ((x >= 6 && x <= 8) || (x >= 10 && x <= 15))
			vecAE.push_back({ xLB + x, yLB - 16 });

		if (x >= 19 && x <= 21)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 8 });
		}
	}

	vecAE.push_back({ xLB + 5, yLB - 6 });
	vecAE.push_back({ xLB + 5, yLB - 7 });
	vecAE.push_back({ xLB + 5, yLB - 8 });
	vecAE.push_back({ xLB + 5, yLB - 9 });
	vecAE.push_back({ xLB + 5, yLB - 15 });
	vecAE.push_back({ xLB + 6, yLB - 7 });
	vecAE.push_back({ xLB + 7, yLB - 9 });
	vecAE.push_back({ xLB + 7, yLB - 10 });
	vecAE.push_back({ xLB + 8, yLB - 8 });
	vecAE.push_back({ xLB + 8, yLB - 14 });
	vecAE.push_back({ xLB + 9, yLB - 7 });
	vecAE.push_back({ xLB + 9, yLB - 15 });
	vecAE.push_back({ xLB + 10, yLB - 6 });
	vecAE.push_back({ xLB + 11, yLB - 6 });
	vecAE.push_back({ xLB + 11, yLB - 9 });
	vecAE.push_back({ xLB + 11, yLB - 10 });
	vecAE.push_back({ xLB + 12, yLB - 8 });
	vecAE.push_back({ xLB + 13, yLB - 7 });
	vecAE.push_back({ xLB + 14, yLB - 6 });
	vecAE.push_back({ xLB + 15, yLB - 6 });
	vecAE.push_back({ xLB + 15, yLB - 9 });
	vecAE.push_back({ xLB + 15, yLB - 15 });
	vecAE.push_back({ xLB + 16, yLB - 6 });
	vecAE.push_back({ xLB + 16, yLB - 8 });
	vecAE.push_back({ xLB + 16, yLB - 12 });
	vecAE.push_back({ xLB + 16, yLB - 15 });
	vecAE.push_back({ xLB + 17, yLB - 7 });
	vecAE.push_back({ xLB + 17, yLB - 11 });
	vecAE.push_back({ xLB + 17, yLB - 14 });
	vecAE.push_back({ xLB + 18, yLB - 6 });
	vecAE.push_back({ xLB + 18, yLB - 9 });
	vecAE.push_back({ xLB + 18, yLB - 10 });
	vecAE.push_back({ xLB + 18, yLB - 13 });
	vecAE.push_back({ xLB + 19, yLB - 11 });
	vecAE.push_back({ xLB + 19, yLB - 12 });
	vecAE.push_back({ xLB + 20, yLB - 3 });
	vecAE.push_back({ xLB + 20, yLB - 7 });
	vecAE.push_back({ xLB + 20, yLB - 10 });
	vecAE.push_back({ xLB + 21, yLB - 6 });
	vecAE.push_back({ xLB + 21, yLB - 9 });
	vecAE.push_back({ xLB + 22, yLB - 5 });

	m_vecAE.push_back(vecAE);

	//vecAE2

	vecAE.clear();

	//Foot

	xLB = m_rcAE.left + 11;
	yLB = m_rcAE.bottom - 1;

	for (int x = 0; x < 9; x++)
	{
		if (x == 5 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 3 });
		}

		if (x == 6 || x == 7)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}
	}

	vecAE.push_back({ xLB - 4, yLB - 2 });
	vecAE.push_back({ xLB - 4, yLB - 3 });
	vecAE.push_back({ xLB - 3, yLB - 1 });
	vecAE.push_back({ xLB - 3, yLB - 3 });
	vecAE.push_back({ xLB - 2, yLB - 0 });
	vecAE.push_back({ xLB - 2, yLB - 2 });
	vecAE.push_back({ xLB - 1, yLB - 1 });
	vecAE.push_back({ xLB - 1, yLB - 3 });
	vecAE.push_back({ xLB - 0, yLB - 2 });
	vecAE.push_back({ xLB + 1, yLB - 3 });
	
	//Body

	xLB = m_rcAE.left + 8;
	yLB = m_rcAE.bottom - 5;

	for (int x = 0; x < 15; x++)
	{
		if (x != 0 && x != 14)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if (x == 1 || x == 3 || x == 6 || x == 8 || x == 11 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}

		if (x == 3 || x == 11)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}
		
		if (x == 6 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x != 1 && x != 3 && x != 11 && x != 13)
			vecAE.push_back({ xLB + x, yLB - 3 });

		if (x <= 2 || x == 5 || x == 7 || x == 9 || x >= 12)
			vecAE.push_back({ xLB + x, yLB - 7 });
	}

//	vecAE.push_back({ xLB - 2, yLB - 3 });
//	vecAE.push_back({ xLB - 1, yLB - 2 });
	vecAE.push_back({ xLB - 1, yLB - 4 });
	vecAE.push_back({ xLB + 0, yLB - 5 });
	vecAE.push_back({ xLB + 1, yLB - 4 });
	vecAE.push_back({ xLB + 1, yLB - 6 });
	vecAE.push_back({ xLB + 2, yLB - 5 });
	vecAE.push_back({ xLB + 7, yLB - 4 });
	vecAE.push_back({ xLB + 12, yLB - 5 });
	vecAE.push_back({ xLB + 13, yLB - 4 });
	vecAE.push_back({ xLB + 13, yLB - 6 });
	vecAE.push_back({ xLB + 14, yLB - 5 });
//	vecAE.push_back({ xLB + 15, yLB - 2 });
	vecAE.push_back({ xLB + 15, yLB - 4 });
//	vecAE.push_back({ xLB + 16, yLB - 3 });

	//Head

	xLB = m_rcAE.left + 4;
	yLB = m_rcAE.bottom - 13;

	for (int x = 0; x < 23; x++)
	{
		if (x == 3 || (x >= 6 && x <= 15) || x == 19)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if ((x >= 3 && x <= 5) || (x >= 16 && x <= 18))
			vecAE.push_back({ xLB + x, yLB - 1 });

		if ((x >= 1 && x <= 2) || x == 5 || (x >= 9 && x <= 11) || x == 16 || x == 19)
			vecAE.push_back({ xLB + x, yLB - 2 });

		if (x == 3 || x == 5 || x == 16 || x == 18)
		{
			vecAE.push_back({ xLB + x, yLB - 3 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 7 || x == 8 || x == 12 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x <= 1)
		{
			vecAE.push_back({ xLB + x, yLB - x - 3 });
			vecAE.push_back({ xLB + x, yLB - x - 8 });
		}

		if (x <= 3)
			vecAE.push_back({ xLB + x, yLB - x - 6 });

		if (x >= 1 && x <= 3)
			vecAE.push_back({ xLB + x, yLB - x - 4 });

		if (x >= 1 && x <= 4)
		{
			vecAE.push_back({ xLB + x, yLB - x - 9 });
			vecAE.push_back({ xLB + x, yLB - x - 11 });
		}

		if ((x >= 6 && x <= 8) || (x >= 10 && x <= 15))
			vecAE.push_back({ xLB + x, yLB - 16 });

		if (x >= 19 && x <= 21)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 8 });
		}
	}

	vecAE.push_back({ xLB + 5, yLB - 6 });
	vecAE.push_back({ xLB + 5, yLB - 7 });
	vecAE.push_back({ xLB + 5, yLB - 8 });
	vecAE.push_back({ xLB + 5, yLB - 9 });
	vecAE.push_back({ xLB + 5, yLB - 15 });
	vecAE.push_back({ xLB + 6, yLB - 7 });
	vecAE.push_back({ xLB + 7, yLB - 9 });
	vecAE.push_back({ xLB + 7, yLB - 10 });
	vecAE.push_back({ xLB + 8, yLB - 8 });
	vecAE.push_back({ xLB + 8, yLB - 14 });
	vecAE.push_back({ xLB + 9, yLB - 7 });
	vecAE.push_back({ xLB + 9, yLB - 15 });
	vecAE.push_back({ xLB + 10, yLB - 6 });
	vecAE.push_back({ xLB + 11, yLB - 6 });
	vecAE.push_back({ xLB + 11, yLB - 9 });
	vecAE.push_back({ xLB + 11, yLB - 10 });
	vecAE.push_back({ xLB + 12, yLB - 8 });
	vecAE.push_back({ xLB + 13, yLB - 7 });
	vecAE.push_back({ xLB + 14, yLB - 6 });
	vecAE.push_back({ xLB + 15, yLB - 6 });
	vecAE.push_back({ xLB + 15, yLB - 9 });
	vecAE.push_back({ xLB + 15, yLB - 15 });
	vecAE.push_back({ xLB + 16, yLB - 6 });
	vecAE.push_back({ xLB + 16, yLB - 8 });
	vecAE.push_back({ xLB + 16, yLB - 12 });
	vecAE.push_back({ xLB + 16, yLB - 15 });
	vecAE.push_back({ xLB + 17, yLB - 7 });
	vecAE.push_back({ xLB + 17, yLB - 11 });
	vecAE.push_back({ xLB + 17, yLB - 14 });
	vecAE.push_back({ xLB + 18, yLB - 6 });
	vecAE.push_back({ xLB + 18, yLB - 9 });
	vecAE.push_back({ xLB + 18, yLB - 10 });
	vecAE.push_back({ xLB + 18, yLB - 13 });
	vecAE.push_back({ xLB + 19, yLB - 11 });
	vecAE.push_back({ xLB + 19, yLB - 12 });
	vecAE.push_back({ xLB + 20, yLB - 3 });
	vecAE.push_back({ xLB + 20, yLB - 7 });
	vecAE.push_back({ xLB + 20, yLB - 10 });
	vecAE.push_back({ xLB + 21, yLB - 6 });
	vecAE.push_back({ xLB + 21, yLB - 9 });
	vecAE.push_back({ xLB + 22, yLB - 5 });

	m_vecAE.push_back(vecAE);

	//vecAE3

	vecAE.clear();

	//Foot

	xLB = m_rcAE.left + 11;
	yLB = m_rcAE.bottom - 1;

	for (int x = 0; x < 9; x++)
	{
		if (x == 0 || x == 3 || x == 5 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}

		if (x == 1 || x == 2 || x == 6 || x == 7)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}
	}

	//Body

	xLB = m_rcAE.left + 8;
	yLB = m_rcAE.bottom - 4;

	for (int x = 0; x < 15; x++)
	{
		if (x != 0 && x != 14)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if (x == 1 || x == 3 || x == 11 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 6 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x != 1 && x != 3 && x != 11 && x != 13)
			vecAE.push_back({ xLB + x, yLB - 3 });

		if (x <= 2 || x == 5 || x == 7 || x == 9 || x >= 12)
			vecAE.push_back({ xLB + x, yLB - 7 });
	}

	vecAE.push_back({ xLB + 0, yLB - 2 });
	vecAE.push_back({ xLB + 2, yLB - 6 });
	vecAE.push_back({ xLB + 7, yLB - 4 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 14, yLB - 2 });

	//Head

	xLB = m_rcAE.left + 4;
	yLB = m_rcAE.bottom - 12;

	for (int x = 0; x < 23; x++)
	{
		if (x == 3 || (x >= 7 && x <= 16) || x == 19)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if ((x >= 4 && x <= 6) || (x >= 17 && x <= 19))
			vecAE.push_back({ xLB + x, yLB - 1 });

		if ((x >= 1 && x <= 3) || x == 6 || (x >= 11 && x <= 13) || x == 17 || x == 20)
			vecAE.push_back({ xLB + x, yLB - 2 });

		if (x == 4 || x == 6 || x == 17 || x == 19)
		{
			vecAE.push_back({ xLB + x, yLB - 3 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x == 9 || x == 10 || x == 14 || x == 15)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		vecAE.push_back({ xLB + 0, yLB - 6 });

		if (x <= 1)
			vecAE.push_back({ xLB + x, yLB - x - 8 });

		if (x <= 2)
			vecAE.push_back({ xLB + x, yLB - x - 3 });

		if (x >= 1 && x <= 4)
		{
			vecAE.push_back({ xLB + x, yLB - x - 4 });
			vecAE.push_back({ xLB + x, yLB - x - 6 });
			vecAE.push_back({ xLB + x, yLB - x - 9 });
			vecAE.push_back({ xLB + x, yLB - x - 11 });
		}

		if ((x >= 6 && x <= 8) || (x >= 10 && x <= 15))
			vecAE.push_back({ xLB + x, yLB - 16 });

		if (x >= 20 && x <= 21)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 8 });
		}
	}

	vecAE.push_back({ xLB + 5, yLB - 15 });
	vecAE.push_back({ xLB + 6, yLB - 7 });
	vecAE.push_back({ xLB + 6, yLB - 8 });
	vecAE.push_back({ xLB + 6, yLB - 9 });
	vecAE.push_back({ xLB + 7, yLB - 7 });
	vecAE.push_back({ xLB + 8, yLB - 6 });
	vecAE.push_back({ xLB + 8, yLB - 14 });
	vecAE.push_back({ xLB + 9, yLB - 9 });
	vecAE.push_back({ xLB + 9, yLB - 10 });
	vecAE.push_back({ xLB + 9, yLB - 15 });
	vecAE.push_back({ xLB + 10, yLB - 8 });
	vecAE.push_back({ xLB + 11, yLB - 7 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 13, yLB - 6 });
	vecAE.push_back({ xLB + 13, yLB - 9 });
	vecAE.push_back({ xLB + 13, yLB - 10 });
	vecAE.push_back({ xLB + 14, yLB - 8 });
	vecAE.push_back({ xLB + 15, yLB - 7 });
	vecAE.push_back({ xLB + 15, yLB - 15 });
	vecAE.push_back({ xLB + 16, yLB - 6 });
	vecAE.push_back({ xLB + 16, yLB - 9 });
	vecAE.push_back({ xLB + 16, yLB - 12 });
	vecAE.push_back({ xLB + 16, yLB - 15 });
	vecAE.push_back({ xLB + 17, yLB - 8 });
	vecAE.push_back({ xLB + 17, yLB - 11 });
	vecAE.push_back({ xLB + 17, yLB - 14 });
	vecAE.push_back({ xLB + 18, yLB - 7 });
	vecAE.push_back({ xLB + 18, yLB - 9 });
	vecAE.push_back({ xLB + 18, yLB - 10 });
	vecAE.push_back({ xLB + 18, yLB - 13 });
	vecAE.push_back({ xLB + 19, yLB - 8 });
	vecAE.push_back({ xLB + 19, yLB - 11 });
	vecAE.push_back({ xLB + 19, yLB - 12 });
	vecAE.push_back({ xLB + 20, yLB - 7 });
	vecAE.push_back({ xLB + 20, yLB - 10 });
	vecAE.push_back({ xLB + 21, yLB - 6 });
	vecAE.push_back({ xLB + 21, yLB - 9 });
	vecAE.push_back({ xLB + 22, yLB - 5 });

	m_vecAE.push_back(vecAE);

	//vecAE4

	vecAE.clear();

	//Foot

	xLB = m_rcAE.left + 11;
	yLB = m_rcAE.bottom - 1;

	for (int x = 0; x < 9; x++)
	{
		if (x == 0 || x == 3)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
			vecAE.push_back({ xLB + x, yLB - 3 });
		}

		if (x == 1 || x == 2)
		{
			vecAE.push_back({ xLB + x, yLB - 0 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}
	}

	vecAE.push_back({ xLB + 7, yLB - 3 });
	vecAE.push_back({ xLB + 8, yLB - 2 });
	vecAE.push_back({ xLB + 9, yLB - 1 });
	vecAE.push_back({ xLB + 9, yLB - 3 });
	vecAE.push_back({ xLB + 10, yLB - 0 });
	vecAE.push_back({ xLB + 10, yLB - 2 });
	vecAE.push_back({ xLB + 11, yLB - 1 });
	vecAE.push_back({ xLB + 11, yLB - 3 });
	vecAE.push_back({ xLB + 12, yLB - 2 });
	vecAE.push_back({ xLB + 12, yLB - 3 });


	//Body

	xLB = m_rcAE.left + 8;
	yLB = m_rcAE.bottom - 5;

	for (int x = 0; x < 15; x++)
	{
		if (x != 0 && x != 14)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if (x == 1 || x == 3 || x == 6 || x == 8 || x == 11 || x == 13)
		{
			vecAE.push_back({ xLB + x, yLB - 1 });
			vecAE.push_back({ xLB + x, yLB - 2 });
		}

		if (x == 3 || x == 11)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		if (x == 6 || x == 8)
		{
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x != 1 && x != 3 && x != 11 && x != 13)
			vecAE.push_back({ xLB + x, yLB - 3 });

		if (x <= 2 || x == 5 || x == 7 || x == 9 || x >= 12)
			vecAE.push_back({ xLB + x, yLB - 7 });
	}

	//	vecAE.push_back({ xLB - 2, yLB - 3 });
	//	vecAE.push_back({ xLB - 1, yLB - 2 });
	vecAE.push_back({ xLB - 1, yLB - 4 });
	vecAE.push_back({ xLB + 0, yLB - 5 });
	vecAE.push_back({ xLB + 1, yLB - 4 });
	vecAE.push_back({ xLB + 1, yLB - 6 });
	vecAE.push_back({ xLB + 2, yLB - 5 });
	vecAE.push_back({ xLB + 7, yLB - 4 });
	vecAE.push_back({ xLB + 12, yLB - 5 });
	vecAE.push_back({ xLB + 13, yLB - 4 });
	vecAE.push_back({ xLB + 13, yLB - 6 });
	vecAE.push_back({ xLB + 14, yLB - 5 });
	//	vecAE.push_back({ xLB + 15, yLB - 2 });
	vecAE.push_back({ xLB + 15, yLB - 4 });
	//	vecAE.push_back({ xLB + 16, yLB - 3 });

	//Head

	xLB = m_rcAE.left + 4;
	yLB = m_rcAE.bottom - 13;

	for (int x = 0; x < 23; x++)
	{
		if (x == 3 || (x >= 7 && x <= 16) || x == 19)
			vecAE.push_back({ xLB + x, yLB - 0 });

		if ((x >= 4 && x <= 6) || (x >= 17 && x <= 19))
			vecAE.push_back({ xLB + x, yLB - 1 });

		if ((x >= 1 && x <= 3) || x == 6 || (x >= 11 && x <= 13) || x == 17 || x == 20)
			vecAE.push_back({ xLB + x, yLB - 2 });

		if (x == 4 || x == 6 || x == 17 || x == 19)
		{
			vecAE.push_back({ xLB + x, yLB - 3 });
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
			vecAE.push_back({ xLB + x, yLB - 6 });
		}

		if (x == 9 || x == 10 || x == 14 || x == 15)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 5 });
		}

		vecAE.push_back({ xLB + 0, yLB - 6 });

		if (x <= 1)
			vecAE.push_back({ xLB + x, yLB - x - 8 });

		if (x <= 2)
			vecAE.push_back({ xLB + x, yLB - x - 3 });

		if (x >= 1 && x <= 4)
		{
			vecAE.push_back({ xLB + x, yLB - x - 4 });
			vecAE.push_back({ xLB + x, yLB - x - 6 });
			vecAE.push_back({ xLB + x, yLB - x - 9 });
			vecAE.push_back({ xLB + x, yLB - x - 11 });
		}

		if ((x >= 6 && x <= 8) || (x >= 10 && x <= 15))
			vecAE.push_back({ xLB + x, yLB - 16 });

		if (x >= 20 && x <= 21)
		{
			vecAE.push_back({ xLB + x, yLB - 4 });
			vecAE.push_back({ xLB + x, yLB - 8 });
		}
	}

	vecAE.push_back({ xLB + 5, yLB - 15 });
	vecAE.push_back({ xLB + 6, yLB - 7 });
	vecAE.push_back({ xLB + 6, yLB - 8 });
	vecAE.push_back({ xLB + 6, yLB - 9 });
	vecAE.push_back({ xLB + 7, yLB - 7 });
	vecAE.push_back({ xLB + 8, yLB - 6 });
	vecAE.push_back({ xLB + 8, yLB - 14 });
	vecAE.push_back({ xLB + 9, yLB - 9 });
	vecAE.push_back({ xLB + 9, yLB - 10 });
	vecAE.push_back({ xLB + 9, yLB - 15 });
	vecAE.push_back({ xLB + 10, yLB - 8 });
	vecAE.push_back({ xLB + 11, yLB - 7 });
	vecAE.push_back({ xLB + 12, yLB - 6 });
	vecAE.push_back({ xLB + 13, yLB - 6 });
	vecAE.push_back({ xLB + 13, yLB - 9 });
	vecAE.push_back({ xLB + 13, yLB - 10 });
	vecAE.push_back({ xLB + 14, yLB - 8 });
	vecAE.push_back({ xLB + 15, yLB - 7 });
	vecAE.push_back({ xLB + 15, yLB - 15 });
	vecAE.push_back({ xLB + 16, yLB - 6 });
	vecAE.push_back({ xLB + 16, yLB - 9 });
	vecAE.push_back({ xLB + 16, yLB - 12 });
	vecAE.push_back({ xLB + 16, yLB - 15 });
	vecAE.push_back({ xLB + 17, yLB - 8 });
	vecAE.push_back({ xLB + 17, yLB - 11 });
	vecAE.push_back({ xLB + 17, yLB - 14 });
	vecAE.push_back({ xLB + 18, yLB - 7 });
	vecAE.push_back({ xLB + 18, yLB - 9 });
	vecAE.push_back({ xLB + 18, yLB - 10 });
	vecAE.push_back({ xLB + 18, yLB - 13 });
	vecAE.push_back({ xLB + 19, yLB - 8 });
	vecAE.push_back({ xLB + 19, yLB - 11 });
	vecAE.push_back({ xLB + 19, yLB - 12 });
	vecAE.push_back({ xLB + 20, yLB - 7 });
	vecAE.push_back({ xLB + 20, yLB - 10 });
	vecAE.push_back({ xLB + 21, yLB - 6 });
	vecAE.push_back({ xLB + 21, yLB - 9 });
	vecAE.push_back({ xLB + 22, yLB - 5 });

	m_vecAE.push_back(vecAE);

	m_vecAEIndex.push_back(0);
	m_vecAEIndex.push_back(2);
	m_vecAEIndex.push_back(1);
	m_vecAEIndex.push_back(2);
	m_vecAEIndex.push_back(1);
	m_vecAEIndex.push_back(2);
	m_vecAEIndex.push_back(1);
	m_vecAEIndex.push_back(4);
	m_vecAEIndex.push_back(3);
	m_vecAEIndex.push_back(4);
	m_vecAEIndex.push_back(3);
	m_vecAEIndex.push_back(4);
	m_vecAEIndex.push_back(3);

	m_iAEIndex = 0;
}
