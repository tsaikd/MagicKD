#include "StdAfx.h"
#include "KDComboBox.h"

CKDComboBox::CKDComboBox()
	: m_bEnableToolTip(FALSE)
{
}

CKDComboBox::~CKDComboBox()
{
}

BOOL CKDComboBox::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CComboBox::EnableToolTips(bEnable);
}

BOOL CKDComboBox::IsEnableToolTips()
{
	return m_bEnableToolTip;
}

void CKDComboBox::SetToolTips(LPCTSTR sToolTip)
{
	if (sToolTip)
		m_sToolTip = sToolTip;
}

INT_PTR CKDComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (pTI && !m_sToolTip.IsEmpty()) {
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT_PTR) m_hWnd;
		pTI->uFlags |= TTF_ALWAYSTIP;
		pTI->lpszText = _tcsdup(m_sToolTip);
		GetClientRect(&pTI->rect);

		return GetDlgCtrlID();
	}
	return -1;
}
