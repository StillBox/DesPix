#pragma once
#ifndef GAMESCRN_H
#define GAMESCRN_H

#include "DXUT.h"
#include "Game.h"

enum REFMODE
{
	IMM,
	BRUSH,
	LINE,
	SHRINK
};

struct PIX
{
	int _x;
	int _y;
};

struct RHWVERTEX
{
	float				x, y, z, rhw;
	DWORD				color;
	float				u, v;
};
#define	RHW_FVF		D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1

class GameScrn
{
public:
	GameScrn();
	~GameScrn();

	BOOL Init();
	BOOL End();
	BOOL OnLostDevice();
	BOOL OnResetDevice();

	BOOL Update(double fTime, float fElapsedTime);
	BOOL Render(double fTime, float fElapsedTime);


	void Refresh(REFMODE RefMode = IMM, RECT *rcBrush = NULL);
	void Refresh(REFMODE RefMode, int xBeg, int yBeg, int xEnd, int yEnd);
	void SetRefPeriod(float fPeriod);
	bool IsRefOver();

public:
	void Set(bool bOn, D3DXCOLOR color, int x, int y);

	template <typename... Args>
	void Set(bool bOn, D3DXCOLOR color, int x, int y, Args... rest)
	{
		Set(bOn, color, x, y);
		Set(bOn, color, rest...);
	}

	void Set(bool bOn, D3DXCOLOR color, std::vector<PIX> *vecPix);
	void Set(bool bOn, D3DXCOLOR color, RECT *rcPix = NULL);
	void SetFrame(bool bOn, D3DXCOLOR color, RECT *rcPix = NULL);

	void Reverse(RECT *rcPix);


private:
	IDirect3DDevice9		*m_pd3dDevice;
	IDirect3DVertexBuffer9	*m_pVertexPix;	//Vertex for Pixels
	IDirect3DVertexBuffer9	*m_pVertexFrm;	//Vertex for Frame Lines
	IDirect3DVertexBuffer9	*m_pVertexCur;	//Vertex for Curtain
	IDirect3DTexture9		*m_pTexture;

	//Pix Var

	int			m_iPixSize;
	bool		m_bOnCur[CXPIX][CYPIX];
	bool		m_bOnTar[CXPIX][CYPIX];
	D3DXCOLOR	m_colorCur[CXPIX][CYPIX];
	D3DXCOLOR	m_colorTar[CXPIX][CYPIX];
	float		m_fOffsetCur[CXPIX][CYPIX];
	float		m_fOffsetTar[CXPIX][CYPIX];
	
	//Refresh Var

	float		m_fRefPeriod;

	REFMODE		m_RefMode;
	bool		m_bRefOver;
	int			m_xRefBeg;
	int			m_xRefEnd;
	int			m_yRefBeg;
	int			m_yRefEnd;
	int			m_iRefStep;
	float		m_RefTimer;

	bool		m_bSuspend;
	REFMODE		m_RefModeSuspend;
	int			m_xRefBegSuspend;
	int			m_xRefEndSuspend;
	int			m_yRefBegSuspend;
	int			m_yRefEndSuspend;
	int			m_iRefStepSuspend;
};

GameScrn &GetScrn();

#endif // !GAMESCRN_H
