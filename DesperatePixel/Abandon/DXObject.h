#pragma once
#ifndef DXOBJECT_H
#define DXOBJECT_H

#include <vector>
#include "DXUT.h"

//Structs and classes for commom use

#define WHITE	D3DCOLOR_ARGB(255,255,255,255)
#define L_GRAY	D3DCOLOR_ARGB(255,191,191,191)
#define GRAY	D3DCOLOR_ARGB(255,127,127,127)
#define D_GRAY	D3DCOLOR_ARGB(255,63,63,63)
#define BLACK	D3DCOLOR_ARGB(255,0,0,0)
#define RED		D3DCOLOR_ARGB(255,255,0,0)
#define GREEN	D3DCOLOR_ARGB(255,0,255,0)
#define BLUE	D3DCOLOR_ARGB(255,0,0,255)
#define L_BLUE	D3DCOLOR_ARGB(255,127,127,255)

struct COLORVERTEX
{
	float				x, y, z;
	DWORD				color;
};
#define	COLOR_FVF D3DFVF_XYZ | D3DFVF_DIFFUSE

struct TEXVERTEX
{
	float				x, y, z;
	float				u, v;
};
#define	TEX_FVF D3DFVF_XYZ | D3DFVF_TEX1

struct COLORTEXVERTEX
{
	float				x, y, z;
	DWORD				color;
	float				u, v;
};
#define	COLORTEX_FVF D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1

struct RECTRANGE
{
	float left, top, right, bottom;
	RECTRANGE(float fLeft, float fTop, float fRight, float fBottom)
		:left(fLeft), top(fTop), right(fRight), bottom(fBottom) {}
	RECTRANGE() :RECTRANGE(0.0f, 0.0f, 0.0f, 0.0f) {}
};

struct ROUNDRANGE
{
	float xOffset, yOffset, radius;
	ROUNDRANGE(float fXOffset, float fYOffset, float fRadius)
		: xOffset(fXOffset), yOffset(fYOffset), radius(fRadius) {}
	ROUNDRANGE() :ROUNDRANGE(0.0f, 0.0f, 0.0f) {}
};

//
//Object class
//

class DXObject
{
protected:
	enum TEXLOOPTYPE { SINGLE = 0, LOOP = 1, NOLOOP = 2 };

	struct STATEDATA
	{
		TEXLOOPTYPE			LoopType = SINGLE;
		float				fTimeGap = 0.0f;
		int					curIndex = 0;
		std::vector<int>	vecTex;
		bool				bInvert = false;
		int					iBegState = -1, iEndState = -1;
	};

	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

	D3DXVECTOR2				m_ObjPos;
	RECTRANGE				m_ObjRange;
	int						m_Layer;
	float					m_Roll, m_Yaw, m_Pitch;

	int						m_TexWidth, m_TexHeight;
	LPDIRECT3DTEXTURE9		m_pTexture;
	std::vector<RECTRANGE>	m_TexRects;

	int						m_iState;
	std::vector<STATEDATA>	m_States;
	float					m_UpdateTime;

public:
	DXObject();
	virtual ~DXObject();

	void SetPos(float xPos, float yPos);
	void SetLayer(int zLayer);
	void SetRoll(float fRoll);
	void SetPitch(float fPitch);
	void SetYaw(float fYaw);
	void SetRange(float left, float top, float right, float bottom);
	void GetPos(D3DXVECTOR2 *pVec);

	void AddTexFromFile(WCHAR *szTexFile, int width, int height);
	void AddTexRect(int left, int top, int right, int bottom);
	BOOL SetTexRect(int iTexRec);

	void AddState(TEXLOOPTYPE LoopType, float fTimeGap);
	template <typename... Args>
	void AddState(TEXLOOPTYPE LoopType, float fTimeGap, int iTex, Args... rest)
	{
		AddState(LoopType, fTimeGap);
		int iState = m_States.size() - 1;
		AddStateTexRect(iState, iTex, rest...);
	}

	void AddStateTexRect(int iState, int iTex);
	template <typename... Args>
	void AddStateTexRect(int iState, int iTex, Args... rest)
	{
		AddStateTexRect(iState, iTex);
		AddStateTexRect(iState, rest...);
	}

	void AddStateChain(int iState, int iBegState = -1, int iEndState = -1);

	BOOL SetState(int iState);
	BOOL SetInvert(int iState, bool bInvert);

	virtual BOOL Init();
	virtual BOOL Update(double fTime, float fElapsedTime);
	virtual BOOL Render(double fTime, float fElapsedTime);
};

#endif // !DXOBJECT_H
