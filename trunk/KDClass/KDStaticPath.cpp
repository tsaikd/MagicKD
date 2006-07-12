#include "stdafx.h"
#include "KDStaticPath.h"

IMPLEMENT_DYNAMIC(CKDStaticPath, CStatic)

CKDStaticPath::CKDStaticPath()
	:	m_pttc(NULL)
{
}

CKDStaticPath::~CKDStaticPath()
{
}

bool CKDStaticPath::SignToolTipCtrl(CToolTipCtrl *ttc)
{
	if (!IsWindow(ttc->GetSafeHwnd()))
		return false;

	CString sBuf;
	GetWindowText(sBuf);
	ModifyStyle(0, SS_NOTIFY);
	m_pttc = ttc;
	if (sBuf.IsEmpty())
		m_pttc->AddTool(this);
	else
		m_pttc->AddTool(this, sBuf);
	return true;
}

void CKDStaticPath::UnSignToolTipCtrl()
{
	if (!m_pttc || !IsWindow(m_pttc->GetSafeHwnd()))
		return;

	m_pttc->DelTool(this);
	m_pttc = NULL;
}

BEGIN_MESSAGE_MAP(CKDStaticPath, CStatic)
END_MESSAGE_MAP()

LRESULT CKDStaticPath::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SETTEXT:
		if (m_pttc && IsWindow(m_pttc->GetSafeHwnd()))
			m_pttc->UpdateTipText((LPCTSTR) lParam, this);
		break;
	}

	return CStatic::DefWindowProc(message, wParam, lParam);
}
