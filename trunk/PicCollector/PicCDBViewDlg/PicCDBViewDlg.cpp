#include "stdafx.h"
#include "Language.h"
#include "Resource.h"
#include "Others.h"
#include "MagicKD.h"
#include "PicCollectorDlg.h"

#include "PicCDBViewDlg.h"

#define TTC_MAX_WIDTH 600

IMPLEMENT_DYNAMIC(CPicCDBViewDlg, CDialog)
CPicCDBViewDlg::CPicCDBViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCDBViewDlg::IDD, pParent), m_bStopWait(true), m_iThreadFunc(THREAD_FUNC_NULL), m_dlgWait(this)
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
		pBox->InsertString(0, CResString(IDS_PICC_DBV_COMBO_UNDL));
		pBox->SetItemData(0, ID_TABLE_UNDL);
		pBox->InsertString(1, CResString(IDS_PICC_DBV_COMBO_DLFAILED));
		pBox->SetItemData(1, ID_TABLE_DLFAILED);
		pBox->InsertString(2, CResString(IDS_PICC_DBV_COMBO_DLOVER));
		pBox->SetItemData(2, ID_TABLE_DLOVER);

		pBox->SetCurSel(0);
	}
	m_list_Item.Init();

	m_dlgWait.Create(this);
	m_dlgWait.ShowWindow(SW_HIDE);

	m_ttc.Create(this, TTS_ALWAYSTIP);
	m_ttc.SetMaxTipWidth(TTC_MAX_WIDTH);
	m_ttc.AddTool(GetDlgItem(IDC_PICC_DBV_BTN_LPAGE));
	m_ttc.AddTool(GetDlgItem(IDC_PICC_DBV_BTN_RPAGE));
	m_ttc.AddTool(GetDlgItem(IDC_PICC_DBV_BTN_LLPAGE));
	m_ttc.AddTool(GetDlgItem(IDC_PICC_DBV_BTN_RRPAGE));

	Localize();
	Invalidate();

	m_list_Item.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCDBViewDlg::Localize()
{
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME);
	if (pBox) {
		int iCurPos = pBox->GetCurSel();
		while (pBox->GetCount())
			pBox->DeleteString(0);

		pBox->InsertString(0, CResString(IDS_PICC_DBV_COMBO_UNDL));
		pBox->SetItemData(0, ID_TABLE_UNDL);
		pBox->InsertString(1, CResString(IDS_PICC_DBV_COMBO_DLFAILED));
		pBox->SetItemData(1, ID_TABLE_DLFAILED);
		pBox->InsertString(2, CResString(IDS_PICC_DBV_COMBO_DLOVER));
		pBox->SetItemData(2, ID_TABLE_DLOVER);

		if (CB_ERR != iCurPos) {
			pBox->SetCurSel(iCurPos);
			OnBnClickedPiccDbvBtnReload();
		}
	}

	m_list_Item.Localize();
	CString sBuf;
	sBuf.Format(CResString(IDS_PICC_DBV_TT_BTN_PAGE), 1, 5);
	m_ttc.UpdateTipText(sBuf, GetDlgItem(IDC_PICC_DBV_BTN_LPAGE));
	m_ttc.UpdateTipText(sBuf, GetDlgItem(IDC_PICC_DBV_BTN_RPAGE));
	sBuf.Format(CResString(IDS_PICC_DBV_TT_BTN_PAGE), 10, 50);
	m_ttc.UpdateTipText(sBuf, GetDlgItem(IDC_PICC_DBV_BTN_LLPAGE));
	m_ttc.UpdateTipText(sBuf, GetDlgItem(IDC_PICC_DBV_BTN_RRPAGE));

	GetDlgItem(IDCANCEL)->SetWindowText(CResString(IDS_PICC_DBV_BTN_EXIT));
	GetDlgItem(IDC_PICC_DBV_BTN_RELOAD)->SetWindowText(CResString(IDS_PICC_DBV_BTN_RELOAD));
	GetDlgItem(IDC_PICC_DBV_BTN_DELRECODE)->SetWindowText(CResString(IDS_PICC_DBV_BTN_DELETE));
	GetDlgItem(IDC_PICC_DBV_BTN_EXPORT)->SetWindowText(CResString(IDS_PICC_DBV_BTN_EXPORT));
}

CString CPicCDBViewDlg::GetTableNameFromID(int nID)
{
	CString sRes;

	switch (nID) {
	case ID_TABLE_DLFAILED:
		sRes = _T("PicDownloadFailed");
		break;
	case ID_TABLE_DLOVER:
		sRes = _T("PicDownloadOver");
		break;
	case ID_TABLE_UNDL:
		sRes = _T("PicUnDownload");
		break;
	}

	return sRes;
}

void CPicCDBViewDlg::_ComboChanged()
{
	OnBnClickedPiccDbvBtnReload();
	m_bStopWait = true;
	EnableWindow(TRUE);
	Invalidate();
}

