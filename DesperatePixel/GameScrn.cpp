#include "GameScrn.h"
#include "DXColor.h"
#include "Settings.h"
#include "resource.h"

GameScrn::GameScrn()
{
	m_pd3dDevice = NULL;
	m_pVertexPix = NULL;
	m_pTexture = NULL;
}

GameScrn::~GameScrn()
{
	End();
}

BOOL GameScrn::Init()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	if (NULL == m_pd3dDevice)
		return FALSE;

	m_pd3dDevice->CreateVertexBuffer(4 * CXPIX * CYPIX * sizeof(RHWVERTEX), 0, RHW_FVF, D3DPOOL_MANAGED, &m_pVertexPix, NULL);
	
	D3DXCreateTextureFromResource(m_pd3dDevice, NULL, MAKEINTRESOURCE(IDR_TEX1 + GetSettings()._iPixSize - 1), &m_pTexture);

	m_iPixSize = GetSettings()._iPixSize;

	for (int y = 0; y < CYPIX; y++)
	{
		float fTop = (float)(y * m_iPixSize);
		float fBtm = (float)((y + 1) * m_iPixSize);

		for (int x = 0; x < CXPIX; x++)
		{
			float fL = (float)(x * m_iPixSize);
			float fR = (float)((x + 1) * m_iPixSize);

			m_bOnCur[x][y] = false;
			m_bOnTar[x][y] = false;
			m_colorCur[x][y] = WHITE;
			m_colorTar[x][y] = WHITE;
			m_fOffsetCur[x][y] = 0.0f;
			m_fOffsetTar[x][y] = 0.0f;

			RHWVERTEX *pVertices;
			m_pVertexPix->Lock(4 * (CXPIX * y + x) * sizeof(RHWVERTEX), 0, (void**)&pVertices, 0);

			pVertices[0] = { fL, fBtm, 0.0f, 1.0f, m_colorCur[x][y], 0.0f,	1.0f };
			pVertices[1] = { fL, fTop, 0.0f, 1.0f, m_colorCur[x][y], 0.0f,	0.0f };
			pVertices[2] = { fR, fBtm, 0.0f, 1.0f, m_colorCur[x][y], 1.0f,	1.0f };
			pVertices[3] = { fR, fTop, 0.0f, 1.0f, m_colorCur[x][y], 1.0f,	0.0f };

			m_pVertexPix->Unlock();
		}
	}

	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(RHWVERTEX), 0, RHW_FVF, D3DPOOL_MANAGED, &m_pVertexCur, NULL);

	{
		float fTop = 0.0f;
		float fBtm = (float)(CYPIX * m_iPixSize);
		float fL = 0.0f;
		float fR = (float)(CXPIX * m_iPixSize);
		D3DXCOLOR color = BLACK;
		color.a = 0.5f;

		RHWVERTEX *pVertices;
		m_pVertexCur->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = { fL, fBtm, 0.0f, 1.0f, color, 0.0f,	1.0f };
		pVertices[1] = { fL, fTop, 0.0f, 1.0f, color, 0.0f,	0.0f };
		pVertices[2] = { fR, fBtm, 0.0f, 1.0f, color, 1.0f,	1.0f };
		pVertices[3] = { fR, fTop, 0.0f, 1.0f, color, 1.0f,	0.0f };

		m_pVertexCur->Unlock();
	}

	m_fRefPeriod = 1.0f;

	m_RefMode = IMM;
	m_bRefOver = true;
	m_xRefBeg = 0;
	m_xRefEnd = 0;
	m_yRefBeg = 0;
	m_yRefEnd = 0;
	m_iRefStep = 0;
	m_RefTimer = 0.0f;

	m_bSuspend = false;
	m_RefModeSuspend = IMM;
	m_xRefBegSuspend = 0;
	m_xRefEndSuspend = 0;
	m_yRefBegSuspend = 0;
	m_yRefEndSuspend = 0;
	m_iRefStepSuspend = 0;	

	return TRUE;
}

BOOL GameScrn::End()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertexPix);
	m_pd3dDevice = NULL;

	return TRUE;
}

BOOL GameScrn::OnLostDevice()
{
	return TRUE;
}

