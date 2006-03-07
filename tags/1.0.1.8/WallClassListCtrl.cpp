#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "MagicKD.h"
#include "WallConf.h"
#include "WallDirListCtrl.h"

#include "WallClassListCtrl.h"

CWallClassListCtrl::CWallClassListCtrl()
	:	m_bInit(false), m_iIDC_WALLDIRLISTBASE(200), m_pbShowDirLoadError(NULL)
{
}

CWallClassListCtrl::~CWallClassListCtrl()
{
}

void CWallClassListCtrl::SaveIni()
{
	if (!m_bInit || !IsIniModify())
		return;

	int iCount = GetItemCount();
	if (iCount) {
		CStringArray saClassList, saEnableClassList;
		for (int i=0 ; i<iCount ; i++) {
			saClassList.Add(GetItemText(i, 0));
			if (ListView_GetCheckState(m_hWnd, i))
				saEnableClassList.Add(GetItemText(i, 0));
		}
		m_pIni->WriteArray(_T("General"), _T("ClassList"), &saClassList);
		if (saEnableClassList.GetCount())
			m_pIni->WriteArray(_T("General"), _T("EnableClassList"), &saEnableClassList);
		else
			m_pIni->DeleteKey(_T("General"), _T("EnableClassList"));
	} else {
		m_pIni->DeleteKey(_T("General"), _T("ClassList"));
		m_pIni->DeleteKey(_T("General"), _T("EnableClassList"));
	}

	CWallListCtrl::SaveIni();
}

void CWallClassListCtrl::Init(CIni *pIni, CRect &rcChildDirList, bool *pbShowDirLoadError)
{
	g_pTheAppEndDlg->SignWnd(GetSafeHwnd(), 2);

	CWallListCtrl::Init(pIni);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES);
	EnableDrag();

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, CResString(IDS_WALL_COLUMN_CLASSLIST), LVCFMT_LEFT, rcWin.right);
	m_rcChildDirList = rcChildDirList;

	m_pbShowDirLoadError = pbShowDirLoadError;

	CStringArray saClassList;
	m_pIni->GetArray(_T("General"), _T("ClassList"), &saClassList);
	int iCount = saClassList.GetCount();
	if (iCount) {
		for (int i=0 ; i<iCount ; i++) {
			AddItem(saClassList[i]);
		}
	}

	m_pIni->GetArray(_T("General"), _T("EnableClassList"), &saClassList);
	iCount = saClassList.GetCount();
	if (iCount) {
		for (int i=0 ; i<iCount ; i++) {
			int nItem = FindItemByText(saClassList[i]);
			ListView_SetCheckState(m_hWnd, nItem, true);
			SetItemEnable(nItem, true);
		}
	}

	m_bInit = true;
	Localize();
}

void CWallClassListCtrl::OnDestroy()
{
	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Freeing\tWallChanger\tClass List"));
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallClassListItem *)GetItemData(i)) {
			if (pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList()) {
				pChildDirList->SetOnExit();
			}
		}
	}

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tWallChanger\tClass List"));
	SaveIni();
	CWallListCtrl::OnDestroy();

	g_pTheAppEndDlg->UnsignWnd(m_hWnd);
}

void CWallClassListCtrl::Localize()
{
	SetToolTips(CResString(IDS_WALL_TOOLTIP_CLASSLIST));
	int iColWidth = GetColumnWidth(0);
	DeleteColumn(0);
	InsertColumn(0, CResString(IDS_WALL_COLUMN_CLASSLIST), LVCFMT_LEFT, iColWidth);

	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirCtrl;
	int i, iCount = GetItemCount();
	for (i=0 ; i<iCount ; i++) {
		pItem = (CWallClassListItem *)GetItemData(i);
		if (pItem) {
			pChildDirCtrl = (CWallDirListCtrl *)pItem->GetChildDirList();
			if (pChildDirCtrl)
				pChildDirCtrl->Localize();
		}
	}
}

