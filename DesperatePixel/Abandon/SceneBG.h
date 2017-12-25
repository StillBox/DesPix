#pragma once
#ifndef SCENEBG_H
#define SCENEBG_H

#include "Game.h"
#include "GameObject.h"

//
//Grid
//

class BGGrid :public Grid
{
private:
	float m_fUpdateTime;
	int m_R1, m_R2, m_G1, m_G2, m_B1, m_B2;
	DWORD m_FrameColor;

public:
	BGGrid();
	~BGGrid();
	void Update(double fTime, float fElapsedTime);

public:
	DWORD GetFrameColor() { return m_FrameColor; }
};

//Seed
//Blocks
//

class SeedBlocks :public Blocks
{
private:
	struct Seed
	{
		int row, col;
		Seed(int r, int c) :row(r), col(c) {};
	};

	int m_cSeed;
	float m_fUpdateTime;
	std::vector<Seed*> m_vecLSeed;
	std::vector<Seed*> m_vecRSeed;
	std::vector<Seed*> m_vecUSeed;
	std::vector<Seed*> m_vecDSeed;
	std::vector<Seed*> m_vecBoomSeed;

public:
	SeedBlocks();
	~SeedBlocks();
	void Init();
	void Update(double fTime, float fElapsedTime);
};

//
//Scene
//

class SceneBG :public GameScene
{
private:
	BGGrid	*pBGGrid;
	SeedBlocks *pSeedBlocks;

public:
	SceneBG();
	~SceneBG();

	void Init();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);

public:
	DWORD GetFrameColor();
};

#endif