#include "StdAfx.h"
#include "Others.h"
#include "WallConf.h"
#include "WallListCtrl.h"

CWallListCtrl::CWallListCtrl() : m_pIni(NULL), m_bInit(false)
{
}

CWallListCtrl::~CWallListCtrl()
{
}

void CWallListCtrl::SaveIni()
{
	if (!m_bInit)
		return;

	CKDIni::SaveIni();
}

void CWallListCtrl::Init(CIni *pIni)
{
	m_pIni = pIni;
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	::g_pWallConf->AddSaveConf(this);

	m_bInit = true;
}

void CWallListCtrl::OnDestroy()
{
	__super::OnDestroy();
	::g_pWallConf->RemoveSaveConf(this);
}

BEGIN_MESSAGE_MAP(CWallListCtrl, CKDListCtrl)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

LRESULT CWallListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_LBUTTONUP:
		if (m_bOnDraging) {
			CPoint pt(lParam);
			int iPos = QueryDragPos(pt); // return the target position
			int iMaxPos = GetItemCount();
			if (!INRANGE(m_nmlvBeginDrag.iItem, 0, iMaxPos) || !INRANGE(iPos, 0, iMaxPos))
				break;

			MoveSelectedItems(iPos);
			SetIniModify();
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
