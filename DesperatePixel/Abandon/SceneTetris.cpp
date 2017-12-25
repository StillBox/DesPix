#include "SceneTetris.h"
#include "DXInput.h"

//
//Global data for Tetris
//

float g_fUpdateGap = 200.0f;


//
//Grid
//

TetrisGrid::TetrisGrid()
{
	Grid::Grid();
	m_pBGVertex = NULL;
	m_color1 = D3DCOLOR_ARGB(255, 120, 120, 150);
	m_color2 = D3DCOLOR_ARGB(255, 100, 120, 180);
}

TetrisGrid::~TetrisGrid()
{
	SAFE_RELEASE(m_pBGVertex);
	Grid::~Grid();
}

void TetrisGrid::Create(int nRows, int nCols, int nSpacing, int x, int y)
{
	Grid::Create(nRows, nCols, nSpacing, x, y);

	SAFE_RELEASE(m_pBGVertex);
	m_pd3dDevice->CreateVertexBuffer(2 * (m_nCols + 1) * sizeof(COLORVERTEX),
		0, COLOR_FVF, D3DPOOL_MANAGED, &m_pBGVertex, NULL);

	COLORVERTEX *pVertices;
	m_pBGVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < m_nCols + 1; i++)
	{
		float x = (float)(i * m_nSpacing);
		DWORD color = (i % 2) ? m_color1 : m_color2;
		pVertices[2 * i]     = { x, m_fHeight, 0.0f, color };
		pVertices[2 * i + 1] = { x,      0.0f, 0.0f, color };
	}

	m_pFrameVertex->Unlock();
}

void TetrisGrid::Update(double fTime, float fElapsedTime)
{
	COLORVERTEX *pVertices;
	m_pFrameVertex->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 2 * (m_nRows + m_nCols + 2); i++)
		pVertices[i].color = GetBGFrameColor();

	m_pFrameVertex->Unlock();
}

void TetrisGrid::Render(double fTime, float fElapsedTime)
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
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * m_nCols);

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
//Piece
//

int aRow[7][4][4] =
{
	{ { 0,0,0,0 },{ -2,-1,0,1 },{ 0,0,0,0 },{ -2,-1,0,1 } },		//I
	{ { 0,1,1,1 },{ 1,0,-1,-1 },{ 0,0,0,1 },{ 1,1,0,-1 } },			//J
	{ { 1,1,1,0 },{ -1,0,1,1 },{ 1,0,0,0 },{ 0,0,1,2 } },			//L
	{ { 0,0,1,1 },{ -1,0,0,1 },{ 0,0,1,1 },{ -1,0,0,1 } },			//Z
	{ { 0,0,1,1 },{ -1,0,0,1 },{ 0,0,1,1 },{ -1,0,0,1 } },			//S
	{ { 0,1,1,1 },{ 1,0,1,2 },{ 1,1,1,2 },{ 1,0,1,2 } },			//T
	{ { 0,0,1,1 },{ 0,0,1,1 },{ 0,0,1,1 },{ 0,0,1,1 } },			//O
};

int aCol[7][4][4] =
{
	{ { 0,1,2,3 },{ 1,1,1,1 },{ 0,1,2,3 },{ 1,1,1,1 } },			//I
	{ { 0,0,1,2 },{ 0,0,0,1 },{ 0,1,2,2 },{ 0,1,1,1 } },			//J
	{ { 0,1,2,2 },{ 1,1,1,2 },{ 0,0,1,2 },{ 0,1,1,1 } },			//L
	{ { 0,1,1,2 },{ 1,1,0,0 },{ 0,1,1,2 },{ 1,1,0,0 } },			//Z
	{ { 2,1,1,0 },{ 0,0,1,1 },{ 2,1,1,0 },{ 0,0,1,1 } },			//S
	{ { 1,0,1,2 },{ 2,1,1,1 },{ 0,1,2,1 },{ 0,1,1,1 } },			//T
	{ { 0,1,1,0 },{ 0,1,1,0 },{ 0,1,1,0 },{ 0,1,1,0 } },			//O
};

Piece::Piece()
{
	Blocks::Blocks();

	m_Bound[0] = 0;
	m_Bound[1] = 0;
	m_Bound[2] = 0;
	m_Bound[3] = 0;
	m_pStock = NULL;
	m_bStocked = TRUE;

	m_fUpdateTime = 0.0f;

	m_Type = I;
	m_Roll = 0;
	m_Row = 0;
	m_Col = 0;
	m_dwColor = WHITE;
}

Piece::~Piece()
{
	Blocks::~Blocks();
}

