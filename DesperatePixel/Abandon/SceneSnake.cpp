#include "SceneSnake.h"
#include "DXInput.h"

SnakeGrid	*g_pGrid = NULL;
Snake		*g_pSnake = NULL;
Food		*g_pFood = NULL;

//
//Grid
//

SnakeGrid::SnakeGrid()
{
	Grid::Grid();
	m_pBGVertex = NULL;
	m_color1 = D3DCOLOR_ARGB(255, 120, 120, 150);
	m_color2 = D3DCOLOR_ARGB(255, 100, 120, 180);
}

SnakeGrid::~SnakeGrid()
{
	SAFE_RELEASE(m_pBGVertex);
	Grid::~Grid();
}

void SnakeGrid::Create(int nRows, int nCols, int nSpacing, int x, int y)
{
	Grid::Create(nRows, nCols, nSpacing, x, y);

	SAFE_RELEASE(m_pBGVertex);
	m_pd3dDevice->CreateVertexBuffer(6 * m_nRows * m_nCols * sizeof(COLORVERTEX),
		0, COLOR_FVF, D3DPOOL_MANAGED, &m_pBGVertex, NULL);

	COLORVERTEX *pVertices;
	m_pBGVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < m_nRows; i++)
	{
		for (int j = 0; j < m_nCols; j++)
		{
			int index = 6*(i*m_nCols + j);
			float left		= (float)(j * m_nSpacing);
			float right		= (float)((j + 1) * m_nSpacing);
			float top		= (float)(i * m_nSpacing);
			float bottom	= (float)((i + 1) * m_nSpacing);
			DWORD color		= ((i + j) % 2) ? m_color1 : m_color2;
			pVertices[index]   = { left,  bottom, 0.0f, color };
			pVertices[index+1] = { left,  top   , 0.0f, color };
			pVertices[index+2] = { right, top   , 0.0f, color };
			pVertices[index+3] = { left,  bottom, 0.0f, color };
			pVertices[index+4] = { right, top   , 0.0f, color };
			pVertices[index+5] = { right, bottom, 0.0f, color };
		}
	}
	m_pFrameVertex->Unlock();
}

void SnakeGrid::Update(double fTime, float fElapsedTime)
{
	COLORVERTEX *pVertices;
	m_pFrameVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 2 * (m_nRows + m_nCols + 2); i++)
		pVertices[i].color = GetBGFrameColor();

	m_pFrameVertex->Unlock();
}

void SnakeGrid::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, (float)m_x, (float)m_y, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	m_pd3dDevice->SetStreamSource(0, m_pBGVertex, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 * m_nRows * m_nCols);

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
//Snake
//

Snake::Snake()
{
	Blocks::Blocks();
	m_bDead = false;
	m_iVRow = 0;
	m_iVCol = 1;
	m_fUpdateTime = 0.0f;
	m_fUpdateGap = 800.0f;
}

Snake::~Snake()
{
	Blocks::~Blocks();
}

void Snake::Create()
{
	int row = g_pGrid->GetRowCount() / 2;
	int col = g_pGrid->GetColCount() / 2;
	m_vecBlock.push_back(BlockData(row, col - 2, WHITE, false));
	m_vecBlock.push_back(BlockData(row, col - 3, L_GRAY, false));
}

void Snake::SetBound(int left, int right, int top, int bottom)
{
	m_Bound[0] = left;
	m_Bound[1] = right;
	m_Bound[2] = top;
	m_Bound[3] = bottom;
}

