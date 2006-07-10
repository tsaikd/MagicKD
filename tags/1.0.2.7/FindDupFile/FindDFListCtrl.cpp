#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"

#include "FindDFListCtrl.h"

CFindDFListCtrl::CFindDFListCtrl()
	:	m_bInit(false)
{
}

CFindDFListCtrl::~CFindDFListCtrl()
{
}

void CFindDFListCtrl::Init()
{
	CKDListCtrl::Init();

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, CResString(IDS_FIND_COLUMN_FINDLIST), LVCFMT_LEFT, rcWin.right);

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_bInit = true;
}

void CFindDFListCtrl::Localize()
{
	if (!m_bInit)
		return;

	int i;
	const int iCount = 1;

	LVCOLUMN Column = {0};
	Column.mask = LVCF_TEXT;

	CString sColName[iCount];
	sColName[0] = CResString(IDS_FIND_COLUMN_FINDLIST);

	for (i=0 ; i<iCount ; i++) {
		Column.pszText = (LPTSTR)(LPCTSTR)sColName[i];
		SetColumn(i, &Column);
	}
}

BEGIN_MESSAGE_MAP(CFindDFListCtrl, CKDListCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CFindDFListCtrl::OnDropFiles(HDROP hDropInfo)
{
	TCHAR sFilePath[MAX_PATH];
	UINT i, uCount = DragQueryFile(hDropInfo, UINT_MAX, NULL, 0);

	for (i = 0;i< uCount;i++) {
		DragQueryFile(hDropInfo, i, sFilePath, MAX_PATH);

		if (PathFileExists(sFilePath))
			InsertItem(GetItemCount(), sFilePath);
	}

	CKDListCtrl::OnDropFiles(hDropInfo);
}

void CFindDFListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	m_mContextMenu.CreatePopupMenu();
	POSITION pos = GetFirstSelectedItemPosition();

	m_mContextMenu.AppendMenu(MF_STRING, IDS_FIND_MENU_ADDFINDPATH, CResString(IDS_FIND_MENU_ADDFINDPATH));
	if (pos) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_FIND_MENU_REMOVEFINDPATH, CResString(IDS_FIND_MENU_REMOVEFINDPATH));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}
