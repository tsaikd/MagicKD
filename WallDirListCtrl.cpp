#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallDirListItem.h"

#include "WallDirListCtrl.h"

CWallDirListCtrl::CWallDirListCtrl()
	:	m_bInit(false), m_bAllItemEnable(false), m_pbShowDirLoadError(NULL)
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

void CWallDirListCtrl::Init(CIni *pIni, LPCTSTR sListClassName, bool *pbShowDirLoadError)
{
	CWallListCtrl::Init(pIni);

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, CResString(IDS_WALL_COLUMN_DIRPATH), LVCFMT_LEFT, rcWin.right - 50);
	InsertColumn(1, CResString(IDS_WALL_COLUMN_PICFINDNUM), LVCFMT_CENTER, 50);
	SetListClassName(sListClassName);
	EnableDrag();

	m_pbShowDirLoadError = pbShowDirLoadError;

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
	CWallDirListItem *pItem = new CWallDirListItem;
	pItem->Init(m_hWnd, m_pIni, sDirPath, m_pbShowDirLoadError);
	if (-1 == InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0,
	I_IMAGECALLBACK, (LPARAM) pItem)) {
		delete pItem;
		return false;
	}
	SetAllItemEnable(IsAllItemEnable());
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

bool CWallDirListCtrl::IsAllItemEnable()
{
	return m_bAllItemEnable;
}

void CWallDirListCtrl::SetAllItemEnable(bool bEnable)
{
	m_bAllItemEnable = bEnable;

	CWallDirListItem *pItem;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallDirListItem *)GetItemData(i)) {
			pItem->SetItemEnable(bEnable);
		}
	}
}

void CWallDirListCtrl::SetOnExit(bool bOnExit/* = true*/)
{
	CWallDirListItem *pItem;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallDirListItem *)GetItemData(i)) {
			pItem->SetOnExit(bOnExit);
		}
	}
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
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CWallDirListCtrl::OnDestroy()
{
	SaveIni();
	CWallListCtrl::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallDirListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_ADDDIRPATH, CResString(IDS_WALL_MENU_ADDDIRPATH));
	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_UPDATEDIRFILEFIND, CResString(IDS_WALL_MENU_UPDATEDIRFILEFIND));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_DELDIRPATH, CResString(IDS_WALL_MENU_DELDIRPATH));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallDirListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallDirListItem *pItem = (CWallDirListItem *)pNMLV->lParam;
	if (pItem)
		delete pItem;

	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}

void CWallDirListCtrl::OnDropFiles(HDROP hDropInfo)
{
	TCHAR sFilePath[MAX_PATH];
	UINT i, uCount = DragQueryFile(hDropInfo, UINT_MAX, NULL, 0);

	for (i = 0;i< uCount;i++) {
		DragQueryFile(hDropInfo, i, sFilePath, MAX_PATH);

		if (PathIsDirectory(sFilePath)) {
			AddItem(sFilePath);
			SetIniModify();
		}
	}

	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CWallListCtrl::OnDropFiles(hDropInfo);
}
