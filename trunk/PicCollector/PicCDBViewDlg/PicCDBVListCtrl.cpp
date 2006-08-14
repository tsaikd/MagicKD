#include "StdAfx.h"
#include "Language.h"
#include "Resource.h"
#include "PicCollectorDlg.h"
#include "PicCDBViewDlg.h"

#include "PicCDBVListCtrl.h"

CPicCDBVListCtrl::CPicCDBVListCtrl()
{
}

CPicCDBVListCtrl::~CPicCDBVListCtrl()
{
}

void CPicCDBVListCtrl::Init()
{
	CKDListCtrl::Init();

	CRect rcWin;
	GetClientRect(rcWin);
	InsertColumn(0, _T("URL"), LVCFMT_LEFT, rcWin.Width()/2);
	InsertColumn(1, _T("Local Path"), LVCFMT_LEFT, rcWin.Width()/2 - 18);

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	Localize();
}

void CPicCDBVListCtrl::Localize()
{
	int i;
	const int iCount = 2;

	LVCOLUMN Column = {0};
	Column.mask = LVCF_TEXT;

	CString sColName[iCount];
	sColName[0] = CResString(IDS_PICC_DBV_LIST_URL);
	sColName[1] = CResString(IDS_PICC_DBV_LIST_LOCALPATH);

	for (i=0 ; i<iCount ; i++) {
		Column.pszText = (LPTSTR)(LPCTSTR)sColName[i];
		SetColumn(i, &Column);
	}
}

void CPicCDBVListCtrl::ReloadItem(int nID)
{
	SetRedraw(FALSE);
	DeleteAllItems();
	GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_URL)->SetWindowText(_T(""));
	GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_LOCALPATH)->SetWindowText(_T(""));
	GetParent()->GetDlgItem(IDC_PICC_DBV_BTN_DELRECODE)->EnableWindow(FALSE);

	CString sTableName = ((CPicCDBViewDlg *)GetParent())->GetTableNameFromID(nID);
	if (!sTableName.IsEmpty()) {
		CStringArray saTable;
		CString strSQL;
		strSQL.Format(_T("SELECT Url,Localpath FROM %s"), sTableName);
		g_pPicCollectorDlg->m_Feed.GetTableSQL(strSQL, saTable);

		CPicCDBVListItem *pItem;
		int i, iCount = saTable.GetCount();
		for (i=2 ; i<iCount ; i++) {
			pItem = new CPicCDBVListItem;

			pItem->SetUrl(saTable[i++]);
			pItem->SetLocalpath(saTable[i]);

			AddItem(pItem);
		}
	}

	SetRedraw(TRUE);
	Invalidate();
}

void CPicCDBVListCtrl::AddItem(CPicCDBVListItem *pItem)
{
	InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM) pItem);
}

BEGIN_MESSAGE_MAP(CPicCDBVListCtrl, CKDListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CPicCDBVListCtrl::OnLvnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CPicCDBVListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

void CPicCDBVListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CPicCDBVListItem *pItem = (CPicCDBVListItem *)pNMLV->lParam;
	DEL(pItem);

	*pResult = 0;
}

void CPicCDBVListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CPicCDBVListItem *pItem = (CPicCDBVListItem *)pNMLV->lParam;
	if (pItem) {
		GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_URL)->SetWindowText(pItem->GetUrl());
		GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_LOCALPATH)->SetWindowText(pItem->GetLocalpath());
		GetParent()->GetDlgItem(IDC_PICC_DBV_BTN_DELRECODE)->EnableWindow(TRUE);
	} else {
		GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_URL)->SetWindowText(_T(""));
		GetParent()->GetDlgItem(IDC_PICC_DBV_EDIT_LOCALPATH)->SetWindowText(_T(""));
		GetParent()->GetDlgItem(IDC_PICC_DBV_BTN_DELRECODE)->EnableWindow(FALSE);
	}

	*pResult = 0;
}
