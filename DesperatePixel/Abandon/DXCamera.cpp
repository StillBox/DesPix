#include "DXCamera.h"

DXCamera::DXCamera()
{
	m_pd3dDevice = NULL;
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_vFwd = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_vCamera = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
	m_Roll = 0.0f;
	m_CameraMode = ORTHO;
}

DXCamera::~DXCamera()
{
}

void DXCamera::Init()
{
	m_pd3dDevice = DXUTGetD3DDevice();
}

void DXCamera::CalculateViewMatrix(D3DXMATRIX *pMatrix)
{
	pMatrix->_11 = m_vRight.x;
	pMatrix->_12 = m_vUp.x;
	pMatrix->_13 = m_vFwd.x;
	pMatrix->_14 = 0.0f;

	pMatrix->_21 = m_vRight.y;
	pMatrix->_22 = m_vUp.y;
	pMatrix->_23 = m_vFwd.y;
	pMatrix->_24 = 0.0f;

	pMatrix->_31 = m_vRight.z;
	pMatrix->_32 = m_vUp.z;
	pMatrix->_33 = m_vFwd.z;
	pMatrix->_34 = 0.0f;

	pMatrix->_41 = -D3DXVec3Dot(&m_vCamera, &m_vRight);
	pMatrix->_42 = -D3DXVec3Dot(&m_vCamera, &m_vUp);
	pMatrix->_43 = -D3DXVec3Dot(&m_vCamera, &m_vFwd);
	pMatrix->_44 = 1.0f;
}

void DXCamera::GetProjMatrix(D3DXMATRIX *pMatrix)
{
	*pMatrix = m_matProj;
}

void DXCamera::GetCameraPosition(D3DXVECTOR3 *pVector)
{
	*pVector = m_vCamera;
}

void DXCamera::SetViewMatrix(D3DXMATRIX *pMatrix)
{
	if (pMatrix)
	{
		m_matView = *pMatrix;
		m_vRight = D3DXVECTOR3(pMatrix->_11, pMatrix->_21, pMatrix->_31);
		m_vUp = D3DXVECTOR3(pMatrix->_12, pMatrix->_22, pMatrix->_32);
		m_vFwd = D3DXVECTOR3(pMatrix->_13, pMatrix->_23, pMatrix->_33);
	}
	else
		CalculateViewMatrix(&m_matView);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void DXCamera::SetProjMatrix(D3DXMATRIX *pMatrix, float fWidth, float fHeight)
{
	if (pMatrix)
		m_matProj = *pMatrix;
	else
	{
		if (m_CameraMode == ORTHO)
			D3DXMatrixOrthoLH(&m_matProj, fWidth == 0.0f ? (float)DXUTGetDeviceWidth() : fWidth,
				fHeight == 0.0f ? (float)DXUTGetDeviceHeight() : fHeight, -2000.0f, 2000.0f);
		else
			D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f,
			(float)((double)DXUTGetDeviceWidth() / DXUTGetDeviceHeight()), 1.0f, 300000.0f);
	}
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void DXCamera::SetViewPort(float fX, float fY, float fWidth, float fHeight)
{
	D3DVIEWPORT9 vp;
	vp.X = (DWORD)fX;
	vp.Y = (DWORD)fY;
	vp.Width = (DWORD)((fWidth == 0.0f) ? DXUTGetDeviceWidth() : fWidth);
	vp.Height = (DWORD)((fWidth == 0.0f) ? DXUTGetDeviceHeight() : fHeight);
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	m_pd3dDevice->SetViewport(&vp);
}

void DXCamera::SetCameraPos(D3DXVECTOR3 *vCameraPos)
{
	m_vCamera = *vCameraPos;
}

DXCamera *g_pDXCamera = new DXCamera();

DXCamera &GetDXCamera()
{
	assert(g_pDXCamera != NULL);
	return *g_pDXCamera;
}

void InitDXCamera()
{
	GetDXCamera().Init();
}

void SetView(D3DXMATRIX *pMatrix)
{
	GetDXCamera().SetViewMatrix(pMatrix);
}

void SetProj(D3DXMATRIX *pMatrix, float fWidth, float fHeight)
{
	GetDXCamera().SetProjMatrix(pMatrix, fWidth, fHeight);
}

void SetViewPort(float fX, float fY, float fWidth, float fHeight)
{
	GetDXCamera().SetViewPort(fX, fY, fWidth, fHeight);
}

void SetCameraPos(D3DXVECTOR3 *vCameraPos)
{
	GetDXCamera().SetCameraPos(vCameraPos);
}