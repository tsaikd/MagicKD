#include "StdAfx.h"
#include "KDComboBox.h"

CKDComboBox::CKDComboBox()
	:	m_bEnableToolTip(false)
{
}

CKDComboBox::~CKDComboBox()
{
}

bool CKDComboBox::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, bool bEnable/* = true*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CComboBox::EnableToolTips(bEnable) != FALSE;
}

bool CKDComboBox::IsEnableToolTips()
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
	return CComboBox::OnToolHitTest(point, pTI);
}
