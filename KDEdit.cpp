#include "StdAfx.h"
#include "KDEdit.h"

CKDEdit::CKDEdit()
	: m_bEnableToolTip(FALSE)
{
}

CKDEdit::~CKDEdit()
{
}

BOOL CKDEdit::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CEdit::EnableToolTips(bEnable);
}

BOOL CKDEdit::IsEnableToolTips()
{
	return m_bEnableToolTip;
}

void CKDEdit::SetToolTips(LPCTSTR sToolTip)
{
	if (sToolTip)
		m_sToolTip = sToolTip;
}

INT_PTR CKDEdit::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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
