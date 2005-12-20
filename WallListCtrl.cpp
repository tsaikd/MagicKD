// WallListCtrl.cpp : ��@��
//

#include "StdAfx.h"
#include "WallChanger.h"
#include "WallListCtrl.h"
#include "WallClassListItem.h"
#include "WallDirListItem.h"


// CWallListCtrl

IMPLEMENT_DYNAMIC(CWallListCtrl, CListCtrl)
CWallListCtrl::CWallListCtrl() : m_pParent(NULL), m_bInit(false)
{
}

CWallListCtrl::~CWallListCtrl()
{
}

void CWallListCtrl::Init(void *pParent)
{
	m_pParent = pParent;
	m_bInit = true;
}

BEGIN_MESSAGE_MAP(CWallListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

// CWallListCtrl �T���B�z�`��

void CWallListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos;
	CPoint cpPopMenu;

/*	pos = GetFirstSelectedItemPosition();
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
	Invalidate();*/
}

LRESULT CWallListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
/*	switch (message) {
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
*/
	return CListCtrl::DefWindowProc(message, wParam, lParam);
}

void CWallListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
/*	TCHAR sText[MAX_PATH] = {0};

	switch (GetDlgCtrlID()) {
	case IDC_WALLDIRLIST:
		if (pDispInfo->item.mask & LVIF_TEXT) {
			CWallDirListItem *pItem = (CWallDirListItem *) pDispInfo->item.lParam;
			switch (pDispInfo->item.iSubItem) {
				case 0:
					_stprintf(sText, "%s", pItem->GetPath());
					pDispInfo->item.pszText = sText;
					break;
				case 1:
					_stprintf(sText, "%d", pItem->GetFindNum());
					pDispInfo->item.pszText = sText;
					break;
			}
		}
		break;
	case IDC_WALLCLASSLIST:
		if (pDispInfo->item.mask & LVIF_TEXT) {
			CWallClassListItem *pItem = (CWallClassListItem *) pDispInfo->item.lParam;
			switch (pDispInfo->item.iSubItem) {
				case 0:
					_stprintf(sText, "%s", pItem->GetItemName());
					pDispInfo->item.pszText = sText;
					break;
			}
		}
		break;
	}
*/
	*pResult = 0;
}

void CWallListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

/*	CWallDirListItem *pItem = (CWallDirListItem *) GetItemData(pNMLV->iItem);
	if (pItem)
		delete pItem;
*/
	*pResult = 0;
}