void Snake::Update(double fTIme, float fElapsedTime)
{
	m_fUpdateTime += fElapsedTime;

	if (IsKeyDown(DIK_LEFT))
		if (0 == m_iVCol)
		{
			m_iVCol = -1;
			m_iVRow = 0;
			m_fUpdateTime = m_fUpdateGap;
		}

	if (IsKeyDown(DIK_RIGHT))
		if (0 == m_iVCol)
		{
			m_iVCol = 1;
			m_iVRow = 0;
			m_fUpdateTime = m_fUpdateGap;
		}

	if (IsKeyDown(DIK_UP))
		if (0 == m_iVRow)
		{
			m_iVRow = -1;
			m_iVCol = 0;
			m_fUpdateTime = m_fUpdateGap;
		}

	if (IsKeyDown(DIK_DOWN))
		if (0 == m_iVRow)
		{
			m_iVRow = 1;
			m_iVCol = 0;
			m_fUpdateTime = m_fUpdateGap;
		}

	float UpdateGap = m_fUpdateGap * (IsKeyDown(DIK_SPACE) ? 0.5f : 1.0f);
	if (m_fUpdateTime >= UpdateGap)
	{
		int newRow = m_vecBlock[0].row + m_iVRow;
		int newCol = m_vecBlock[0].col + m_iVCol;
		BlockData tail = m_vecBlock[m_vecBlock.size() - 1];

		if (newCol < m_Bound[0] || newCol > m_Bound[1] ||
			newRow < m_Bound[2] || newRow > m_Bound[3])
		{
			m_bDead = true;
		}
		else if (NULL != g_pSnake->FindBlock(newRow, newCol) &&
			(newCol != tail.col || newRow != tail.row))
		{
			m_bDead = true;
		}
		else
		{
			for (auto i = m_vecBlock.size() - 1; i > 0; i--)
			{
				m_vecBlock[i].row = m_vecBlock[i - 1].row;
				m_vecBlock[i].col = m_vecBlock[i - 1].col;
			}
			m_vecBlock[0].row = newRow;
			m_vecBlock[0].col = newCol;
			if (NULL != g_pFood->FindBlock(newRow, newCol))
			{
				g_pFood->DelBlock(newRow, newCol);
				m_vecBlock.push_back(tail);
			}
		}
		while (m_fUpdateTime >= UpdateGap)
			m_fUpdateTime -= UpdateGap;
	}
}

BOOL Snake::IsDead()
{
	return m_bDead;
}

//
//Food
//

void Food::Update(double fTime, float fElapsedTime)
{
	if (0 == m_vecBlock.size())
	{
		BlockData newBlock;
		do
		{
			newBlock.row = rand() % g_pGrid->GetRowCount();
			newBlock.col = rand() % g_pGrid->GetColCount();
		} while (NULL != g_pSnake->FindBlock(newBlock.row, newBlock.col));
		newBlock.color = BLACK;
		newBlock.bDrawTex = true;
		m_vecBlock.push_back(newBlock);
	}
}

//
//GameScene
//

SceneSnake::SceneSnake()
{
	g_pGrid = NULL;
	g_pSnake = NULL;
	g_pFood = NULL;
	m_iLevel = 0;
}

SceneSnake::~SceneSnake()
{
	SAFE_DELETE(g_pFood);
	SAFE_DELETE(g_pSnake);
	SAFE_DELETE(g_pGrid);
}

void SceneSnake::Init()
{
	g_pGrid = new SnakeGrid();
	g_pGrid->Create(9, 10, 64, -392, -288);

	g_pSnake = new Snake();
	g_pSnake->SetGrid(g_pGrid);
	g_pSnake->SetBound(0, g_pGrid->GetColCount() - 1, 0, g_pGrid->GetRowCount());
	g_pSnake->Create();

	g_pFood = new Food();
	g_pFood->SetGrid(g_pGrid);
}

void SceneSnake::Update(double fTime, float fElapsedTime)
{
	g_pGrid->Update(fTime, fElapsedTime);
	g_pSnake->Update(fTime, fElapsedTime);
	if (g_pSnake->IsDead())
		GameOver();
	g_pFood->Update(fTime, fElapsedTime);
}

void SceneSnake::Render(double fTime, float fElapsedTime)
{
	g_pGrid->Render(fTime, fElapsedTime);
}