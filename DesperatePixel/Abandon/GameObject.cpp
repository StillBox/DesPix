#include "GameObject.h"

//
//Backgroud
//

void Background::TexMove(float uDist, float vDist)
{
	static float uOffset = 0.0f, vOffset = 0.0f;
	TEXVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	for (int i = 0; i < 4; i++)
	{
		uOffset += uDist;
		vOffset += vDist;
		pVertices[i].u += uDist;
		pVertices[i].v += vDist;
		if (uOffset >= 1.0f) { uOffset -= 1.0f; pVertices[i].u -= 1.0f; }
		if (vOffset >= 1.0f) { vOffset -= 1.0f; pVertices[i].v -= 1.0f; }
	}
	m_pVertexBuffer->Unlock();
}

//
//Grid
//

Grid::Grid()
{
	m_pd3dDevice	= DXUTGetD3DDevice();
	m_pFrameVertex	= NULL;
	m_pBlockVertex	= NULL;
	m_bFrame		= true;
	m_x				= 0;
	m_y				= 0;
	m_nRows			= 0;
	m_nCols			= 0;
	m_nSpacing		= 0;
	m_fWidth		= 0.0f;
	m_fHeight		= 0.0f;
}

Grid::~Grid()
{
	SAFE_RELEASE(m_pFrameVertex);
	SAFE_RELEASE(m_pBlockVertex);
}

void Grid::Create(int nRows, int nCols, int nSpacing, int x, int y)
{
	m_x			= x;
	m_y			= y;
	m_nRows		= nRows;
	m_nCols		= nCols;
	m_nSpacing	= nSpacing;
	m_fWidth	= (float)(m_nCols * m_nSpacing);
	m_fHeight	= (float)(m_nRows * m_nSpacing);

	SAFE_RELEASE(m_pFrameVertex);
	m_pd3dDevice->CreateVertexBuffer(2 * (m_nRows + m_nCols + 2) * sizeof(COLORVERTEX),
		0, COLOR_FVF, D3DPOOL_MANAGED, &m_pFrameVertex, NULL);

	SAFE_RELEASE(m_pBlockVertex);
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(COLORTEXVERTEX),
		0, COLORTEX_FVF, D3DPOOL_MANAGED, &m_pBlockVertex, NULL);

	COLORVERTEX *pVertices;
	m_pFrameVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i <= m_nRows; i++)
	{
		int index			 = 2 * i;
		float y				 = (float)(i * m_nSpacing);
		pVertices[index]     = { 0.0f    , y, 0.0f, BLACK };
		pVertices[index + 1] = { m_fWidth, y, 0.0f, BLACK };
	}
	for (int i = 0; i <= m_nCols; i++)
	{
		int index			 = 2 * (m_nRows + 1 + i);
		float x				 = (float)(i * m_nSpacing);
		pVertices[index]     = { x, 0.0f     , 0.0f, BLACK };
		pVertices[index + 1] = { x, m_fHeight, 0.0f, BLACK };
	}

	m_pFrameVertex->Unlock();
}

int Grid::GetRowCount()
{
	return m_nRows;
}

int Grid::GetColCount()
{
	return m_nCols;
}

void Grid::AddBlocks(Blocks *pBlocks)
{
	m_vecBlocks.push_back(pBlocks);
}

void Grid::DelBlocks(Blocks *pBlocks)
{
	auto iter = find(m_vecBlocks.begin(), m_vecBlocks.end(), pBlocks);
	m_vecBlocks.erase(iter);
}

void Grid::RenderFrame()
{
	m_pd3dDevice->SetStreamSource(0, m_pFrameVertex, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nRows + m_nCols + 2);
}

