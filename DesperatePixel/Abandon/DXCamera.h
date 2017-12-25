#pragma once

#ifndef DXCAMERA_H
#define DXCAMERA_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DXUT.h"

class DXCamera
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_vFwd, m_vUp, m_vRight;
	D3DXVECTOR3 m_vCamera, m_vTarget;
	float m_Pitch, m_Yaw, m_Roll;
	enum CAMERAMODE { ORTHO = 0, PERSP = 1 } m_CameraMode;

public:
	DXCamera();
	~DXCamera();

	void Init();

	void CalculateViewMatrix(D3DXMATRIX *);
	
	void GetProjMatrix(D3DXMATRIX *);
	void GetCameraPosition(D3DXVECTOR3 *);
	
	void SetViewMatrix(D3DXMATRIX *pMatrix = NULL);
	void SetProjMatrix(D3DXMATRIX *pMatrix = NULL, float fWidth = 0.0f, float fHeight = 0.0f);
	void SetViewPort(float fX = 0.0f, float fY = 0.0f, float fWidth = 0.0f, float fHeight = 0.0f);
	
	void SetCameraPos(D3DXVECTOR3 *);
};

void InitDXCamera();
void SetView(D3DXMATRIX *pMatrix = NULL);
void SetProj(D3DXMATRIX *pMatrix = NULL, float fWidth = 0.0f, float fHeight = 0.0f);
void SetViewPort(float fX = 0.0f, float fY = 0.0f, float fWidth = 0.0f, float fHeight = 0.0f);
void SetCameraPos(D3DXVECTOR3 *);

#endif // !DXCAMERA_H