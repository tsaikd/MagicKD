#include "stdafx.h"
#include "Language.h"
#include "Resource.h"
#include "MagicKD.h"
#include "PicCollectorDlg.h"

#include "PicCDBViewDlg.h"

IMPLEMENT_DYNAMIC(CPicCDBViewDlg, CDialog)
CPicCDBViewDlg::CPicCDBViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCDBViewDlg::IDD, pParent)
{
}

CPicCDBViewDlg::~CPicCDBViewDlg()
{
}

BOOL CPicCDBViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME);
	if (pBox) {
		pBox->InsertString(0, CResString(IDS_PICC_DBV_COMBO_DLFAILED));
		pBox->SetItemData(0, ID_TABLE_DLFAILED);
		pBox->InsertString(1, CResString(IDS_PICC_DBV_COMBO_DLOVER));
		pBox->SetItemData(1, ID_TABLE_DLOVER);
		pBox->InsertString(2, CResString(IDS_PICC_DBV_COMBO_UNDL));
		pBox->SetItemData(2, ID_TABLE_UNDL);

		pBox->SetCurSel(0);
	}
	m_list_Item.Init();

	OnBnClickedPiccDbvBtnReload();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCDBViewDlg::Localize()
{
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME);
	if (pBox) {
		int iCurPos = pBox->GetCurSel();
		while (pBox->GetCount())
			pBox->DeleteString(0);

		pBox->InsertString(0, CResString(IDS_PICC_DBV_COMBO_DLFAILED));
		pBox->SetItemData(0, ID_TABLE_DLFAILED);
		pBox->InsertString(1, CResString(IDS_PICC_DBV_COMBO_DLOVER));
		pBox->SetItemData(1, ID_TABLE_DLOVER);
		pBox->InsertString(2, CResString(IDS_PICC_DBV_COMBO_UNDL));
		pBox->SetItemData(2, ID_TABLE_UNDL);

		if (CB_ERR != iCurPos)
			pBox->SetCurSel(iCurPos);
	}

	m_list_Item.Localize();

	GetDlgItem(IDCANCEL)->SetWindowText(CResString(IDS_PICC_DBV_BTN_EXIT));
	GetDlgItem(IDC_PICC_DBV_BTN_RELOAD)->SetWindowText(CResString(IDS_PICC_DBV_BTN_RELOAD));
	GetDlgItem(IDC_PICC_DBV_BTN_DELRECODE)->SetWindowText(CResString(IDS_PICC_DBV_BTN_DELETE));
}

CString CPicCDBViewDlg::GetTableNameFromID(int nID)
{
	CString sRes;

	switch (nID) {
	case CPicCDBViewDlg::ID_TABLE_DLFAILED:
		sRes = _T("PicDownloadFailed");
		break;
	case CPicCDBViewDlg::ID_TABLE_DLOVER:
		sRes = _T("PicDownloadOver");
		break;
	case CPicCDBViewDlg::ID_TABLE_UNDL:
		sRes = _T("PicUnDownload");
		break;
	}

	return sRes;
}

BEGIN_MESSAGE_MAP(CPicCDBViewDlg, CDialog)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_RELOAD, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnReload)
	ON_CBN_SELCHANGE(IDC_PICC_DBV_COMBO_TBNAME, &CPicCDBViewDlg::OnCbnSelchangePiccDbvComboTbname)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_DELRECODE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnDelrecode)
	ON_EN_SETFOCUS(IDC_PICC_DBV_EDIT_URL, &CPicCDBViewDlg::OnEnSetfocusPiccDbvEditUrl)
	ON_EN_SETFOCUS(IDC_PICC_DBV_EDIT_LOCALPATH, &CPicCDBViewDlg::OnEnSetfocusPiccDbvEditLocalpath)
END_MESSAGE_MAP()

void CPicCDBViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_DBV_LIST_ITEM, m_list_Item);
}

void CPicCDBViewDlg::OnCbnSelchangePiccDbvComboTbname()
{
	OnBnClickedPiccDbvBtnReload();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnReload()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	m_list_Item.ReloadItem(nID);
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnDelrecode()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);
	CString sTableName = GetTableNameFromID(nID);

	if (sTableName.IsEmpty())
		return;

	SetRedraw(FALSE);
	CPicCDBVListItem *pItem = (CPicCDBVListItem *)m_list_Item.GetFirstSelectedItemLParam();
	while (pItem) {
		CString strSQL;
		strSQL.Format(_T("DELETE FROM %s WHERE Url = '%s'"),
			sTableName,
			g_pPicCollectorDlg->m_Feed.EscapeQuote(pItem->GetUrl()));
		g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);

		m_list_Item.DeleteItem(m_list_Item.GetFirstSelectedItemNum());
		pItem = (CPicCDBVListItem *)m_list_Item.GetFirstSelectedItemLParam();
	}
	SetRedraw(TRUE);
	Invalidate();
	OnBnClickedPiccDbvBtnReload();
}

void CPicCDBViewDlg::OnEnSetfocusPiccDbvEditUrl()
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_PICC_DBV_EDIT_URL);
	if (pEdit && pEdit->GetWindowTextLength()) {
		pEdit->SetRedraw(FALSE);
		pEdit->SetSel(0, -1);
		pEdit->Copy();
		pEdit->SetSel(0, 0);
		pEdit->SetRedraw(TRUE);
	}
	GetDlgItem(IDC_PICC_DBV_LIST_ITEM)->SetFocus();
}

void CPicCDBViewDlg::OnEnSetfocusPiccDbvEditLocalpath()
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_PICC_DBV_EDIT_LOCALPATH);
	if (pEdit && pEdit->GetWindowTextLength()) {
		pEdit->SetRedraw(FALSE);
		pEdit->SetSel(0, -1);
		pEdit->Copy();
		pEdit->SetSel(0, 0);
		pEdit->SetRedraw(TRUE);
	}
	GetDlgItem(IDC_PICC_DBV_LIST_ITEM)->SetFocus();
}
