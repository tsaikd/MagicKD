#include "StdAfx.h"
#include "WallListBox.h"

CWallListBox::CWallListBox(void)
{
}

CWallListBox::~CWallListBox(void)
{
}
BEGIN_MESSAGE_MAP(CWallListBox, CKDListBox)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CWallListBox::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	CPoint cpPopMenu;
	GetCursorPos(&cpPopMenu);
	m_mContextMenu.CreatePopupMenu();
	switch (GetDlgCtrlID()) {
	case IDC_WALLENABLECLASS:
		m_mContextMenu.AppendMenu(MF_STRING, IDS_ABOUTBOX, GetResString(IDS_ABOUTBOX));
		m_mContextMenu.InsertMenu(-1, MF_STRING, 1, _T("BB"));
		break;
	case IDC_WALLCLASSLIST:
		m_mContextMenu.InsertMenu(-1, MF_STRING | MF_BYPOSITION, IDS_ABOUTBOX, _T("CC"));
		m_mContextMenu.InsertMenu(-1, MF_STRING | MF_BYPOSITION, IDS_ABOUTBOX, _T("DD"));
		break;
	case IDC_WALLDIRLIST:
		m_mContextMenu.InsertMenu(-1, MF_STRING | MF_BYPOSITION, IDS_ABOUTBOX, _T("EE"));
		m_mContextMenu.InsertMenu(-1, MF_STRING | MF_BYPOSITION, IDS_ABOUTBOX, _T("FF"));
		break;
	default:
		break;
	}
	if (m_mContextMenu.GetMenuItemCount())
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	m_mContextMenu.DestroyMenu();
}

LRESULT CWallListBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			if (nID == IDS_ABOUTBOX)
				wParam = wParam;
		}
		break;
	default:
		break;
	}

	return CKDListBox::DefWindowProc(message, wParam, lParam);
}
