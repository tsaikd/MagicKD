#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallDirListCtrl.h"
#include "WallClassListCtrl.h"
#include ".\wallclasslistctrl.h"

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
	InsertColumn(0, CResString(IDS_WALLCLASSLIST), LVCFMT_LEFT, rcWin.right);
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

	CancleAllSelected();
	SetItemSelected(0);
	EnableToolTips();

	m_bInit = true;
}

bool CWallClassListCtrl::AddItem(LPCTSTR sClassName)
{
	CWallDirListCtrl *pChildList = new CWallDirListCtrl;
	if (! pChildList->CreateEx(WS_EX_CLIENTEDGE | WS_EX_TRANSPARENT,
	WS_BORDER | WS_TABSTOP | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS,
	m_rcChildDirList, GetParent(), m_iIDC_WALLDIRLISTBASE++)) {
		delete pChildList;
		return false;
	}
	pChildList->Init(m_pIni, sClassName);
	pChildList->ShowWindow(SW_HIDE);

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
				MessageBox(CResString(IDS_WALLCLASSCANNOTENABLE), NULL, MB_OK|MB_ICONERROR);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CWallClassListCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

void CWallClassListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDENABLECLASSLIST, GetResString(IDS_WALLADDENABLECLASSLIST));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSLIST, GetResString(IDS_WALLDELCLASSLIST));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this->GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
	// TODO: �b���[�J�z���T���B�z�`���{���X
}

void CWallClassListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �b���[�J����i���B�z�`���{���X

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

void CWallClassListCtrl::OnDestroy()
{
	SaveIni();
	CWallListCtrl::OnDestroy();

	// TODO: �b���[�J�z���T���B�z�`���{���X
}

INT_PTR CWallClassListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

	return CWallListCtrl::OnToolHitTest(point, pTI);
}
