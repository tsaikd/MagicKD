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

	m_pttc = ttc;
	return true;
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
