#pragma once

#ifndef DXUT_H
#define DXUT_H

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h>
#include <assert.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#ifndef HR
#define HR(hr,x)				{ hr = x; if (FAILED(hr)) { return hr; } }
#endif // !HR

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = NULL; } }
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = NULL; } }
#endif // !SAFE_DELETE_ARRAY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = NULL; } }
#endif // !SAFE_RELEASE

//
//Functions for managing DXUTStates
//

LPDIRECT3DDEVICE9 DXUTGetD3DDevice();
UINT DXUTGetDeviceWidth();
UINT DXUTGetDeviceHeight();

HINSTANCE DXUTGetHInstance();
HWND DXUTGetHWnd();
UINT DXUTGetWindowWidth();
UINT DXUTGetWindowHeight();
UINT DXUTGetExitCode();
void DXUTSetFPS(float fFPS);
float DXUTGetFPS();

//
//Callback functions
//

//Definitions

typedef LRESULT(CALLBACK *LPMSGPROC)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef HRESULT(CALLBACK *LPD3DCREATEDEVICE) ();
typedef HRESULT(CALLBACK *LPD3DRESETDEVICE) ();
typedef HRESULT(CALLBACK *LPD3DLOSTDEVICE) ();
typedef HRESULT(CALLBACK *LPD3DDESTROYDEVICE) ();
typedef void (CALLBACK *LPD3DFRAMEUPDATE)(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);
typedef void (CALLBACK *LPD3DFRAMERENDER)(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);

//Functions setting the callback functions

void SetCallbackMsgProc(LPMSGPROC);
void SetCallbackCreateDevice(LPD3DCREATEDEVICE);
void SetCallbackResetDevice(LPD3DRESETDEVICE);
void SetCallbackLostDevice(LPD3DLOSTDEVICE);
void SetCallbackDestroyDevice(LPD3DDESTROYDEVICE);
void SetCallbackD3DFrameUpdate(LPD3DFRAMEUPDATE);
void SetCallbackD3DFrameRender(LPD3DFRAMERENDER);

//
//Functions for running
//

HRESULT WINAPI DXUTCreateWindow(const WCHAR* szWindowTitle = L"Direct3D Window", 
	DWORD dwStyle = WS_OVERLAPPEDWINDOW, HINSTANCE hInstance = NULL, HICON hIcon = NULL, HMENU hMenu = NULL,
	int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT);
HRESULT WINAPI DXUTMainLoop(HACCEL hAccel = NULL);
LRESULT CALLBACK DXUTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT DXUTResetWindow();

//Functions for Direct3D
HRESULT DXUTCreateDevice(int cx, int cy);
HRESULT DXUTResetDevice();
void DXUTFrameRender(double fTime, float fElapsedTime);

void DXUTChangeDevice(int cx = 0, int cy = 0);
void DXUTChangeWindow(int cx = 0, int cy = 0, DWORD dwStyle = WS_OVERLAPPEDWINDOW);

//Functions for commom uses

HRESULT DXErrMsgBox(WCHAR* szErrMsg, DWORD dwErr);
float GetRealtimeFPS();

#endif // !D3DUTIL_H