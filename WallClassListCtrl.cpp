#include "StdAfx.h"
#include "WallChanger.h"
#include "WallClassListCtrl.h"

CWallClassListCtrl::CWallClassListCtrl() : m_iIDC_WALLDIRLISTBASE(200)
{
}

CWallClassListCtrl::~CWallClassListCtrl()
{
}

void CWallClassListCtrl::Init()
{
	CWallListCtrl::Init();
	InsertColumn(0, _T(""));

	m_bInit = true;
}

bool CWallClassListCtrl::AddItem(LPCTSTR sName)
{
	if (!m_bInit)
		return false;

	CWallDirListCtrl *pChildList = new CWallDirListCtrl;
	if (! pChildList->CreateEx(WS_EX_CLIENTEDGE, WS_CHILD | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS,
	CRect(), GetParent(), m_iIDC_WALLDIRLISTBASE++)) {
		delete pChildList;
		return false;
	}
	pChildList->ShowWindow(SW_HIDE);
	pChildList->Init();

	CWallClassListItem *pItem = new CWallClassListItem;
	pItem->SetItemName(sName);
	pItem->SetChildDirList(pChildList);
	if (-1 == InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0,
	I_IMAGECALLBACK, (LPARAM) pItem)) {
		delete pItem;
		delete pChildList;
		return false;
	}
	return true;
}

BEGIN_MESSAGE_MAP(CWallClassListCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

void CWallClassListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDENCLASS, GetResString(IDS_WALLADDENCLASS));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSLIST, GetResString(IDS_WALLDELCLASSLIST));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

LRESULT CWallClassListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_WALLADDENCLASS:
				((CWallChanger *)GetParent())->AddClassToEnable();
				break;
			case IDS_WALLDELCLASSLIST:
				((CWallChanger *)GetParent())->DelClassList();
				break;
			}
		}
		break;
	}

	return CWallListCtrl::DefWindowProc(message, wParam, lParam);
}

void CWallClassListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
		CWallClassListItem *pItem = (CWallClassListItem *) pDispInfo->item.lParam;
		switch (pDispInfo->item.iSubItem) {
			case 0:
				_stprintf(sText, "%s", pItem->GetItemName());
				pDispInfo->item.pszText = sText;
				break;
		}
	}

	*pResult = 0;
}

void CWallClassListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallClassListItem *pItem = (CWallClassListItem *) GetItemData(pNMLV->iItem);
	CWallDirListCtrl *pChildList;
	if (pItem) {
		pChildList = pItem->GetChildDirList();
		if (pChildList)
			delete pChildList;
		delete pItem;
	}

	*pResult = 0;
}