BOOL GameScrn::OnResetDevice()
{
	D3DXCreateTextureFromResource(m_pd3dDevice, NULL, MAKEINTRESOURCE(IDR_TEX1 + GetSettings()._iPixSize - 1), &m_pTexture);

	m_iPixSize = GetSettings()._iPixSize;

	for (int y = 0; y < CYPIX; y++)
	{
		float fTop = (float)(y * m_iPixSize);
		float fBtm = (float)((y + 1) * m_iPixSize);

		for (int x = 0; x < CXPIX; x++)
		{
			float fL = (float)(x * m_iPixSize);
			float fR = (float)((x + 1) * m_iPixSize);

			RHWVERTEX *pVertices;
			m_pVertexPix->Lock(4 * (CXPIX * y + x) * sizeof(RHWVERTEX), 0, (void**)&pVertices, 0);

			pVertices[0].x = fL;
			pVertices[0].y = fBtm;

			pVertices[1].x = fL;
			pVertices[1].y = fTop;

			pVertices[2].x = fR;
			pVertices[2].y = fBtm;

			pVertices[3].x = fR;
			pVertices[3].y = fTop;

			m_pVertexPix->Unlock();
		}
	}

	{
		float fTop = 0.0f;
		float fBtm = (float)(CYPIX * m_iPixSize);
		float fL = 0.0f;
		float fR = (float)(CXPIX * m_iPixSize);
		D3DXCOLOR color = BLACK;
		color.a = 0.5f;

		RHWVERTEX *pVertices;
		m_pVertexCur->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0].x = fL;
		pVertices[0].y = fBtm;

		pVertices[1].x = fL;
		pVertices[1].y = fTop;

		pVertices[2].x = fR;
		pVertices[2].y = fBtm;

		pVertices[3].x = fR;
		pVertices[3].y = fTop;

		m_pVertexCur->Unlock();
	}

	return TRUE;
}

