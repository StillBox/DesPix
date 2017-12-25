#include "DXUT.h"

#define MIN_WIDTH 240
#define MIN_HEIGHT 180

//
//Micros for managing states of Direct3D and window
//

#define SET_ACCESSOR(x, y)		inline void Set##y(x t)		{ m_##y = t; }
#define GET_ACCESSOR(x, y)		inline x Get##y()			{ return m_##y; }
#define	GET_SET_ACCESSOR(x, y)	SET_ACCESSOR(x, y) GET_ACCESSOR( x, y )

#define SETP_ACCESSOR(x, y)		inline void Set##y(x* t)	{ m_##y = *t; }
#define GETP_ACCESSOR(x, y)		inline x* Get##y()			{ return &m_##y; }
#define	GETP_SETP_ACCESSOR(x, y)	SETP_ACCESSOR(x, y) GETP_ACCESSOR(x, y)

//
//Class for managing states of Direct3D and window
//

class DXUTSTATE
{
private:
	//Direct3D9
	BOOL					m_DeviceCreated = FALSE;

	LPDIRECT3DDEVICE9		m_D3DDevice = NULL;
	D3DPRESENT_PARAMETERS	m_D3DPP;
	UINT					m_DeviceWidth;
	UINT					m_DeviceHeight;

	bool					m_DeviceLost = false;

	//General
	BOOL					m_WindowCreated = FALSE;

	HINSTANCE				m_HInstance = NULL;
	HWND					m_HWnd = NULL;
	UINT					m_WindowWidth;
	UINT					m_WindowHeight;
	DWORD					m_WindowStyle;
	bool					m_Menu;

	bool					m_WindowReset = false;

	//Settings
	float					m_FPS = 60.0f;

	UINT					m_ExitCode = 0;

private:
	LPMSGPROC				m_MsgProcFunc = NULL;
	LPD3DCREATEDEVICE		m_D3DCreateDeviceFunc = NULL;
	LPD3DRESETDEVICE		m_D3DResetDeviceFunc = NULL;
	LPD3DLOSTDEVICE			m_D3DLostDeviceFunc = NULL;
	LPD3DDESTROYDEVICE		m_D3DDestroyDeviceFunc = NULL;
	LPD3DFRAMEUPDATE		m_D3DFrameUpdateFunc = NULL;
	LPD3DFRAMERENDER		m_D3DFrameRenderFunc = NULL;

public:
	GET_SET_ACCESSOR(BOOL,					DeviceCreated);
	GET_SET_ACCESSOR(LPDIRECT3DDEVICE9,		D3DDevice);
	GET_SET_ACCESSOR(D3DPRESENT_PARAMETERS,	D3DPP);
	GET_SET_ACCESSOR(UINT,					DeviceWidth);
	GET_SET_ACCESSOR(UINT,					DeviceHeight);
	GET_SET_ACCESSOR(bool,					DeviceLost);

	GET_SET_ACCESSOR(BOOL,					WindowCreated);
	GET_SET_ACCESSOR(HINSTANCE,				HInstance);
	GET_SET_ACCESSOR(HWND,					HWnd);
	GET_SET_ACCESSOR(UINT,					WindowWidth);
	GET_SET_ACCESSOR(UINT,					WindowHeight);
	GET_SET_ACCESSOR(DWORD,					WindowStyle);
	GET_SET_ACCESSOR(bool,					Menu);
	GET_SET_ACCESSOR(bool,					WindowReset);

	GET_SET_ACCESSOR(float,					FPS);
	GET_SET_ACCESSOR(UINT,					ExitCode);

	GET_SET_ACCESSOR(LPMSGPROC,				MsgProcFunc);
	GET_SET_ACCESSOR(LPD3DCREATEDEVICE,		D3DCreateDeviceFunc);
	GET_SET_ACCESSOR(LPD3DRESETDEVICE,		D3DResetDeviceFunc);
	GET_SET_ACCESSOR(LPD3DLOSTDEVICE,		D3DLostDeviceFunc);
	GET_SET_ACCESSOR(LPD3DDESTROYDEVICE,	D3DDestroyDeviceFunc);
	GET_SET_ACCESSOR(LPD3DFRAMERENDER,		D3DFrameUpdateFunc);
	GET_SET_ACCESSOR(LPD3DFRAMERENDER,		D3DFrameRenderFunc);
};