void Grid::RenderBlock(int row, int col, DWORD color, IDirect3DTexture9 *pTexture)
{
	float left		= col * (float)m_nSpacing + (m_bFrame ? 1.0f : 0.0f);
	float top		= row * (float)m_nSpacing + (m_bFrame ? 1.0f : 0.0f);
	float right		= (col + 1) * (float)m_nSpacing;
	float bottom	= (row + 1) * (float)m_nSpacing;

	COLORTEXVERTEX *pVertices;
	m_pBlockVertex->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = { left , bottom, 0.0f, color, 0.0f, 1.0f };
	pVertices[1] = { left , top	  , 0.0f, color, 0.0f, 0.0f };
	pVertices[2] = { right, bottom, 0.0f, color, 1.0f, 1.0f };
	pVertices[3] = { right, top   , 0.0f, color, 1.0f, 0.0f };
	m_pBlockVertex->Unlock();

	m_pd3dDevice->SetStreamSource(0, m_pBlockVertex, 0, sizeof(COLORTEXVERTEX));
	m_pd3dDevice->SetFVF(COLORTEX_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (NULL != pTexture)
	{
		m_pd3dDevice->SetTexture(0, pTexture);
		m_pd3dDevice->SetStreamSource(0, m_pBlockVertex, 0, sizeof(COLORTEXVERTEX));
		m_pd3dDevice->SetFVF(COLORTEX_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		m_pd3dDevice->SetTexture(0, NULL);
	}
}

void Grid::Update(double fTime, float fElapsedTime)
{
	for (auto p : m_vecBlocks)
	{
		p->Update(fTime, fElapsedTime);
	}
}

void Grid::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, (float)m_x, (float)m_y, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	RenderFrame();

	for (auto p : m_vecBlocks)
	{
		p->Render(fTime, fElapsedTime);
	}

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//
//Blocks
//

Blocks::Blocks()
{
	m_pGrid = NULL;
	m_pTexture = NULL;
}

Blocks::~Blocks()
{
	std::vector<BlockData>().swap(m_vecBlock);
	SAFE_RELEASE(m_pTexture);
}

void Blocks::SetGrid(Grid *pGrid)
{
	m_pGrid = pGrid;
	pGrid->AddBlocks(this);
}

void Blocks::SetTexture(WCHAR *szFile)
{
	D3DXCreateTextureFromFile(DXUTGetD3DDevice(), szFile, &m_pTexture);
}

void Blocks::AddBlock(int row, int col, DWORD color)
{
	BlockData newBlock = BlockData(row, col, color);
	m_vecBlock.push_back(newBlock);
}

void Blocks::DelBlock(int row, int col)
{
	auto iter = m_vecBlock.begin();
	while (iter != m_vecBlock.end())
	{
		if (row == iter->row && col == iter->col)
			iter = m_vecBlock.erase(iter);
		else
			iter++;
	}
}

void Blocks::AddBlock(Blocks *pBlocks)
{
	for (auto data : pBlocks->m_vecBlock)
	{
		m_vecBlock.push_back(data);
	}
}

BlockData* Blocks::FindBlock(int row, int col)
{
	for (auto data : m_vecBlock)
	{
		if (row == data.row && col == data.col)
			return &data;
	}
	return NULL;
}

void Blocks::Update(double fTime, float fElapsedTime)
{

}

void Blocks::Render(double fTime, float fElapsedTime)
{
	for (auto data : m_vecBlock)
	{
		m_pGrid->RenderBlock(data.row, data.col, data.color, data.bDrawTex ? m_pTexture : NULL);
	}
}

//
//Curtain
//

Curtain::Curtain()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_Range = RECTRANGE(0.0f, 0.0f, (float)DXUTGetDeviceWidth(), (float)DXUTGetDeviceHeight());
	m_IsOn = FALSE;
	m_NextMode = MAIN_MENU;
}

Curtain::Curtain(float left, float top, float right, float bottom)
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_Range = RECTRANGE(left, top, right, bottom);
	m_IsOn = FALSE;
	m_NextMode = MAIN_MENU;
}

Curtain::~Curtain()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void Curtain::Set(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)())
{
	m_IsOn = TRUE;
	m_AlphaBeg = AlphaBeg;
	m_AlphaEnd = AlphaEnd;
	m_fDuration = fDuration;
	m_fPassTime = 0.0f;
	m_pOnCurtainEnd = pFunc;
}

void Curtain::RenderAlpha(int Alpha)
{
	if (NULL == m_pVertexBuffer)
		m_pd3dDevice->CreateVertexBuffer(4 * sizeof(POINTVERTRHWEXCOLOR), 0, POINTVERTRHWEXCOLOR::FVF,
			D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	POINTVERTRHWEXCOLOR *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { m_Range.left,	 m_Range.bottom, 0.0f, 0.0f, D3DCOLOR_ARGB(Alpha,0,0,0) };
	pVertices[1] = { m_Range.left,	 m_Range.top,	 0.0f, 0.0f, D3DCOLOR_ARGB(Alpha,0,0,0) };
	pVertices[2] = { m_Range.right,	 m_Range.bottom, 0.0f, 0.0f, D3DCOLOR_ARGB(Alpha,0,0,0) };
	pVertices[3] = { m_Range.right,	 m_Range.top,	 0.0f, 0.0f, D3DCOLOR_ARGB(Alpha,0,0,0) };

	m_pVertexBuffer->Unlock();

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(POINTVERTRHWEXCOLOR));
	m_pd3dDevice->SetFVF(POINTVERTRHWEXCOLOR::FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Curtain::Render(float fElapsedTime)
{
	m_fPassTime += fElapsedTime;
	int Alpha = m_AlphaBeg + (int)((float)(m_AlphaEnd - m_AlphaBeg)*m_fPassTime / m_fDuration);
	RenderAlpha(Alpha);
	if (m_fPassTime >= m_fDuration)
	{
		m_IsOn = FALSE;
		if (NULL != m_pOnCurtainEnd) m_pOnCurtainEnd();
	}
}