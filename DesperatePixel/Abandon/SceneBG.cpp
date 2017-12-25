#include "SceneBG.h"

//
//Grid
//

BGGrid::BGGrid()
{
	Grid::Grid();
	int nSpacing = 8;
	int nRows = 2 * (int)((DXUTGetDeviceHeight() / nSpacing + 1) / 2);
	int nCols = 2 * (int)((DXUTGetDeviceWidth() / nSpacing + 1) / 2);
	int x = -nCols * nSpacing / 2;
	int y = -nRows * nSpacing / 2;
	Create(nRows, nCols, nSpacing, x, y);
	m_fUpdateTime = 880.0f;
	m_R1 = 99;
	m_G1 = 99;
	m_B1 = 99;
	m_R2 = 223;
	m_G2 = 223;
	m_B2 = 223;
}

BGGrid::~BGGrid()
{
	Grid::~Grid();
}

void BGGrid::Update(double fTime, float fElapsedTime)
{
	m_fUpdateTime += fElapsedTime;
	float ratio = abs(m_fUpdateTime - 2000.0f) / 2000.0f;
	int R = (int)(ratio * m_R1 + (1.0f - ratio) * m_R2);
	int G = (int)(ratio * m_G1 + (1.0f - ratio) * m_G2);
	int B = (int)(ratio * m_B1 + (1.0f - ratio) * m_B2);
	m_FrameColor = D3DCOLOR_ARGB(255, R, G, B);

	COLORVERTEX *pVertices;
		m_pFrameVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 2 * (m_nRows + m_nCols + 2); i++)
		pVertices[i].color = m_FrameColor;

	m_pFrameVertex->Unlock();

	if (m_fUpdateTime >= 4000.0f) m_fUpdateTime -= 4000.0f;
}

//
//Blocks
//

SeedBlocks::SeedBlocks()
{
	Blocks::Blocks();
	m_cSeed = 8;
	m_fUpdateTime = 1000.0f;

	for (int i = 0; i < m_cSeed; i++)
	{
		m_vecLSeed.push_back(new Seed(0,0));
		m_vecRSeed.push_back(new Seed(0,0));
		m_vecUSeed.push_back(new Seed(0,0));
		m_vecDSeed.push_back(new Seed(0,0));
	}
}

SeedBlocks::~SeedBlocks()
{
	Blocks::~Blocks();
}

void SeedBlocks::Init()
{
	int nRows = m_pGrid->GetRowCount();
	int nCols = m_pGrid->GetColCount();

	for (int i = 0; i < m_cSeed; i++)
	{
		m_vecLSeed[i]->row = rand() % (nRows / 2);
		m_vecLSeed[i]->col = i * nCols / m_cSeed;
		m_vecRSeed[i]->row = rand() % (nRows / 2) + nRows / 2;
		m_vecRSeed[i]->col = i * nCols / m_cSeed;
		m_vecUSeed[i]->row = i * nRows / m_cSeed;
		m_vecUSeed[i]->col = rand() % (nCols / 2) + nCols / 2;
		m_vecDSeed[i]->row = i * nRows / m_cSeed;
		m_vecDSeed[i]->col = rand() % (nCols / 2);
	}
}

void SeedBlocks::Update(double fTime, float fElapsedTime)
{
	m_fUpdateTime += fElapsedTime;

	int nRows = m_pGrid->GetRowCount();
	int nCols = m_pGrid->GetColCount();

	if (m_fUpdateTime>250.0f)
	{
		for (int i = 0; i < m_cSeed; i++)
		{
			m_vecLSeed[i]->col++;
			if (m_vecLSeed[i]->col > nCols + 2)
			{
				m_vecLSeed[i]->col = 0;
				m_vecLSeed[i]->row = rand() % (nRows / 2);
			}

			m_vecRSeed[i]->col--;
			if (m_vecRSeed[i]->col < -2)
			{
				m_vecRSeed[i]->col = nCols;
				m_vecRSeed[i]->row = rand() % (nRows / 2) + nRows / 2;
			}

			m_vecUSeed[i]->row++;
			if (m_vecUSeed[i]->row > nRows + 2)
			{
				m_vecUSeed[i]->row = 0;
				m_vecUSeed[i]->col = rand() % (nCols / 2) + nCols / 2;
			}

			m_vecDSeed[i]->row--;
			if (m_vecDSeed[i]->row < -2)
			{
				m_vecDSeed[i]->row = nRows;
				m_vecDSeed[i]->col = rand() % (nCols / 2);
			}
		}

		std::vector<BlockData>().swap(m_vecBlock);
		for (int i = 0; i < m_cSeed; i++)
		{
			m_vecBlock.push_back(BlockData(m_vecLSeed[i]->row    , m_vecLSeed[i]->col - 2, D3DCOLOR_ARGB(31,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecRSeed[i]->row    , m_vecRSeed[i]->col + 2, D3DCOLOR_ARGB(31,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecUSeed[i]->row - 2, m_vecUSeed[i]->col    , D3DCOLOR_ARGB(31,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecDSeed[i]->row + 2, m_vecDSeed[i]->col    , D3DCOLOR_ARGB(31,0,0,0), false));
		}
		for (int i = 0; i < m_cSeed; i++)
		{
			m_vecBlock.push_back(BlockData(m_vecLSeed[i]->row    , m_vecLSeed[i]->col - 1, D3DCOLOR_ARGB(63,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecRSeed[i]->row    , m_vecRSeed[i]->col + 1, D3DCOLOR_ARGB(63,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecUSeed[i]->row - 1, m_vecUSeed[i]->col    , D3DCOLOR_ARGB(63,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecDSeed[i]->row + 1, m_vecDSeed[i]->col    , D3DCOLOR_ARGB(63,0,0,0), false));
		}
		for (int i = 0; i < m_cSeed; i++)
		{
			m_vecBlock.push_back(BlockData(m_vecLSeed[i]->row    , m_vecLSeed[i]->col    , D3DCOLOR_ARGB(95,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecRSeed[i]->row    , m_vecRSeed[i]->col    , D3DCOLOR_ARGB(95,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecUSeed[i]->row    , m_vecUSeed[i]->col    , D3DCOLOR_ARGB(95,0,0,0), false));
			m_vecBlock.push_back(BlockData(m_vecDSeed[i]->row    , m_vecDSeed[i]->col    , D3DCOLOR_ARGB(95,0,0,0), false));
		}

		m_fUpdateTime -= 250.0f;
	}
}

//
//Scene
//

SceneBG::SceneBG()
{
	pBGGrid = NULL;
	pSeedBlocks = NULL;
}

SceneBG::~SceneBG()
{
}

void SceneBG::Init()
{
	pBGGrid = new BGGrid();
	pSeedBlocks = new SeedBlocks();
	pSeedBlocks->SetGrid(pBGGrid);
	pSeedBlocks->Init();
}

void SceneBG::Update(double fTime, float fElapsedTime)
{
	pBGGrid->Update(fTime, fElapsedTime);
	pSeedBlocks->Update(fTime, fElapsedTime);
}

void SceneBG::Render(double fTime, float fElapsedTime)
{
	pBGGrid->Render(fTime, fElapsedTime);
}

DWORD SceneBG::GetFrameColor()
{
	return pBGGrid->GetFrameColor();
}