DXUTSTATE *g_pDXUTState = new DXUTSTATE();

DXUTSTATE &GetDXUTState()
{
	assert(g_pDXUTState != NULL);
	return *g_pDXUTState;
}

LPDIRECT3DDEVICE9 DXUTGetD3DDevice()	{ return GetDXUTState().GetD3DDevice(); }
UINT DXUTGetDeviceWidth()				{ return GetDXUTState().GetDeviceWidth(); }
UINT DXUTGetDeviceHeight()				{ return GetDXUTState().GetDeviceHeight(); }
HINSTANCE DXUTGetHInstance()			{ return GetDXUTState().GetHInstance(); }
HWND DXUTGetHWnd()						{ return GetDXUTState().GetHWnd(); }
UINT DXUTGetWindowWidth()				{ return GetDXUTState().GetWindowWidth(); }
UINT DXUTGetWindowHeight()				{ return GetDXUTState().GetWindowHeight(); }
UINT DXUTGetExitCode()					{ return GetDXUTState().GetExitCode(); }
void DXUTSetFPS(float fFPS)				{ GetDXUTState().SetFPS(fFPS); }
float DXUTGetFPS()						{ return GetDXUTState().GetFPS(); }

void SetCallbackMsgProc(LPMSGPROC pCallback)				{ GetDXUTState().SetMsgProcFunc(pCallback); }
void SetCallbackCreateDevice(LPD3DCREATEDEVICE pCallback)	{ GetDXUTState().SetD3DCreateDeviceFunc(pCallback); }
void SetCallbackResetDevice(LPD3DRESETDEVICE pCallback)		{ GetDXUTState().SetD3DResetDeviceFunc(pCallback); }
void SetCallbackLostDevice(LPD3DLOSTDEVICE pCallback)		{ GetDXUTState().SetD3DLostDeviceFunc(pCallback); }
void SetCallbackDestroyDevice(LPD3DDESTROYDEVICE pCallback)	{ GetDXUTState().SetD3DDestroyDeviceFunc(pCallback); }
void SetCallbackD3DFrameUpdate(LPD3DFRAMERENDER pCallback)	{ GetDXUTState().SetD3DFrameUpdateFunc(pCallback); }
void SetCallbackD3DFrameRender(LPD3DFRAMERENDER pCallback)	{ GetDXUTState().SetD3DFrameRenderFunc(pCallback); }

//
//Functions for Window
//

HRESULT WINAPI DXUTCreateWindow(const WCHAR* szWindowTitle, DWORD dwStyle,
	HINSTANCE hInstance, HICON hIcon, HMENU hMenu, int cx, int cy)
{
	if (hInstance == NULL)
		hInstance = (HINSTANCE)GetModuleHandle(NULL);
	GetDXUTState().SetHInstance(hInstance);

	WNDCLASS wndClass;
	wndClass.style			= CS_DBLCLKS;
	wndClass.lpfnWndProc	= DXUTWndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= (hIcon == NULL) ? LoadIcon(NULL, IDI_APPLICATION) : hIcon;
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= L"D3DWindowClass";

	if (!RegisterClass(&wndClass))
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
			return DXErrMsgBox(L"RegisterClass", dwError);
	}

	RECT rect;
	if (cx == CW_USEDEFAULT) cx = MIN_WIDTH;
	if (cy == CW_USEDEFAULT) cy = MIN_HEIGHT;
	SetRect(&rect, 0, 0, cx, cy);
	AdjustWindowRect(&rect, dwStyle, (hMenu != NULL) ? TRUE : FALSE);
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);

	HWND hWnd = CreateWindow(L"D3DWindowClass", szWindowTitle, dwStyle,
		(cxScreen - rect.right + rect.left) / 2, (cyScreen - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, NULL, hMenu, hInstance, NULL);

	if (hWnd == NULL)
	{
		DWORD dwError = GetLastError();
		return DXErrMsgBox(L"CreateWindow", dwError);
	}
	ShowWindow(hWnd, SW_SHOW);

	GetDXUTState().SetWindowCreated(TRUE);
	GetDXUTState().SetHWnd(hWnd);
	GetDXUTState().SetWindowStyle(dwStyle);
	GetDXUTState().SetMenu((hMenu != NULL) ? true : false);

	return S_OK;
}

