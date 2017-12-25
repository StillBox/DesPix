#include "DXInput.h"

DXInput::DXInput(void)
{
	m_pDirectInput = NULL;
	m_pKeyboardDevice = NULL;
	ZeroMemory(m_szKeyBuffer, sizeof(char) * 256);
	ZeroMemory(m_fKeyPressTime, sizeof(float) * 256);
	m_pMouseDevice = NULL;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

DXInput::~DXInput(void)
{
	if (m_pKeyboardDevice != NULL)
		m_pKeyboardDevice->Unacquire();
	if (m_pMouseDevice != NULL)
		m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pKeyboardDevice);
	SAFE_RELEASE(m_pMouseDevice);
	SAFE_RELEASE(m_pDirectInput);
}

HRESULT DXInput::InitInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	HRESULT hr;

	m_dwKeyboardCoopFlags = keyboardCoopFlags;
	m_dwMouseCoopFlags = mouseCoopFlags;
	HWND hwnd = DXUTGetHWnd();
	HINSTANCE hInstance = DXUTGetHInstance();

	HR(hr, DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDirectInput, NULL));

	HR(hr, m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL));
	HR(hr, m_pKeyboardDevice->SetCooperativeLevel(hwnd, keyboardCoopFlags));
	HR(hr, m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(hr, m_pKeyboardDevice->Acquire());
	HR(hr, m_pKeyboardDevice->Poll());

	HR(hr, m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL));
	HR(hr, m_pMouseDevice->SetCooperativeLevel(hwnd, mouseCoopFlags));
	HR(hr, m_pMouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(hr, m_pMouseDevice->Acquire());
	HR(hr, m_pMouseDevice->Poll());

	for (auto c : m_fKeyPressTime)
		c = (float)timeGetTime();

	return S_OK;
}

void DXInput::GetInput()
{
	HRESULT hr;

	while (NULL == m_pKeyboardDevice || NULL == m_pMouseDevice)
	{
		if (m_pKeyboardDevice != NULL)
			m_pKeyboardDevice->Unacquire();
		if (m_pMouseDevice != NULL)
			m_pMouseDevice->Unacquire();
		SAFE_RELEASE(m_pKeyboardDevice);
		SAFE_RELEASE(m_pMouseDevice);
		SAFE_RELEASE(m_pDirectInput);

		InitInput(m_dwKeyboardCoopFlags, m_dwMouseCoopFlags);
	}

	hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_szKeyBuffer), (LPVOID)m_szKeyBuffer);
	if (hr)
	{
		m_pKeyboardDevice->Acquire();
		m_pKeyboardDevice->GetDeviceState(sizeof(m_szKeyBuffer), (LPVOID)m_szKeyBuffer);
	}

	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID *)&m_MouseState);
	if (hr)
	{
		m_pMouseDevice->Acquire();
		m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID *)&m_MouseState);
	}
}

BOOL DXInput::IsKeyDown(UINT nKey, double fTimeGap)
{
	if (m_szKeyBuffer[nKey] & 0x80)
	{
		if (fTimeGap == 0.0)
			return TRUE;
		else
		{
			double time = timeGetTime();
			if (time > m_fKeyPressTime[nKey] + fTimeGap)
			{
				m_fKeyPressTime[nKey] = time;
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	else
		return FALSE;
}

BOOL DXInput::IsMouseButtonDown(UINT nButton)
{
	if (m_MouseState.rgbButtons[nButton] & 0x80)
		return TRUE;
	else
		return FALSE;
}

float DXInput::GetMouseX()
{
	return (float)m_MouseState.lX;
}

float DXInput::GetMouseY()
{
	return (float)m_MouseState.lY;
}

float DXInput::GetMouseZ()
{
	return (float)m_MouseState.lZ;
}

//Global Input

DXInput *g_pDXInput = new DXInput();

DXInput &GetDXInput()
{
	assert(g_pDXInput != NULL);
	return *g_pDXInput;
}

//Functions

HRESULT InitInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags) 
{
	return GetDXInput().InitInput(keyboardCoopFlags, mouseCoopFlags); 
}

HRESULT ReleaseInput()
{
	SAFE_DELETE(g_pDXInput);
	return S_OK;
}

void GetInput() { GetDXInput().GetInput(); }

BOOL IsKeyDown(UINT nKey, double fTimeGap)
{
	return GetDXInput().IsKeyDown(nKey, fTimeGap);
}

BOOL IsKeyDown()
{
	for (UINT nKey = 0; nKey < 256; nKey++)
	{
		if (IsKeyDown(nKey))
			return TRUE;
	}
	return FALSE;
}

BOOL IsMouseButtonDown(UINT nButton)
{
	return GetDXInput().IsMouseButtonDown(nButton);
}

float GetMouseX() { return GetDXInput().GetMouseX(); }
float GetMouseY() { return GetDXInput().GetMouseY(); }
float GetMouseZ() { return GetDXInput().GetMouseZ(); }