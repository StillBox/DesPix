#include "DXGUI.h"

//
//DXDialogResourceManager
//

DXDialogResourceManager::DXDialogResourceManager()
{
	m_pd3dDevice = NULL;
}

DXDialogResourceManager::~DXDialogResourceManager()
{
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
	}
	std::vector<DXFontData*>().swap(m_pFonts);
	std::vector<DXDialog*>().swap(m_pDialogs);
}

int DXDialogResourceManager::AddFont(LPCWSTR szFaceName, LONG height, LONG weight)
{
	for (unsigned int i = 0; i < m_pFonts.size(); i++)
	{
		DXFontData *pFont = m_pFonts[i];
		auto len = wcslen(pFont->szFaceName);
		if (0 == _wcsnicmp(szFaceName, pFont->szFaceName, len)
			&& height == pFont->nHeight && weight == pFont->nWeight)
			return i;
	}

	DXFontData *pNewFont = new DXFontData();
	ZeroMemory(pNewFont, sizeof(DXFontData));
	wcscpy_s(pNewFont->szFaceName, MAX_PATH, szFaceName);
	pNewFont->nHeight = height;
	pNewFont->nWeight = weight;
	m_pFonts.push_back(pNewFont);
	int i = m_pFonts.size() - 1;
	if (m_pd3dDevice)
		SetFont(i);
	return i;
}

void DXDialogResourceManager::SetFont(int iFont)
{
	SAFE_RELEASE(m_pFonts[iFont]->pFont);
	D3DXCreateFont(m_pd3dDevice, m_pFonts[iFont]->nHeight, 0, m_pFonts[iFont]->nWeight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		m_pFonts[iFont]->szFaceName, &m_pFonts[iFont]->pFont);
}

DXFontData* DXDialogResourceManager::GetFont(int iFont)
{
	return m_pFonts[iFont];
}

void DXDialogResourceManager::RegisterDialog(DXDialog *pDialog, int IDD)
{
	m_pDialogs.push_back(pDialog);
	pDialog->m_pManager = this;
	pDialog->m_ID = IDD;
}

void DXDialogResourceManager::UnregisterDialog(DXDialog *pDialog)
{
	auto iter = m_pDialogs.begin();
	while (iter!= m_pDialogs.end())
	{
		if (*iter == pDialog)
		{
			m_pDialogs.erase(iter);
			break;
		}
		iter++;
	}
}

void DXDialogResourceManager::UnregisterDialog(int IDD)
{
	auto iter = m_pDialogs.begin();
	while (iter != m_pDialogs.end())
	{
		if ((*iter)->m_ID == IDD)
		{
			m_pDialogs.erase(iter);
			break;
		}
		iter++;
	}
}

HRESULT DXDialogResourceManager::OnCreateDevice()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	for (auto p : m_pFonts)
	{
		if (NULL != p)
		{
			SAFE_RELEASE(p->pFont);
			D3DXCreateFont(m_pd3dDevice, p->nHeight, 0, p->nWeight, 1, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
				p->szFaceName, &p->pFont);
		}
	}
	return S_OK;
}

HRESULT DXDialogResourceManager::OnResetDevice()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
		D3DXCreateFont(m_pd3dDevice, p->nHeight, 0, p->nWeight, 1, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			p->szFaceName, &p->pFont);
	}
	return S_OK;
}

HRESULT DXDialogResourceManager::OnLostDevice()
{
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
	}
	return S_OK;
}

DXDialog* DXDialogResourceManager::GetDialog(int IDD)
{
	for (auto p : m_pDialogs)
	{
		if (IDD == p->m_ID)
			return p;
	}
	return NULL;
}


//
//DXDialog
//

DXDialog::DXDialog()
{
	m_pManager = NULL;
	m_x = 0;
	m_y = 0;
	m_Width = 0;
	m_Height = 0;
	m_CaptionHeight = 18;

	m_pFocus = NULL;
	m_pPressed = NULL;

	m_pSoundFocus = NULL;
	m_pSoundPress = NULL;
	m_pSoundCancel = NULL;
}

DXDialog::~DXDialog()
{
	std::vector<DXControl*>().swap(m_pControls);
	std::vector<int>().swap(m_pFonts);
}

void DXDialog::Default()
{
	for (auto p : m_pControls)
		p->SetState(DXSTATE_NORMAL);
	for (auto p : m_pControls)
		if (p->SetState(DXSTATE_FOCUS))
		{
			m_pFocus = p;
			break;
		}
}

