#include "StdAfx.h"
#include "WallListBox.h"
#include "WallChanger.h"

CWallListBox::CWallListBox()
{
}

CWallListBox::~CWallListBox()
{
}

BEGIN_MESSAGE_MAP(CWallListBox, CKDListBox)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CWallListBox::Init(void *pParent)
{
	m_pParent = pParent;
}

void CWallListBox::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	int iCusSel = GetCurSel();
	CPoint cpPopMenu;
	GetCursorPos(&cpPopMenu);
	m_mContextMenu.CreatePopupMenu();
	switch (GetDlgCtrlID()) {
	case IDC_WALLENABLECLASS:
		if (iCusSel != LB_ERR)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELENCLASS, GetResString(IDS_WALLDELENCLASS));
		break;
	case IDC_WALLCLASSLIST:
		if (iCusSel != LB_ERR)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDENCLASS, GetResString(IDS_WALLADDENCLASS));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLNEWCLASSLIST, GetResString(IDS_WALLNEWCLASSLIST));
		if (iCusSel != LB_ERR)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSLIST, GetResString(IDS_WALLDELCLASSLIST));
		break;
	case IDC_WALLDIRLIST:
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDCLASSDIR, GetResString(IDS_WALLADDCLASSDIR));
		if (iCusSel != LB_ERR)
			m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSDIR, GetResString(IDS_WALLDELCLASSDIR));
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
			switch (nID) {
			case IDS_WALLADDENCLASS:
				((CWallChanger *)m_pParent)->AddEnableClass();
				break;
			case IDS_WALLDELENCLASS:
				((CWallChanger *)m_pParent)->DelEnableClass();
				break;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			int iItemPos = MouseInItem();
			if (iItemPos != LB_ERR) {
				break;
			}
			break;
			CPoint ptBuf;
			BOOL bOutside;
			int iNearPos;

			iNearPos = ItemFromPoint(ptBuf, bOutside);
			if (bOutside == FALSE) {
				switch (GetDlgCtrlID()) {
				case IDC_WALLCLASSLIST:
					((CWallChanger *)m_pParent)->AddEnableClass();
					break;
				}
			}
		}
		break;
	}

	return CKDListBox::DefWindowProc(message, wParam, lParam);
}
