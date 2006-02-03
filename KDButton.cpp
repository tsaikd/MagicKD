#include "StdAfx.h"
#include "KDButton.h"

CKDButton::CKDButton()
	: m_bEnableToolTip(FALSE)
{
}

CKDButton::~CKDButton()
{
}

BOOL CKDButton::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CButton::EnableToolTips(bEnable);
}

BOOL CKDButton::IsEnableToolTips()
{
	return m_bEnableToolTip;
}

void CKDButton::SetToolTips(LPCTSTR sToolTip)
{
	if (sToolTip)
		m_sToolTip = sToolTip;
}

INT_PTR CKDButton::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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