BOOL GameScrn::Update(double fTime, float fElapsedTime)
{
	if (!m_bRefOver)
	{
		switch (m_RefMode)
		{
		case IMM:
			for (int y = m_yRefBeg; y < m_yRefEnd; y++)
			{
				for (int x = m_xRefBeg; x < m_xRefEnd; x++)
				{
					int index = 4 * (CXPIX * y + x);

					m_bOnCur[x][y] = m_bOnTar[x][y];

					if (m_colorCur[x][y] != m_colorTar[x][y])
					{
						m_colorCur[x][y] = m_colorTar[x][y];

						RHWVERTEX *pVertices;
						m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

						pVertices[index + 0].color = m_colorTar[x][y];
						pVertices[index + 1].color = m_colorTar[x][y];
						pVertices[index + 2].color = m_colorTar[x][y];
						pVertices[index + 3].color = m_colorTar[x][y];

						m_pVertexPix->Unlock();
					}
					
					if (m_fOffsetCur[x][y] != m_fOffsetTar[x][y])
					{
						m_fOffsetCur[x][y] = m_fOffsetTar[x][y];

						float fTop = (float)(y * m_iPixSize);
						float fBtm = (float)((y + 1) * m_iPixSize);

						RHWVERTEX *pVertices;
						m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

						pVertices[index + 0].y = fBtm + m_fOffsetTar[x][y];
						pVertices[index + 1].y = fTop + m_fOffsetTar[x][y];
						pVertices[index + 2].y = fBtm + m_fOffsetTar[x][y];
						pVertices[index + 3].y = fTop + m_fOffsetTar[x][y];

						m_pVertexPix->Unlock();
					}
				}
			}

			if (m_bSuspend)
			{
				m_bSuspend = false;
				m_RefMode = m_RefModeSuspend;
				m_xRefBeg = m_xRefBegSuspend;
				m_xRefEnd = m_xRefEndSuspend;
				m_yRefBeg = m_yRefBegSuspend;
				m_yRefEnd = m_yRefEndSuspend;
				m_iRefStep = m_iRefStepSuspend;
			}
			else
				m_bRefOver = true;

			break;

		case BRUSH:
			m_RefTimer += fElapsedTime;
			while (m_RefTimer >= m_fRefPeriod)
			{
				m_RefTimer -= m_fRefPeriod;

				for (int y = m_yRefBeg; y < m_yRefEnd; y++)
				{
					int x = m_xRefBeg + m_yRefBeg + m_iRefStep - y;
					int index = 4 * (CXPIX * y + x);

					if (x >= m_xRefBeg && x < m_xRefEnd)
					{
						m_bOnCur[x][y] = m_bOnTar[x][y];

						if (m_colorCur[x][y] != m_colorTar[x][y])
						{
							m_colorCur[x][y] = m_colorTar[x][y];

							RHWVERTEX *pVertices;
							m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

							pVertices[index + 0].color = m_colorTar[x][y];
							pVertices[index + 1].color = m_colorTar[x][y];
							pVertices[index + 2].color = m_colorTar[x][y];
							pVertices[index + 3].color = m_colorTar[x][y];

							m_pVertexPix->Unlock();
						}

						if (m_fOffsetCur[x][y] != m_fOffsetTar[x][y])
						{
							m_fOffsetCur[x][y] = m_fOffsetTar[x][y];

							float fTop = (float)(y * m_iPixSize);
							float fBtm = (float)((y + 1) * m_iPixSize);

							RHWVERTEX *pVertices;
							m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

							pVertices[index + 0].y = fBtm + m_fOffsetTar[x][y];
							pVertices[index + 1].y = fTop + m_fOffsetTar[x][y];
							pVertices[index + 2].y = fBtm + m_fOffsetTar[x][y];
							pVertices[index + 3].y = fTop + m_fOffsetTar[x][y];

							m_pVertexPix->Unlock();
						}
					}
				}

				m_iRefStep++;
			}
			if (m_iRefStep >= m_xRefEnd + m_yRefEnd - m_xRefBeg - m_yRefBeg - 1)
				m_bRefOver = true;

			break;

		case LINE:
			m_RefTimer += fElapsedTime;
			while (m_RefTimer >= m_fRefPeriod)
			{
				m_RefTimer -= m_fRefPeriod;

				int y = m_yRefBeg + m_iRefStep / (m_xRefEnd - m_xRefBeg + 1);
				int x = m_xRefBeg + m_iRefStep % (m_xRefEnd - m_xRefBeg + 1);
				int index = 4 * (CXPIX * y + x);

				m_bOnCur[x][y] = m_bOnTar[x][y];

				if (m_colorCur[x][y] != m_colorTar[x][y])
				{
					m_colorCur[x][y] = m_colorTar[x][y];

					RHWVERTEX *pVertices;
					m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[index + 0].color = m_colorTar[x][y];
					pVertices[index + 1].color = m_colorTar[x][y];
					pVertices[index + 2].color = m_colorTar[x][y];
					pVertices[index + 3].color = m_colorTar[x][y];

					m_pVertexPix->Unlock();
				}

				if (m_fOffsetCur[x][y] != m_fOffsetTar[x][y])
				{
					m_fOffsetCur[x][y] = m_fOffsetTar[x][y];

					float fTop = (float)(y * m_iPixSize);
					float fBtm = (float)((y + 1) * m_iPixSize);

					RHWVERTEX *pVertices;
					m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[index + 0].y = fBtm + m_fOffsetTar[x][y];
					pVertices[index + 1].y = fTop + m_fOffsetTar[x][y];
					pVertices[index + 2].y = fBtm + m_fOffsetTar[x][y];
					pVertices[index + 3].y = fTop + m_fOffsetTar[x][y];

					m_pVertexPix->Unlock();
				}

				m_iRefStep++;
			}
			if (m_iRefStep == (m_xRefEnd - m_xRefBeg) * (m_yRefEnd - m_yRefBeg))
				m_bRefOver = true;

			break;

		case SHRINK:
			m_RefTimer += fElapsedTime;
			while (m_RefTimer >= 10.0f * m_fRefPeriod)
			{
				m_RefTimer -= 10.0f * m_fRefPeriod;

				int xCenter = (m_xRefBeg + m_xRefEnd) / 2;
				int yCenter = (m_yRefBeg + m_yRefEnd) / 2;
				int iWidth = (m_xRefEnd - m_xRefBeg) / 2;

				for (int i = 0; i < iWidth - m_iRefStep; i++)
				{
					int x = xCenter - i - 1;
					int y = yCenter - i - 1;

					do
					{
						if (i == iWidth - m_iRefStep - 1)
						{
							int index = 4 * (CXPIX * y + x);

							m_bOnCur[x][y] = m_bOnTar[x][y];

							if (m_colorCur[x][y] != m_colorTar[x][y])
							{
								m_colorCur[x][y] = m_colorTar[x][y];

								RHWVERTEX *pVertices;
								m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

								pVertices[index + 0].color = m_colorTar[x][y];
								pVertices[index + 1].color = m_colorTar[x][y];
								pVertices[index + 2].color = m_colorTar[x][y];
								pVertices[index + 3].color = m_colorTar[x][y];

								m_pVertexPix->Unlock();
							}

							if (m_fOffsetCur[x][y] != m_fOffsetTar[x][y])
							{
								m_fOffsetCur[x][y] = m_fOffsetTar[x][y];

								float fTop = (float)(y * m_iPixSize);
								float fBtm = (float)((y + 1) * m_iPixSize);

								RHWVERTEX *pVertices;
								m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

								pVertices[index + 0].y = fBtm + m_fOffsetTar[x][y];
								pVertices[index + 1].y = fTop + m_fOffsetTar[x][y];
								pVertices[index + 2].y = fBtm + m_fOffsetTar[x][y];
								pVertices[index + 3].y = fTop + m_fOffsetTar[x][y];

								m_pVertexPix->Unlock();
							}
						}
						else
						{
							int xDelta = x < xCenter ? -1 : 1;
							int yDelta = y < yCenter ? -1 : 1;

							m_bOnCur[x][y] = m_bOnCur[x + xDelta][y + yDelta];

							int index = 4 * (CXPIX * y + x);

							if (m_colorCur[x][y] != m_colorCur[x + xDelta][y + yDelta])
							{
								m_colorCur[x][y] = m_colorCur[x + xDelta][y + yDelta];

								RHWVERTEX *pVertices;
								m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

								pVertices[index + 0].color = m_colorCur[x][y];
								pVertices[index + 1].color = m_colorCur[x][y];
								pVertices[index + 2].color = m_colorCur[x][y];
								pVertices[index + 3].color = m_colorCur[x][y];

								m_pVertexPix->Unlock();
							}

							if (m_fOffsetCur[x][y] != m_fOffsetTar[x + xDelta][y + yDelta])
							{
								m_fOffsetCur[x][y] = m_fOffsetTar[x + xDelta][y + yDelta];

								float fTop = (float)(y * m_iPixSize);
								float fBtm = (float)((y + 1) * m_iPixSize);

								RHWVERTEX *pVertices;
								m_pVertexPix->Lock(0, 0, (void**)&pVertices, 0);

								pVertices[index + 0].y = fBtm + m_fOffsetCur[x][y];
								pVertices[index + 1].y = fTop + m_fOffsetCur[x][y];
								pVertices[index + 2].y = fBtm + m_fOffsetCur[x][y];
								pVertices[index + 3].y = fTop + m_fOffsetCur[x][y];

								m_pVertexPix->Unlock();
							}
						}

						if (x != xCenter + i && y == yCenter - i - 1)
							x++;
						else if (x == xCenter + i && y != yCenter + i)
							y++;
						else if (x != xCenter - i - 1 && y == yCenter + i)
							x--;
						else if (x == xCenter - i - 1 && y != yCenter - i - 1)
							y--;
					} while (x != xCenter - i - 1 || y != yCenter - i - 1);
				}

				m_iRefStep++;
				if (m_iRefStep == iWidth)
					m_bRefOver = true;
			}

			break;

		default:
			break;
		}

		return TRUE;
	}
	else
		return FALSE;
}

