#pragma once
#ifndef SCENESNAKE_H
#define SCENESNAKE_H

#include "Game.h"
#include "GameObject.h"

//
//Grid
//

class SnakeGrid :public Grid
{
private:
	IDirect3DVertexBuffer9 *m_pBGVertex;
	DWORD m_color1, m_color2;

public:
	SnakeGrid();
	~SnakeGrid();

	void Create(int nRows, int nCols, int nSpacing, int x = 0, int y = 0);
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

//
//Snake
//

class Snake :public Blocks
{
private:
	BOOL m_bDead;
	int m_iVRow;
	int m_iVCol;
	float m_fUpdateTime;
	float m_fUpdateGap;
	int m_Bound[4];

public:
	Snake();
	~Snake();

	void SetBound(int left, int right, int top, int bottom);

	void Create();
	void Update(double fTIme, float fElapsedTime);
	BOOL IsDead();
};

//
//Food
//

class Food :public Blocks
{
public:
	void Update(double fTime, float fElapsedTime);
};

//
//GameScene
//

class SceneSnake :public GameScene
{
private:
	int m_iLevel;

public:
	SceneSnake();
	~SceneSnake();

	void Init();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENESNAKE_H