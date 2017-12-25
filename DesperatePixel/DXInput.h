#pragma once

#ifndef DXINPUT_H
#define DXINPUT_H

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <tchar.h>
#include "DXUT.h"

class DXInput
{
private:
	IDirectInput8 *m_pDirectInput;
	IDirectInputDevice8 *m_pKeyboardDevice;
	char m_szKeyBuffer[256];
	double m_fKeyPressTime[256];
	IDirectInputDevice8 *m_pMouseDevice;
	DIMOUSESTATE m_MouseState;
	DWORD m_dwKeyboardCoopFlags;
	DWORD m_dwMouseCoopFlags;

public:
	DXInput();
	~DXInput();
	HRESULT InitInput(DWORD, DWORD);
	void GetInput();
	BOOL IsKeyDown(UINT nKey, double fTimeGap =0.0f);
	BOOL IsMouseButtonDown(UINT nButton);
	float GetMouseX();
	float GetMouseY();
	float GetMouseZ();
};

HRESULT InitInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
HRESULT ReleaseInput();
void GetInput();
BOOL IsKeyDown(UINT nKey, double fTimeGap = 0.0f);
BOOL IsKeyDown();
BOOL IsMouseButtonDown(UINT nButton);
float GetMouseX();
float GetMouseY();
float GetMouseZ();

#endif // !DXINPUT_H
