#include "StdAfx.h"
#include "KDEdit.h"

CKDEdit::CKDEdit()
	:	m_bEnableToolTip(false)
{
}

CKDEdit::~CKDEdit()
{
}

bool CKDEdit::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, bool bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CEdit::EnableToolTips(bEnable) != FALSE;
}

bool CKDEdit::IsEnableToolTips()
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
	return CEdit::OnToolHitTest(point, pTI);
}
