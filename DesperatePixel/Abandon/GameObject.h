#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <algorithm>
#include "Game.h"
#include "DXObject.h"

//
//Background
//

class Background : public DXObject
{
public:
	void TexMove(float uDist, float vDist);
};

//
//Grid
//

class Blocks;

class Grid
{
protected:
	IDirect3DDevice9		*m_pd3dDevice;
	IDirect3DVertexBuffer9	*m_pFrameVertex;
	IDirect3DVertexBuffer9	*m_pBlockVertex;

	bool					m_bFrame;
	int						m_x, m_y;
	int						m_nRows, m_nCols, m_nSpacing;
	float					m_fWidth, m_fHeight;

	std::vector<Blocks*>	m_vecBlocks;

public:
	Grid();
	~Grid();

	virtual	void Create(int nRows, int nCols, int nSpacing, int x = 0, int y = 0);
	int GetRowCount();
	int GetColCount();

	void AddBlocks(Blocks *pBlocks);
	void DelBlocks(Blocks *pBlocks);

	void RenderFrame();
	void RenderBlock(int row, int col, DWORD color = BLACK, IDirect3DTexture9 *pTexture = NULL);

	virtual	void Update(double fTime, float fElapsedTime);
	virtual	void Render(double fTime, float fElapsedTime);
};

//
//Blocks
//

struct BlockData
{
	int					row = 0, col = 0;
	DWORD				color;
	bool				bDrawTex;
	BlockData(int r = 0, int c = 0, DWORD dwColor = BLACK, bool bDraw = true) :
		row(r), col(c), color(dwColor), bDrawTex(bDraw) {};
};

class Blocks
{
protected:

	Grid					*m_pGrid;
	IDirect3DTexture9		*m_pTexture;
	std::vector<BlockData>	m_vecBlock;

public:
	Blocks();
	~Blocks();

	void SetGrid(Grid *pGrid);
	void SetTexture(WCHAR *szFile);

	void AddBlock(int row, int col, DWORD color);
	void DelBlock(int row, int col);
	void AddBlock(Blocks *pBlocks);
	
	BlockData* FindBlock(int row, int col);

	virtual void Update(double fTime, float fElapsedTime);
	virtual void Render(double fTime, float fElapsedTime);
};

//
//Curtain
//

class Curtain
{
private:
	struct POINTVERTRHWEXCOLOR
	{
		float x, y, z, rhw;
		DWORD color;
		static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	};

	LPDIRECT3DDEVICE9		m_pd3dDevice;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	RECTRANGE				m_Range;

	BOOL					m_IsOn;
	int						m_AlphaBeg, m_AlphaEnd;
	float					m_fDuration;
	float					m_fPassTime;
	GAME_MODE				m_NextMode;
	void					(*m_pOnCurtainEnd)();

public:
	Curtain();
	Curtain(float left, float top, float right, float bottom);
	~Curtain();

	void Set(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)() = NULL);
	BOOL IsOn() { return m_IsOn; }
	GAME_MODE GetNextMode() { return m_NextMode; }

	void RenderAlpha(int Alpha);
	void Render(float fElapsedTime);
};

#endif // !GAMEOBJECT_H