void Piece::Create()
{
	PieceType aPiece[7] = { I,J,L,Z,S,T,O };
	m_Type = aPiece[rand() % 7];
	m_Roll = 0;
	m_Row = 0;
	m_Col = 0;
	m_dwColor = D3DCOLOR_ARGB(255, 192 + rand() % 64, 192 + rand() % 64, 192 + rand() % 64);
	std::vector<BlockData>().swap(m_vecBlock);
	for (int i = 0; i < 4; i++)
	{
		int row = m_Row + aRow[m_Type][m_Roll][i];
		int col = m_Col + aCol[m_Type][m_Roll][i];
		m_vecBlock.push_back(BlockData(row, col, m_dwColor, true));
	}
}

BOOL Piece::GetNew(Piece *pPieceNext)
{
	m_Type = pPieceNext->m_Type;
	m_dwColor = pPieceNext->m_dwColor;
	m_Roll = 0;
	m_Row = m_Bound[2];
	m_Col = (m_Bound[0] + m_Bound[1]) / 2 - 1;
	pPieceNext->Create();
	m_bStocked = FALSE;
	return SetPos(m_Row, m_Col, m_Roll);
}

void Piece::Update(double fTime, float fElapsedTime)
{
	m_fUpdateTime += fElapsedTime;

	GetInput();
	
	//Down

	float fTimeGap = g_fUpdateGap * (IsKeyDown(DIK_DOWN) ? 0.1f : 1.0f);
	if (m_fUpdateTime > fTimeGap)
	{
		Down();
		while (m_fUpdateTime > fTimeGap)
			m_fUpdateTime -= fTimeGap;
	}

	//Left

	static float fLeftGap = 0.0f;
	fLeftGap += fElapsedTime;
	if (IsKeyDown(DIK_LEFT))
	{
		if (fLeftGap >= 0.0f)
		{
			Left();
			fLeftGap -= 500.0f;
		}
		if (fLeftGap >= -100.0f)
		{
			Left();
			fLeftGap -= 100.0f;
		}
	}
	else
		fLeftGap = 0.0f;

	//Right

	static float fRightGap = 0.0f;
	fRightGap += fElapsedTime;
	if (IsKeyDown(DIK_RIGHT))
	{
		if (fRightGap >= 0.0f)
		{
			Right();
			fRightGap -= 500.0f;
		}
		if (fRightGap >= -100.0f)
		{
			Right();
			fRightGap -= 100.0f;
		}
	}
	else
		fRightGap = 0.0f;

	//Rotate

	static bool bUpUp = true;
	if (IsKeyDown(DIK_UP))
	{
		if (bUpUp)
		{
			Rotate();
			bUpUp = false;
		}
	}
	else
		bUpUp = true;

	//Quick Down

	static bool bSpaceUp = true;
	if (IsKeyDown(DIK_SPACE))
	{
		if (bSpaceUp)
		{
			bSpaceUp = false;
			while (Down());
			m_fUpdateTime = 0.0f;
		}	
	}
	else
		bSpaceUp = true;

	std::vector<BlockData>().swap(m_vecBlock);
	for (int i = 0; i < 4; i++)
	{
		int row = m_Row + aRow[m_Type][m_Roll][i];
		int col = m_Col + aCol[m_Type][m_Roll][i];
		DWORD color = m_dwColor;
		if (m_bStocked)
			color -= D3DCOLOR_ARGB(0, 32, 32, 32);
		m_vecBlock.push_back(BlockData(row, col, color, true));
	}
}

void Piece::SetStock(Blocks *pStock)
{
	m_pStock = pStock;
}

void Piece::SetBound(int left, int right, int top, int bottom)
{
	m_Bound[0] = left;
	m_Bound[1] = right;
	m_Bound[2] = top;
	m_Bound[3] = bottom;
}

BOOL Piece::SetPos(int row, int col, int roll)
{
	for (int i = 0; i < 4; i++)
	{
		int r = row + aRow[m_Type][roll][i];
		int c = col + aCol[m_Type][roll][i];
		if (NULL != m_pStock->FindBlock(r, c))
			return FALSE;
		if (c < m_Bound[0] || c > m_Bound[1] || r < m_Bound[2] || r > m_Bound[3])
			return FALSE;
	}

	m_Row = row;
	m_Col = col;
	m_Roll = roll;

	return TRUE;
}

BOOL Piece::Left() 
{
	return SetPos(m_Row, m_Col - 1, m_Roll);
}

BOOL Piece::Right()
{
	return SetPos(m_Row, m_Col + 1, m_Roll);
}

BOOL Piece::Down()
{
	int newRow = m_Row + 1;
	if (!SetPos(newRow, m_Col, m_Roll))
	{
		m_bStocked = true;
		return FALSE;
	}
	return TRUE;
}

