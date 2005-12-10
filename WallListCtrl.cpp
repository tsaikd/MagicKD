// WallListCtrl.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallListCtrl.h"
#include "WallChanger.h"


// CWallListCtrl

IMPLEMENT_DYNAMIC(CWallListCtrl, CListCtrl)
CWallListCtrl::CWallListCtrl() : m_pParent(NULL)
{
}

CWallListCtrl::~CWallListCtrl()
{
}

void CWallListCtrl::Init(void *pParent)
{
	m_pParent = pParent;
}

BEGIN_MESSAGE_MAP(CWallListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CWallListCtrl 訊息處理常式

void CWallListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos;
	CPoint cpPopMenu;

	pos = GetFirstSelectedItemPosition();
	GetCursorPos(&cpPopMenu);
	m_mContextMenu.CreatePopupMenu();

	switch (GetDlgCtrlID()) {
	case IDC_WALLENABLECLASS:
		if (pos)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELENCLASS, GetResString(IDS_WALLDELENCLASS));
		break;
	case IDC_WALLCLASSLIST:
		if (pos) {
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDENCLASS, GetResString(IDS_WALLADDENCLASS));
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSLIST, GetResString(IDS_WALLDELCLASSLIST));
		}
		break;
	case IDC_WALLDIRLIST:
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDCLASSDIR, GetResString(IDS_WALLADDCLASSDIR));
		if (pos)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSDIR, GetResString(IDS_WALLDELCLASSDIR));
		break;
	}

	if (m_mContextMenu.GetMenuItemCount())
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	m_mContextMenu.DestroyMenu();
}

LRESULT CWallListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_WALLADDENCLASS:
				((CWallChanger *)m_pParent)->AddClassToEnable();
				break;
			case IDS_WALLDELENCLASS:
				((CWallChanger *)m_pParent)->DelEnableClass();
				break;
			case IDS_WALLDELCLASSLIST:
				((CWallChanger *)m_pParent)->DelClassList();
				break;
			case IDS_WALLADDCLASSDIR:
				((CWallChanger *)m_pParent)->AddClassDir();
				break;
			case IDS_WALLDELCLASSDIR:
				((CWallChanger *)m_pParent)->DelClassDir();
				break;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			int nID = GetDlgCtrlID();
			switch (nID) {
			case IDC_WALLCLASSLIST:
				((CWallChanger *)m_pParent)->AddClassToEnable();
				break;
			}
		}
		break;
	}

	return CListCtrl::DefWindowProc(message, wParam, lParam);
}