void DXDialog::SetFont(int iFont, LPWSTR szFaceName, LONG height, LONG weight)
{
	auto size = m_pFonts.size();
	int iFontManager = m_pManager->AddFont(szFaceName, height, weight);

	if (iFont >= (int)size)
	{
		for (int i = size; i < iFont; i++)
		{
			m_pFonts.push_back(-1);
		}
		m_pFonts.push_back(iFontManager);
	}
	else
		m_pFonts[iFont] = iFontManager;
}

DXFontData* DXDialog::GetFont(int iFont)
{
	if (NULL == m_pManager)
		return NULL;
	else
		return m_pManager->GetFont(iFont);
}

void DXDialog::AddControl(DXControl *pControl)
{
	m_pControls.push_back(pControl);
	pControl->SetParentDialog(this);
	if (NULL == m_pFocus)
		SetFocus(pControl);
}

DXStatic* DXDialog::AddStatic(int ID, int x, int y, int width, int height, LPWSTR szText)
{
	DXStatic *pNewStatic = new DXStatic();
	AddControl(pNewStatic);

	pNewStatic->SetID(ID);
	pNewStatic->SetLocation(x, y);
	pNewStatic->SetSize(width, height);
	pNewStatic->UpdateRect();
	pNewStatic->SetText(szText);

	return pNewStatic;
}

DXButton* DXDialog::AddButton(int ID, int x, int y, int width, int height, LPWSTR szText)
{
	DXButton *pNewStatic = new DXButton();
	AddControl(pNewStatic);

	pNewStatic->SetID(ID);
	pNewStatic->SetLocation(x, y);
	pNewStatic->SetSize(width, height);
	pNewStatic->UpdateRect();
	pNewStatic->SetText(szText);

	return pNewStatic;
}

void DXDialog::SetNormal(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_NORMAL);
}

void DXDialog::SetNormal(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetNormal(p);
			break;
		}
	}
}

void DXDialog::SetDisabled(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_DISABLED);
}

void DXDialog::SetDisabled(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetDisabled(p);
			break;
		}
	}
}

void DXDialog::SetHidden(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_HIDDEN);

}

void DXDialog::SetHidden(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetHidden(p);
			break;
		}
	}
}

bool DXDialog::SetFocus(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (NULL != m_pFocus)
		m_pFocus->SetState(DXSTATE_NORMAL);

	if (pControl->SetState(DXSTATE_FOCUS))
	{
		m_pFocus = pControl;
		return true;
	}
	else
	{
		m_pFocus = NULL;
		return false;
	}
}

void DXDialog::SetFocus(int ID)
{
	m_pFocus->SetState(DXSTATE_NORMAL);
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetFocus(p);
			break;
		}
	}
}

DXControl* DXDialog::GetFocus()
{
	return m_pFocus;
}

DXControl* DXDialog::PrevFocus()
{
	DXControl *pNewFocus = NULL;
	auto iter = find(m_pControls.begin(), m_pControls.end(), m_pFocus);
	while (iter != m_pControls.begin())
	{
		iter--;
		if ((*iter)->SetState(DXSTATE_FOCUS))
		{
			pNewFocus = *iter;
			break;
		}
	}
	if (pNewFocus != NULL)
		SetFocus(pNewFocus);

	return pNewFocus;
}

DXControl* DXDialog::NextFocus()
{
	DXControl *pNewFocus = NULL;
	auto iter = find(m_pControls.begin(), m_pControls.end(), m_pFocus);
	iter++;
	while (iter != m_pControls.end())
	{
		if ((*iter)->SetState(DXSTATE_FOCUS))
		{
			pNewFocus = *iter;
			break;
		}
		iter++;
	}
	if (pNewFocus != NULL)
		SetFocus(pNewFocus);

	return pNewFocus;
}

void DXDialog::SetPressed(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl->SetState(DXSTATE_PRESSED))
		m_pPressed = pControl;
	else
		m_pPressed = NULL;

}

void DXDialog::SetPressed(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
			SetPressed(p);
	}
}

DXControl* DXDialog::GetPressed()
{
	DXControl *pReturn = m_pPressed;
	if (NULL != m_pPressed)
	{
		m_pPressed->SetState(DXSTATE_FOCUS);
		m_pPressed = NULL;
	}
	return pReturn;
}

void DXDialog::Render()
{
	for (auto p : m_pControls)
	{
		p->Render();
	}
}

