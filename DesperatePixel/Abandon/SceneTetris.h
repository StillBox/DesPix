#pragma once
#ifndef SCENETETRIS_H
#define SCENETETRIS_H

#include "Game.h"
#include "GameObject.h"

//
//Grid
//

class TetrisGrid :public Grid
{
private:
	IDirect3DVertexBuffer9 *m_pBGVertex;
	DWORD m_color1, m_color2;

public:
	TetrisGrid();
	~TetrisGrid();

	void Create(int nRows, int nCols, int nSpacing, int x = 0, int y = 0);
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

//
//Piece
//

enum PieceType { I = 0, J, L, Z, S, T, O };

class Piece :public Blocks
{
private:
	Blocks *m_pStock;
	int m_Bound[4];
	BOOL m_bStocked;

	float m_fUpdateTime;

	PieceType m_Type;
	int m_Roll;
	int m_Row, m_Col;
	DWORD m_dwColor;

public:
	Piece();
	~Piece();
	void Update(double fTime, float fElapsedTime);

public:
	void SetStock(Blocks *pStock);
	void SetBound(int left, int right, int top, int bottom);

	void Create();
	BOOL GetNew(Piece *pPieceNext);
	BOOL SetPos(int row, int col, int roll);
	BOOL Left();
	BOOL Right();
	BOOL Down();
	BOOL Rotate();
	BOOL Stocked();
};
//
//Stock
//

class Stock :public Blocks
{
public:
	void Update(double fTime, float fElapsedTime);
};

//
//Scene
//

class SceneTetris:public GameScene
{
private:
	TetrisGrid	*m_pGrid;
	Stock		*m_pStock;
	Piece		*m_pPiece;

	TetrisGrid	*m_pGridNext;
	Piece		*m_pPieceNext;

	int			iLevel;

public:
	SceneTetris();
	~SceneTetris();

	void Init();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENETETRIS_H