BOOL GameScrn::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetStreamSource(0, m_pVertexPix, 0, sizeof(RHWVERTEX));
	m_pd3dDevice->SetFVF(RHW_FVF);

	if (GetSettings()._iTexture != 0)
		m_pd3dDevice->SetTexture(0, m_pTexture);

	for (int y = 0; y < CYPIX; y++)
	{
		for (int x = 0; x < CXPIX; x++)
		{
			if (m_bOnCur[x][y])
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (CXPIX * y + x), 2);
		}
	}

	m_pd3dDevice->SetTexture(0, NULL);

	if (GAME_PAUSE == GetMode())
	{
		m_pd3dDevice->SetStreamSource(0, m_pVertexCur, 0, sizeof(RHWVERTEX));
		m_pd3dDevice->SetFVF(RHW_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

void GameScrn::Refresh(REFMODE RefMode, RECT *rcBrush)
{
	if (!m_bRefOver && RefMode == IMM)
	{
		m_bSuspend = true;
		m_RefModeSuspend = m_RefMode;
		m_xRefBegSuspend = m_xRefBeg;
		m_xRefEndSuspend = m_xRefEnd;
		m_yRefBegSuspend = m_yRefBeg;
		m_yRefEndSuspend = m_yRefEnd;
		m_iRefStepSuspend = m_iRefStep;
	}

	m_RefMode = RefMode;
	m_bRefOver = false;
	m_xRefBeg = rcBrush ? rcBrush->left : 0;
	m_xRefEnd = rcBrush ? rcBrush->right : CXPIX;
	m_yRefBeg = rcBrush ? rcBrush->top : 0;
	m_yRefEnd = rcBrush ? rcBrush->bottom : CYPIX;
	m_iRefStep = 0;

	if (RefMode != IMM)
		m_RefTimer = 0.0f;
}

void GameScrn::Refresh(REFMODE RefMode, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!m_bRefOver && RefMode == IMM)
	{
		m_bSuspend = true;
		m_RefModeSuspend = m_RefMode;
		m_xRefBegSuspend = m_xRefBeg;
		m_xRefEndSuspend = m_xRefEnd;
		m_yRefBegSuspend = m_yRefBeg;
		m_yRefEndSuspend = m_yRefEnd;
		m_iRefStepSuspend = m_iRefStep;
	}

	m_RefMode = RefMode;
	m_bRefOver = false;
	m_xRefBeg = max(xBeg, 0);
	m_xRefEnd = min(xEnd, CXPIX);
	m_yRefBeg = max(yBeg, 0);
	m_yRefEnd = min(yEnd, CYPIX);
	m_iRefStep = 0;

	if (RefMode != IMM)
		m_RefTimer = 0.0f;
}

void GameScrn::SetRefPeriod(float fPeriod)
{
	m_fRefPeriod = fPeriod;
}

bool GameScrn::IsRefOver()
{
	return m_bRefOver;
}

void GameScrn::Set(bool bOn, D3DXCOLOR color, int x, int y)
{
	if (x < 0 || x >= CXPIX || y < 0 || y >= CYPIX)
		return;

	m_bOnTar[x][y] = bOn;
	m_colorTar[x][y] = color;
}

void GameScrn::Set(bool bOn, D3DXCOLOR color, std::vector<PIX> *vecPix)
{
	auto iter = vecPix->begin();
	while (iter != vecPix->end())
	{
		int x = iter->_x;
		int y = iter->_y;
		Set(bOn, color, x, y);
		iter++;
	}
}

void GameScrn::Set(bool bOn, D3DXCOLOR color, RECT *rcPix)
{
	int xBeg = rcPix ? rcPix->left : 0;
	int xEnd = rcPix ? rcPix->right : CXPIX;
	int yBeg = rcPix ? rcPix->top : 0;
	int yEnd = rcPix ? rcPix->bottom : CYPIX;
	
	for (int x = xBeg; x < xEnd; x++)
	{
		for (int y = yBeg; y < yEnd; y++)
		{
			Set(bOn, color, x, y);
		}
	}
}

void GameScrn::SetFrame(bool bOn, D3DXCOLOR color, RECT *rcPix)
{
	int xBeg = rcPix ? rcPix->left : 0;
	int xEnd = rcPix ? rcPix->right : CXPIX;
	int yBeg = rcPix ? rcPix->top : 0;
	int yEnd = rcPix ? rcPix->bottom : CYPIX;

	for (int x = xBeg; x < xEnd; x++)
	{
		Set(bOn, color, x, yBeg);
		Set(bOn, color, x, yEnd - 1);
	}

	for (int y = yBeg; y < yEnd; y++)
	{
		Set(bOn, color, xBeg, y);
		Set(bOn, color, xEnd - 1, y);
	}
}

void GameScrn::Reverse(RECT *rcPix)
{
	for (int x = rcPix->left; x < rcPix->right; x++)
	{
		for (int y = rcPix->top; y < rcPix->bottom; y++)
		{
			m_bOnTar[x][y] = !m_bOnTar[x][y];
		}
	}
}

GameScrn *g_pGameScrn = new GameScrn();

GameScrn &GetScrn()
{
	assert(g_pGameScrn != NULL);
	return *g_pGameScrn;
}