void DXDialog::DrawControlText(LPWSTR szText, RECT *prcRect, DXElement *pElement)
{
	RECT destRect = *prcRect;
	//	OffsetRect(&destRect, m_x, m_y);
	//	if (m_CaptionHeight)
	//		OffsetRect(&destRect, 0, m_CaptionHeight);

	int iFont = 0;
	DWORD dwFormat = DT_CENTER | DT_VCENTER;
	D3DXCOLOR TextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (NULL != pElement)
	{
		iFont = pElement->iFont;
		dwFormat = pElement->dwFormat;
		TextColor = pElement->TextColor;
	}

	DXFontData *pFontData = GetFont(m_pFonts[iFont]);
	pFontData->pFont->DrawText(NULL, szText, -1, &destRect, dwFormat, TextColor);
}

void DXDialog::PlayFocusSound()
{
	if (NULL != m_pSoundFocus)
		m_pSoundFocus->Play(0);
}

void DXDialog::PlayPressSound()
{
	if (NULL != m_pSoundPress)
		m_pSoundPress->Play(0);
}

void DXDialog::PlayCancelSound()
{
	if (NULL != m_pSoundCancel)
		m_pSoundCancel->Play(0);
}

void DXDialog::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			PrevFocus();
			PlayFocusSound();
			break;
		case VK_DOWN:
			NextFocus();
			PlayFocusSound();
			break;
		case VK_RETURN:
			SetPressed(m_pFocus);
			PlayPressSound();
			break;
		default:
			break;
		}
	default:
		break;
	}
}

//
//DXControl
//

DXControl::DXControl()
{
	m_ID = -1;
	m_x = 0;
	m_y = 0;
	m_Width = 0;
	m_Height = 0;
	m_pDlgParent = NULL;
}

DXControl::~DXControl()
{
}

void DXControl::UpdateRect()
{
	SetRect(&m_rectBound, m_x, m_y, m_x + m_Width, m_y + m_Height);
}

void DXControl::SetParentDialog(DXDialog *pDialog)
{
	m_pDlgParent = pDialog;
}

void DXControl::AddElement(DXElement *pElement)
{
	m_pElements.push_back(pElement);
}

void DXControl::AddElement(DXElement Element)
{
	DXElement *pNewElement = new DXElement();
	pNewElement->iFont = Element.iFont;
	pNewElement->dwFormat = Element.dwFormat;
	pNewElement->TextColor = Element.TextColor;

	m_pElements.push_back(pNewElement);
}

void DXControl::AddElement(int iFont, DWORD dwFormat, D3DXCOLOR TextColor)
{
	DXElement *pNewElement = new DXElement();
	pNewElement->iFont = iFont;
	pNewElement->dwFormat = dwFormat;
	pNewElement->TextColor = TextColor;

	m_pElements.push_back(pNewElement);
}

//
//DXStatic
//

DXStatic::DXStatic()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_STATIC;
	m_State = DXSTATE_NORMAL;
}

DXStatic::~DXStatic()
{
}

BOOL DXStatic::SetState(DXCONTROL_STATE state)
{
	if (state == DXSTATE_FOCUS ||
		state == DXSTATE_MOUSEOVER ||
		state == DXSTATE_PRESSED)
		return FALSE;
	else
	{
		m_State = state;
		return TRUE;
	}
}

void DXStatic::Render()
{
	if (m_szText)
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound,
			m_pElements.size() > 0 ? m_pElements[0] : NULL);
}

//
//DXButton
//

DXButton::DXButton()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_BUTTON;
	m_State = DXSTATE_NORMAL;
}

DXButton::~DXButton()
{
}

BOOL DXButton::SetState(DXCONTROL_STATE state)
{
	if (DXSTATE_DISABLED == m_State || DXSTATE_HIDDEN == m_State)
	{
		if (DXSTATE_NORMAL == state)
		{
			m_State = state;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		m_State = state;
		return TRUE;
	}

}

void DXButton::Render()
{
	if (m_szText)
	{
		switch (m_State)
		{
		case DXSTATE_NORMAL:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[0]);
			break;
		case DXSTATE_DISABLED:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[2]);
			break;
		case DXSTATE_HIDDEN:
			break;
		case DXSTATE_FOCUS:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);
			break;
		case DXSTATE_MOUSEOVER:
			break;
		case DXSTATE_PRESSED:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);
			break;
		default:
			break;
		}
	}
}