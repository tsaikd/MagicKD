#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallDirListCtrl.h"
#include "WallClassListCtrl.h"

CWallClassListCtrl::CWallClassListCtrl() :
	m_bInit(false), m_iIDC_WALLDIRLISTBASE(200)
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

void CWallClassListCtrl::Init(CIni *pIni, CRect &rcChildDirList)
{
	CWallListCtrl::Init(pIni);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES);

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, CResString(IDS_WALL_COLUMN_CLASSLIST), LVCFMT_LEFT, rcWin.right);
	m_rcChildDirList = rcChildDirList;

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

	SetToolTips(CResString(IDS_WALL_TOOLTIP_CLASSLIST));

	m_bInit = true;
}

bool CWallClassListCtrl::AddItem(LPCTSTR sClassName)
{
	CWallDirListCtrl *pChildList = new CWallDirListCtrl;
	if (! pChildList->CreateEx(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TRANSPARENT,
	WS_BORDER | WS_TABSTOP | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS,
	m_rcChildDirList, GetParent(), m_iIDC_WALLDIRLISTBASE++)) {
		delete pChildList;
		return false;
	}
	pChildList->Init(m_pIni, sClassName);
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
			if (bEnable && !(pChildDirList->IsFindPath())) {
				ListView_SetCheckState(m_hWnd, nItem, false);
				MessageBox(CResString(IDS_WALL_CLASSCANNOTENABLE), NULL, MB_OK|MB_ICONERROR);
			}
		}
	}
}

void CWallClassListCtrl::UpdateSelectItemFileFindNum()
{
	CWallClassListItem *pItem;
	CWallDirListCtrl *pChildDirList;
	int iCount = GetItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if (pItem = (CWallClassListItem *)GetItemData(i)) {
			if (pChildDirList = (CWallDirListCtrl *)pItem->GetChildDirList()) {
				pChildDirList->UpdateAllItemFileFindNum();
			}
		}
	}
}

BOOL CWallClassListCtrl::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
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
END_MESSAGE_MAP()

void CWallClassListCtrl::OnDestroy()
{
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

	SaveIni();
	CWallListCtrl::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallClassListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_ADDENABLECLASSLIST, GetResString(IDS_WALL_MENU_ADDENABLECLASSLIST));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_DELCLASSLIST, GetResString(IDS_WALL_MENU_DELCLASSLIST));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALL_MENU_UPDATEALLDIRFILEFIND, GetResString(IDS_WALL_MENU_UPDATEALLDIRFILEFIND));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this->GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallClassListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼

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