void CPicCDBViewDlg::_DeleteRecode()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);
	CString sTableName = GetTableNameFromID(nID);

	if (sTableName.IsEmpty())
		return;

	CPicCDBVListItem *pItem = (CPicCDBVListItem *)m_list_Item.GetFirstSelectedItemLParam();
	while (pItem) {
		CString strSQL;
		strSQL.Format(_T("DELETE FROM %s WHERE Localpath = '%s'"),
			sTableName,
			g_pPicCollectorDlg->m_Feed.EscapeQuote(pItem->GetLocalpath()));
		g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);

		m_list_Item.DeleteItem(m_list_Item.GetFirstSelectedItemNum());
		pItem = (CPicCDBVListItem *)m_list_Item.GetFirstSelectedItemLParam();
	}

	OnBnClickedPiccDbvBtnReload();
	EnableWindow(TRUE);
	m_bStopWait = true;
	Invalidate();
}

BEGIN_MESSAGE_MAP(CPicCDBViewDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_PICC_DBV_COMBO_TBNAME, &CPicCDBViewDlg::OnCbnSelchangePiccDbvComboTbname)
	ON_EN_SETFOCUS(IDC_PICC_DBV_EDIT_URL, &CPicCDBViewDlg::OnEnSetfocusPiccDbvEditUrl)
	ON_EN_SETFOCUS(IDC_PICC_DBV_EDIT_LOCALPATH, &CPicCDBViewDlg::OnEnSetfocusPiccDbvEditLocalpath)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_RELOAD, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnReload)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_DELRECODE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnDelrecode)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_LPAGE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnLpage)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_RPAGE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnRpage)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_LLPAGE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnLLpage)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_RRPAGE, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnRRpage)
	ON_BN_CLICKED(IDC_PICC_DBV_BTN_EXPORT, &CPicCDBViewDlg::OnBnClickedPiccDbvBtnExport)
END_MESSAGE_MAP()

void CPicCDBViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_DBV_LIST_ITEM, m_list_Item);
}

BOOL CPicCDBViewDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ttc.RelayEvent(pMsg);

	return __super::PreTranslateMessage(pMsg);
}

void CPicCDBViewDlg::OnCbnSelchangePiccDbvComboTbname()
{
	EnableWindow(FALSE);
	m_iThreadFunc = THREAD_FUNC_COMBOCHANGE;
	CreateThread(THREAD_PRIORITY_LOWEST);
	m_bStopWait = false;
	m_dlgWait.Display(CResString(IDS_PICC_DBV_MSG_DATAREADING), &m_bStopWait);
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnReload()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	m_list_Item.ReloadItem(nID);
	m_list_Item.SetFocus();
	m_list_Item.SetItemSelected(0);
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnLpage()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	if (IsCtrlPressed())
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()-5);
	else
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()-1);
	m_list_Item.SetFocus();
	Invalidate();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnRpage()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	if (IsCtrlPressed())
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()+5);
	else
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()+1);
	m_list_Item.SetFocus();
	Invalidate();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnLLpage()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	if (IsCtrlPressed())
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()-50);
	else
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()-10);
	m_list_Item.SetFocus();
	Invalidate();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnRRpage()
{
	int nCurSel = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetCurSel();
	int nID = ((CComboBox *)GetDlgItem(IDC_PICC_DBV_COMBO_TBNAME))->GetItemData(nCurSel);

	if (IsCtrlPressed())
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()+50);
	else
		m_list_Item.ReloadItem(nID, m_list_Item.GetCurPage()+10);
	m_list_Item.SetFocus();
	Invalidate();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnDelrecode()
{
	EnableWindow(FALSE);
	m_iThreadFunc = THREAD_FUNC_DELRECODE;
	CreateThread(THREAD_PRIORITY_LOWEST);
	m_bStopWait = false;
	m_dlgWait.Display(CResString(IDS_PICC_DBV_MSG_DATADELING), &m_bStopWait);
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
	m_list_Item.SetFocus();
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
	m_list_Item.SetFocus();
}

void CPicCDBViewDlg::OnBnClickedPiccDbvBtnExport()
{
	CString sBuf, sPath;
	sPath.Format(_T("%sPicCDB.txt"), theApp.GetAppDir());

	CPicCDBVListItem *pItem;
	int i, iCount = m_list_Item.GetItemCount();
	if (!iCount)
		return;

	CStdioFile file;
	if (!file.Open(sPath, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::shareExclusive))
		return;

	for (i=0 ; i<iCount ; i++) {
		pItem = (CPicCDBVListItem *)m_list_Item.GetItemData(i);
		if (!pItem)
			continue;

		sBuf.Format(_T("\"%s\" \"%s\"\n"), pItem->GetUrl(), pItem->GetLocalpath());
		file.WriteString(sBuf);
	}
	file.Close();

	sBuf.Format(CResString(IDS_PICC_DBV_MSG_EXPORT), sPath);
	MessageBox(sBuf, NULL, MB_OK | MB_ICONINFORMATION);
}
