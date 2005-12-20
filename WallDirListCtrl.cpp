#include "StdAfx.h"
#include "WallChanger.h"
#include "WallDirListCtrl.h"

CWallDirListCtrl::CWallDirListCtrl() : m_bInit(false)
{
}

CWallDirListCtrl::~CWallDirListCtrl()
{
}

void CWallDirListCtrl::Init(void *pParent)
{
	CWallListCtrl::Init(pParent);
	InsertColumn(0, CResString(IDS_WALL_DIRLISTCOLUMN0), LVCFMT_LEFT, 200, 0);
	InsertColumn(1, CResString(IDS_WALL_DIRLISTCOLUMN1), LVCFMT_LEFT, 50, 1);

	m_bInit = true;
}

void CWallDirListCtrl::AddItem(int iClassNum, LPCTSTR sPath, int iFindNum/* = 0*/)
{
	if (!m_bInit)
		return;
	CWallDirListItem *pItem = new CWallDirListItem;
	pItem->SetClassNum(iClassNum);
	pItem->SetPath(sPath);
	pItem->SetFindNum(iFindNum);
	InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM) pItem);
//	if (m_aryDirListItem.GetUpperBound() < iClassNum) {
//		CList<CWallDirListItem*> listItem;
//		m_aryDirListItem.Add(listItem);
//	}
}

BEGIN_MESSAGE_MAP(CWallDirListCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

void CWallDirListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDCLASSDIR, GetResString(IDS_WALLADDCLASSDIR));
	if (pos)
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSDIR, GetResString(IDS_WALLDELCLASSDIR));

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

LRESULT CWallDirListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_WALLADDCLASSDIR:
				((CWallChanger *)m_pParent)->AddClassDir();
				break;
			case IDS_WALLDELCLASSDIR:
				((CWallChanger *)m_pParent)->DelClassDir();
				break;
			}
		}
		break;
	}

	return CWallListCtrl::DefWindowProc(message, wParam, lParam);
}

void CWallDirListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
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

	*pResult = 0;
}

void CWallDirListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallDirListItem *pItem = (CWallDirListItem *) GetItemData(pNMLV->iItem);
	if (pItem)
		delete pItem;

	*pResult = 0;
}