BOOL Piece::Rotate()
{
	int newRoll = (m_Roll + 1) % 4;
	if (SetPos(m_Row, m_Col, newRoll))
		return TRUE;
	else if (SetPos(m_Row, m_Col - 1, newRoll))
		return TRUE;
	else if (SetPos(m_Row, m_Col + 1, newRoll))
		return TRUE;
	else if (SetPos(m_Row - 1, m_Col, newRoll))
		return TRUE;
	else if (SetPos(m_Row + 1, m_Col, newRoll))
		return TRUE;
	else if (SetPos(m_Row + 2, m_Col, newRoll))
		return TRUE;
	else
		return FALSE;
}

BOOL Piece::Stocked()
{
	return m_bStocked;
}

//
//Stock
//

void Stock::Update(double fTime, float fElapsedTime)
{
	static std::vector<int> vecClear;

	if (0 == vecClear.size())
	{
		for (int row = m_pGrid->GetRowCount() - 1; row >= 0; row--)
		{
			int cBlocks = 0;
			for (auto data : m_vecBlock)
				if (row == data.row) cBlocks++;
			if (cBlocks == m_pGrid->GetColCount())
				vecClear.push_back(row);
		}
	}

	static float fClearTime = 0.0f;
	if (0 != vecClear.size())
	{
		fClearTime += fElapsedTime;
		static int col = 0;
		if (col< m_pGrid->GetColCount())
		{
			if (fClearTime > col * g_fUpdateGap / 20.0f)
			{
				for (int rowClear : vecClear)
				{
					for (auto &data : m_vecBlock)
					{
						if (rowClear == data.row && col == data.col)
							data.color = WHITE;
					}
				}
				col++;
			}
		}

		if (fClearTime > col * g_fUpdateGap / 20.0f)
		{
			for (int i = 0; i < (int)vecClear.size(); i++)
			{
				auto iter = m_vecBlock.begin();
				while (iter != m_vecBlock.end())
				{
					if (iter->row == vecClear[i])
						iter = m_vecBlock.erase(iter);
					else
					{
						if (iter->row < vecClear[i])
							iter->row++;
						iter++;
					}
				}
				for (int j = i + 1; j < (int)vecClear.size(); j++)
					vecClear[j] = vecClear[j] + 1;
			}
			col = 0;
			fClearTime = 0.0f;
			vecClear.clear();
		}
	}
}

//
//GameScene
//

SceneTetris::SceneTetris()
{
	m_pGrid			= NULL;
	m_pStock		= NULL;
	m_pPiece		= NULL;
	m_pGridNext		= NULL;
	m_pPieceNext	= NULL;
	iLevel			= 0;
}

SceneTetris::~SceneTetris()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pStock);
	SAFE_DELETE(m_pPiece);
	SAFE_DELETE(m_pGridNext);
	SAFE_DELETE(m_pPieceNext);
}

void SceneTetris::Init()
{
	m_pGrid = new TetrisGrid();
	m_pGrid->Create(18, 12, 32, -288, -288);

	m_pStock = new Stock();
	m_pStock->SetGrid(m_pGrid);
	m_pStock->SetTexture(L"Data\\Resources\\Pictures\\block.png");

	m_pPiece = new Piece();
	m_pPiece->SetGrid(m_pGrid);
	m_pPiece->SetStock(m_pStock);
	m_pPiece->SetBound(0, m_pGrid->GetColCount() - 1, 0, m_pGrid->GetRowCount() - 1);
	m_pPiece->SetTexture(L"Data\\Resources\\Pictures\\block.png");

	m_pGridNext = new TetrisGrid();
	m_pGridNext->Create(2, 4, 32, 192, -224);

	m_pPieceNext = new Piece();
	m_pPieceNext->SetGrid(m_pGridNext);
	m_pPieceNext->SetTexture(L"Data\\Resources\\Pictures\\block.png");
	m_pPieceNext->Create();
}

void SceneTetris::Update(double fTime, float fElapsedTime)
{
	m_pGrid->Update(fTime, fElapsedTime);
	m_pGridNext->Update(fTime, fElapsedTime);

	if (m_pPiece->Stocked())
	{
		m_pStock->AddBlock(m_pPiece);
		if (!m_pPiece->GetNew(m_pPieceNext))
			GameOver();
	}
	m_pPiece->Update(fTime, fElapsedTime);
	m_pStock->Update(fTime, fElapsedTime);
}

void SceneTetris::Render(double fTime, float fElapsedTime)
{
	m_pGrid->Render(fTime, fElapsedTime);
	m_pGridNext->Render(fTime, fElapsedTime);
}