#include "StdAfx.h"
#include "WallChanger.h"
#include "WallEnableCtrl.h"
#include "WallDirListCtrl.h"

CWallEnableCtrl::CWallEnableCtrl()
{
}

CWallEnableCtrl::~CWallEnableCtrl()
{
}

void CWallEnableCtrl::Init(void *pParent)
{
	CWallListCtrl::Init(pParent);
	InsertColumn(0, _T(""));

	m_bInit = true;
}

void CWallEnableCtrl::AddItem(LPCTSTR sName, void *pChildList)
{
	if (!m_bInit)
		return;

	CWallEnableItem *pItem = new CWallEnableItem;
	pItem->SetItemName(sName);
	pItem->SetChildDirList((CWallDirListCtrl *)pChildList);
	InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM) pItem);
}

BEGIN_MESSAGE_MAP(CWallEnableCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

void CWallEnableCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	if (pos)
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELENCLASS, GetResString(IDS_WALLDELENCLASS));

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

LRESULT CWallEnableCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_WALLDELENCLASS:
				((CWallChanger *)m_pParent)->DelEnableClass();
				break;
			}
		}
		break;
	}

	return CWallListCtrl::DefWindowProc(message, wParam, lParam);
}

void CWallEnableCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
		CWallEnableItem *pItem = (CWallEnableItem *) pDispInfo->item.lParam;
		switch (pDispInfo->item.iSubItem) {
			case 0:
				_stprintf(sText, "%s", pItem->GetItemName());
				pDispInfo->item.pszText = sText;
				break;
		}
	}

	*pResult = 0;
}

void CWallEnableCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallEnableItem *pItem = (CWallEnableItem *) GetItemData(pNMLV->iItem);
	if (pItem) {
		delete pItem;
	}

	*pResult = 0;
}