bool CWallClassListCtrl::AddItem(LPCTSTR sClassName)
{
	CWallDirListCtrl *pChildList = new CWallDirListCtrl;
	if (! pChildList->CreateEx(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TRANSPARENT,
	WS_TABSTOP | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS,
	m_rcChildDirList, GetParent(), m_iIDC_WALLDIRLISTBASE++)) {
		delete pChildList;
		return false;
	}
	pChildList->Init(m_pIni, sClassName, m_pbShowDirLoadError);
	pChildList->EnableToolTips(NULL, pChildList->IsEnableToolTips());

	CWallClassListItem *pItem = new CWallClassListItem;
	pItem->SetItemName(sClassName);
	pItem->SetChildDirList(pChildList);
	if (-1 == InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0,
	I_IMAGECALLBACK, (LPARAM) pItem)) {
		delete pItem;
		delete pChildList;
		return false;
	}

	return true;
}

void CWallClassListCtrl::SetItemEnable(int nItem, bool bEnable)
{
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	int iCount = GetItemCount();
	if (nItem >= iCount)
		return;
	if (pItem = (CWallClassListItem *)GetItemData(nItem)) {
		if (pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList()) {
			pChildDirList->SetAllItemEnable(bEnable);
		}
	}
}

void CWallClassListCtrl::UpdateSelectItemFileFindNum()
{
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	POSITION pos = GetFirstSelectedItemPosition();
	int nItem;
	while (pos) {
		nItem = GetNextSelectedItem(pos);

		pItem = (CWallClassListItem *)GetItemData(nItem);
		if (pItem) {
			pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList();
			if (pChildDirList) {
				pChildDirList->UpdateAllItemFileFindNum();
			}
		}
	}
}

bool CWallClassListCtrl::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, bool bEnable/* = true*/)
{
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallClassListItem *)GetItemData(i)) {
			if (pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList()) {
				pChildDirList->EnableToolTips(NULL, bEnable);
			}
		}
	}

	return CWallListCtrl::EnableToolTips(sToolTip, bEnable);
}

void CWallClassListCtrl::RemoveAllDirListFromPath(CString &sPath)
{
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallClassListItem *)GetItemData(i)) {
			if (pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList()) {
				pChildDirList->RemoveFromPath(sPath);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CWallClassListCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
END_MESSAGE_MAP()

void CWallClassListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	m_mContextMenu.CreatePopupMenu();
	POSITION pos = GetFirstSelectedItemPosition();

	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_ADDENABLECLASSLIST, CResString(IDS_WALL_MENU_ADDENABLECLASSLIST));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_DELCLASSLIST, CResString(IDS_WALL_MENU_DELCLASSLIST));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_UPDATEALLDIRFILEFIND, CResString(IDS_WALL_MENU_UPDATEALLDIRFILEFIND));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

void CWallClassListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallClassListItem *pItem = (CWallClassListItem *)pNMLV->lParam;
	if (pItem) {
		CWallDirListCtrl *pChildList = (CWallDirListCtrl *)pItem->GetChildDirList();
		if (pChildList) {
			pChildList->DestroyWindow();
			delete pChildList;
		}
		delete pItem;
	}

	*pResult = 0;
}

void CWallClassListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.pszText) {
		CWallClassListItem *pItem = (CWallClassListItem *)GetItemData(pDispInfo->item.iItem);
		if (pItem) {
            pItem->SetItemName(pDispInfo->item.pszText);
			CWallDirListCtrl *pDirList = (CWallDirListCtrl *)pItem->GetChildDirList();
			if (pDirList) {
				CString sOldClassName = pDirList->GetListClassName();
				if (!sOldClassName.IsEmpty())
					m_pIni->DeleteKey(_T("DirList"), sOldClassName);
				pDirList->SetListClassName(pDispInfo->item.pszText);
				pDirList->SetIniModify();
			}
			SetIniModify();
		}
	}

	*pResult = 0;
}