HRESULT WINAPI DXUTMainLoop(HACCEL hAccel)
{
	HWND hWnd = GetDXUTState().GetHWnd();
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		static double timePrev = timeGetTime(), time = timeGetTime();
		static float timeDelta = 0.0f;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (hAccel == NULL || hWnd == NULL || 0 == TranslateAccelerator(hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			time = timeGetTime();
			timeDelta = (float)(time - timePrev);
			{
				DXUTFrameRender(time, timeDelta);
				timePrev = time;
			}
		}
	}

	if (hAccel != NULL)
		DestroyAcceleratorTable(hAccel);

	return S_OK;
}

LRESULT CALLBACK DXUTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		GetDXUTState().SetWindowWidth(short(LOWORD(lParam)));
		GetDXUTState().SetWindowHeight(short(HIWORD(lParam)));
	case WM_MOVE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	LPMSGPROC MsgProc = GetDXUTState().GetMsgProcFunc();
	if (NULL!=MsgProc) MsgProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT DXUTResetWindow()
{
	RECT rect;
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);

	int cx = GetDXUTState().GetWindowWidth();
	int cy = GetDXUTState().GetWindowHeight();
	DWORD dwStyle = GetDXUTState().GetWindowStyle();

	SetRect(&rect, 0, 0, cx, cy);
	AdjustWindowRect(&rect, dwStyle, GetDXUTState().GetMenu());
	SetWindowLong(DXUTGetHWnd(), GWL_STYLE, dwStyle);
	MoveWindow(DXUTGetHWnd(),
		(cxScreen - rect.right + rect.left) / 2,
		(cyScreen - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, TRUE);
	ShowWindow(DXUTGetHWnd(), SW_SHOW);

	GetDXUTState().SetWindowReset(false);

	return S_OK;
}

//
//Functions for Direct3D
//

HRESULT DXUTCreateDevice(int cx, int cy)
{
	LPDIRECT3D9 pd3d = NULL;
	if (NULL == (pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DCAPS9 caps;
	int vp = 0;
	if (S_OK != pd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps))
		return E_FAIL;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = max(cx, MIN_WIDTH);
	d3dpp.BackBufferHeight = max(cy, MIN_HEIGHT);
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 2;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetDXUTState().GetHWnd();
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	GetDXUTState().SetD3DPP(d3dpp);
	GetDXUTState().SetDeviceWidth(cx);
	GetDXUTState().SetDeviceHeight(cy);

	LPDIRECT3DDEVICE9 pd3dDevice;
	if (S_OK != pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		GetDXUTState().GetHWnd(), vp, &d3dpp, &pd3dDevice))
		return E_FAIL;

	GetDXUTState().SetDeviceCreated(TRUE);
	GetDXUTState().SetD3DDevice(pd3dDevice);

	SAFE_RELEASE(pd3d);

	LPD3DCREATEDEVICE CreateDevice = GetDXUTState().GetD3DCreateDeviceFunc();
	if (CreateDevice != NULL)
	{
		HRESULT hr;
		HR(hr, CreateDevice());
	}

	return S_OK;
}

HRESULT DXUTResetDevice()
{
	D3DPRESENT_PARAMETERS d3dpp = GetDXUTState().GetD3DPP();

	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3DDevice();

	HRESULT hr = pd3dDevice->Reset(&d3dpp);

	if (S_OK != hr)
	{
		GetDXUTState().SetDeviceLost(true);
		return hr;
	}
	else
		GetDXUTState().SetDeviceLost(false);

	LPD3DRESETDEVICE OnResetDevice = GetDXUTState().GetD3DResetDeviceFunc();
	if (OnResetDevice != NULL)
	{
		HRESULT hr;
		HR(hr, OnResetDevice());
	}

	return S_OK;
}

