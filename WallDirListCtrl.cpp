#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallDirListCtrl.h"
#include ".\walldirlistctrl.h"

CWallDirListCtrl::CWallDirListCtrl() :
	m_bInit(false), m_bFindPath(true), m_bAllItemEnable(false)
{
}

CWallDirListCtrl::~CWallDirListCtrl()
{
}

void CWallDirListCtrl::SaveIni()
{
	if (!m_bInit || !IsIniModify())
		return;

	int iCount = GetItemCount();
	if (iCount) {
		CStringArray saDirList;
		for (int i=0 ; i<iCount ; i++)
			saDirList.Add(GetItemText(i, 0));
		m_pIni->WriteArray(_T("DirList"), GetListClassName(), &saDirList);
	} else {
		m_pIni->DeleteKey(_T("DirList"), GetListClassName());
	}

	CWallListCtrl::SaveIni();
}

void CWallDirListCtrl::Init(CIni *pIni, LPCTSTR sListClassName)
{
	CWallListCtrl::Init(pIni);

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, CResString(IDS_WALL_COLUMN_DIRPATH), LVCFMT_LEFT, rcWin.right - 50);
	InsertColumn(1, CResString(IDS_WALL_COLUMN_PICFINDNUM), LVCFMT_CENTER, 50);
	SetListClassName(sListClassName);

	CStringArray saDirList;
	m_pIni->GetArray(_T("DirList"), GetListClassName(), &saDirList);
	int iCount = saDirList.GetCount();
	if (iCount) {
		for (int i=0 ; i<iCount ; i++) {
			AddItem(saDirList[i]);
		}
	}

	SetToolTips(CResString(IDS_WALL_TOOLTIP_DIRLIST));
	ShowWindow(SW_HIDE);

	m_bInit = true;
}

void CWallDirListCtrl::SetListClassName(LPCTSTR sName)
{
	m_sListClassName = sName;
}

CString CWallDirListCtrl::GetListClassName()
{
	return m_sListClassName;
}

bool CWallDirListCtrl::AddItem(LPCTSTR sDirPath)
{
	CWallDirListItem *pItem = new CWallDirListItem(m_hWnd, m_pIni, sDirPath);
	if (-1 == InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0,
	I_IMAGECALLBACK, (LPARAM) pItem)) {
		delete pItem;
		return false;
	}
	if (pItem->IsFindPath()) {
		SetAllItemEnable(IsAllItemEnable());
	} else {
		SetAllItemEnable(false);
		m_bFindPath = false;
	}
	return true;
}

void CWallDirListCtrl::UpdateAllItemFileFindNum()
{
	CWallDirListItem *pItem;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallDirListItem *)GetItemData(i)) {
			pItem->UpdateItemFileFindNum();
		}
	}
}

void CWallDirListCtrl::UpdateSelectItemFileFindNum()
{
	CWallDirListItem *pItem;
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		pItem = (CWallDirListItem *)GetItemData(nItem);
		if (pItem) {
			pItem->SetItemFileFindNum(-1);
			pItem->UpdateItemFileFindNum();
		}
	}
}

bool CWallDirListCtrl::IsFindPath()
{
	return m_bFindPath;
}

bool CWallDirListCtrl::IsAllItemEnable()
{
	return m_bAllItemEnable;
}

void CWallDirListCtrl::SetAllItemEnable(bool bEnable)
{
	bool bFindPath = true;
	m_bAllItemEnable = bEnable;

	CWallDirListItem *pItem;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallDirListItem *)GetItemData(i)) {
			pItem->SetItemEnable(bEnable);
			bFindPath = bFindPath && pItem->IsFindPath();
		}
	}
	m_bFindPath = bFindPath;
}

void CWallDirListCtrl::RemoveFromPath(CString &sPath)
{
	CWallDirListItem *pItem;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallDirListItem *)GetItemData(i)) {
			pItem->RemoveAllPath(sPath);
		}
	}
}

BEGIN_MESSAGE_MAP(CWallDirListCtrl, CWallListCtrl)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void CWallDirListCtrl::OnDestroy()
{
	SaveIni();
	CWallListCtrl::OnDestroy();

	// TODO: �b���[�J�z���T���B�z�`���{���X
}

void CWallDirListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_ADDDIRPATH, GetResString(IDS_WALL_MENU_ADDDIRPATH));
	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_UPDATEDIRFILEFIND, GetResString(IDS_WALL_MENU_UPDATEDIRFILEFIND));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_DELDIRPATH, GetResString(IDS_WALL_MENU_DELDIRPATH));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
	// TODO: �b���[�J�z���T���B�z�`���{���X
}

void CWallDirListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallDirListItem *pItem = (CWallDirListItem *)pNMLV->lParam;
	if (pItem)
		delete pItem;

	// TODO: �b���[�J����i���B�z�`���{���X
	*pResult = 0;
}

void CWallDirListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWallListCtrl::OnSetFocus(pOldWnd);

//	Invalidate();

	// TODO: �b���[�J�z���T���B�z�`���{���X
}
