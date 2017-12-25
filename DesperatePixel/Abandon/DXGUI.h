#pragma once

#ifndef DXGUI_H
#define DXGUI_H

#include <vector>
#include <algorithm>
#include "DXUT.h"
#include "DXAudio.h"

class DXDialogResourceManager;
class DXDialog;
class DXControl;
class DXStatic;
class DXButton;
class DXCheckBox;
class DXScrollBar;
class DXEditBox;
class DXListBox;

enum DXCONTROL_TYPE
{
	DXCONTROL_BUTTON,
	DXCONTROL_STATIC,
	DXCONTROL_CHECKBOX,
	DXCONTROL_RADIOBUTTON,
	DXCONTROL_COMBOBOX,
	DXCONTROL_SLIDER,
	DXCONTROL_EDITBOX,
	DXCONTROL_IMEEDITBOX,
	DXCONTROL_LISTBOX,
	DXCONTROL_SCROLLBAR,
};

enum DXCONTROL_STATE
{
	DXSTATE_NORMAL = 0,
	DXSTATE_DISABLED,
	DXSTATE_HIDDEN,
	DXSTATE_FOCUS,
	DXSTATE_MOUSEOVER,
	DXSTATE_PRESSED,
};

struct DXFontData
{
	WCHAR szFaceName[MAX_PATH];
	LONG nHeight;
	LONG nWeight;
	LPD3DXFONT pFont;
};

struct DXElement
{
	int iFont;
	DWORD dwFormat;
	D3DXCOLOR TextColor;
	DXElement(int font = 0, DWORD format = DT_CENTER | DT_VCENTER, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) :
		iFont(font), dwFormat(format), TextColor(color) {};
};

class DXDialogResourceManager
{
private:
	IDirect3DDevice9 *m_pd3dDevice;

	std::vector<DXDialog*> m_pDialogs;
	std::vector<DXFontData*> m_pFonts;

public:
	DXDialogResourceManager();
	~DXDialogResourceManager();

	int AddFont(LPCWSTR szFaceName, LONG height, LONG weight);
	void SetFont(int iFont);
	DXFontData *GetFont(int iFont);

	void RegisterDialog(DXDialog *pDialog, int IDD);
	void UnregisterDialog(DXDialog *pDialog);
	void UnregisterDialog(int IDD);
	HRESULT OnCreateDevice();
	HRESULT OnResetDevice();
	HRESULT OnLostDevice();

	DXDialog* GetDialog(int IDD);
};

class DXDialog
{
	friend class DXDialogResourceManager;

protected:
	DXDialogResourceManager *m_pManager;
	int m_ID;
	int m_x;
	int m_y;
	int m_Width;
	int m_Height;
	int m_CaptionHeight;

	DXControl *m_pFocus;
	DXControl *m_pPressed;
	std::vector<DXControl*> m_pControls;
	std::vector<int> m_pFonts;

	Sound *m_pSoundFocus;
	Sound *m_pSoundPress;
	Sound *m_pSoundCancel;

public:
	DXDialog();
	virtual ~DXDialog();
	virtual void Init() {};
	virtual void Default();

	void SetFont(int iFont, LPWSTR szFaceName, LONG height, LONG weight);
	DXFontData* GetFont(int iFont);

	void AddControl(DXControl* pControl);
	DXStatic* AddStatic(int ID, int x, int y, int width, int height, LPWSTR szText);
	DXButton* AddButton(int ID, int x, int y, int width, int height, LPWSTR szText);

	void SetNormal(DXControl* pControl);
	void SetNormal(int ID);
	void SetDisabled(DXControl* pControl);
	void SetDisabled(int ID);
	void SetHidden(DXControl* pControl);
	void SetHidden(int ID);
	bool SetFocus(DXControl* pControl);
	void SetFocus(int ID);
	DXControl* GetFocus();
	DXControl* PrevFocus();
	DXControl* NextFocus();
	void SetPressed(DXControl* pControl);
	void SetPressed(int ID);
	DXControl* GetPressed();

	void Render();
	void DrawControlText(LPWSTR szText, RECT *prcRect, DXElement *pElement = NULL);
	void PlayFocusSound();
	void PlayPressSound();
	void PlayCancelSound();

	virtual void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

class DXControl
{
protected:
	DXDialog *m_pDlgParent;
	DXCONTROL_TYPE m_Type;
	DXCONTROL_STATE m_State;

	int m_ID;
	int m_x, m_y;
	int m_Width, m_Height;
	RECT m_rectBound;

	WCHAR m_szText[MAX_PATH];
	std::vector<DXElement*> m_pElements;

public:
	DXControl();
	virtual ~DXControl();

	DXCONTROL_TYPE GetType() { return m_Type; }
	DXCONTROL_STATE GetState() { return m_State; }
	virtual BOOL SetState(DXCONTROL_STATE state) { m_State = state; return TRUE; }

	void SetID(int ID) { m_ID = ID; }
	int GetID() { return m_ID; }

	void SetLocation(int x, int y)
	{
		m_x = x; m_y = y; UpdateRect();
	}

	void SetSize(int width, int height)
	{
		m_Width = width; m_Height = height; UpdateRect();
	}

	void UpdateRect();

	void SetParentDialog(DXDialog*);

	void SetText(LPWSTR szText)
	{
		wcscpy_s(m_szText, MAX_PATH, szText);
	}

	void AddElement(DXElement *pElement);
	void AddElement(DXElement Element);
	void AddElement(int iFont, DWORD dwFormat, D3DXCOLOR TextColor);

	virtual void Render() {};
};

class DXStatic : public DXControl
{
public:
	DXStatic();
	~DXStatic();

	BOOL SetState(DXCONTROL_STATE state);
	void Render();
};

class DXButton :public DXControl
{
public:
	DXButton();
	~DXButton();

	BOOL SetState(DXCONTROL_STATE state);
	void Render();
};

#endif // !DXGUI_H