HRESULT DXUTLostDevice()
{
	LPD3DLOSTDEVICE OnLostDevice = GetDXUTState().GetD3DLostDeviceFunc();
	if (OnLostDevice != NULL)
	{
		HRESULT hr;
		HR(hr, OnLostDevice());
	}

	return S_OK;
}

HRESULT DXUTDestroyDevice()
{
	LPD3DDESTROYDEVICE OnDestroyDevice = GetDXUTState().GetD3DDestroyDeviceFunc();

	if (OnDestroyDevice != NULL)
	{
		HRESULT hr;
		HR(hr, OnDestroyDevice());
	}

	IDirect3DDevice9 *pd3dDevice = GetDXUTState().GetD3DDevice();
	SAFE_RELEASE(pd3dDevice);

	return S_OK;
}

void DXUTFrameRender(double fTime, float fElapsedTime)
{
	HRESULT hr;

	if (GetDXUTState().GetDeviceLost())
	{
		Sleep(50);
	}

	LPDIRECT3DDEVICE9 pd3dDevice = GetDXUTState().GetD3DDevice();

	if (NULL == pd3dDevice)
	{
		if (GetDXUTState().GetDeviceLost())
		{
			DXUTLostDevice();
			DXUTResetDevice();
		}
		return;
	}

	if (GetDXUTState().GetDeviceLost())
	{
		hr = DXUTLostDevice();

		if (GetDXUTState().GetWindowReset())
		{
			DXUTResetWindow();
		}

		hr = DXUTResetDevice();
		if (FAILED(hr))
		{
			GetDXUTState().SetDeviceLost(true);
			return;
		}
	}

	LPD3DFRAMERENDER FrameUpdate = GetDXUTState().GetD3DFrameUpdateFunc();
	if (FrameUpdate != NULL) FrameUpdate(pd3dDevice, fTime, fElapsedTime);

	LPD3DFRAMERENDER FrameRender = GetDXUTState().GetD3DFrameRenderFunc();
	if (FrameRender != NULL) FrameRender(pd3dDevice, fTime, fElapsedTime);

	hr = pd3dDevice->Present(NULL, NULL, NULL, NULL);
	if (D3DERR_DEVICELOST == hr)
	{
		GetDXUTState().SetDeviceLost(true);
	}
}

void DXUTChangeDevice(int cx, int cy)
{
	if (cx != 0 && cy != 0)
	{
		D3DPRESENT_PARAMETERS d3dpp = GetDXUTState().GetD3DPP();
		d3dpp.BackBufferWidth = max(cx, MIN_WIDTH);
		d3dpp.BackBufferHeight = max(cy, MIN_HEIGHT);
		GetDXUTState().SetD3DPP(d3dpp);
		GetDXUTState().SetDeviceWidth(cx);
		GetDXUTState().SetDeviceHeight(cy);
	}

	GetDXUTState().SetDeviceLost(true);
}

void DXUTChangeWindow(int cx, int cy, DWORD dwStyle)
{
	if (cx != 0 && cy != 0)
	{
		GetDXUTState().SetWindowWidth(max(cx, MIN_WIDTH));
		GetDXUTState().SetWindowHeight(max(cy, MIN_HEIGHT));
		GetDXUTState().SetWindowStyle(dwStyle);
	}

	GetDXUTState().SetWindowReset(true);
	GetDXUTState().SetDeviceLost(true);
}

//Functions for commom uses

HRESULT DXErrMsgBox(WCHAR* szErrMsg, DWORD dwError)
{
	WCHAR szBuffer[60];
	wsprintf(szBuffer, L"%s:%d", szErrMsg, dwError);
	MessageBox(NULL, szBuffer, L"Error Occured", MB_ICONERROR);
	return HRESULT_FROM_WIN32(dwError);
}

float GetRealtimeFPS()
{
	static float FPS = 0.0f;
	static int cFrames = 0;
	static double time = 0;
	static double timePrev = 0;
	cFrames++;
	time = timeGetTime();
	if (time - timePrev > 1000)
	{
		FPS = (float)(1000.0 * cFrames / (time - timePrev));
		timePrev = time;
		cFrames = 0;
	}

	return FPS;
}