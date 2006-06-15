#include "stdafx.h"
#include "MagicKD.h"

#include "PicCollectorDlg.h"

// GLobal variable
CPicCollectorDlg *g_pPicCollectorDlg = NULL;
#include "PicCConf.h"
CPicCConf *g_pPicCConf = NULL;

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)
CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCollectorDlg::IDD, pParent)
{
	g_pPicCollectorDlg = this;
}

CPicCollectorDlg::~CPicCollectorDlg()
{
	g_pPicCollectorDlg = NULL;
}

BOOL CPicCollectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);

	CString sIniPath;
	sIniPath.Format(_T("%sPicCollector.ini"), theApp.GetAppConfDir());
	m_Ini.SetPathName(sIniPath);
	g_pPicCConf = new CPicCConf;
	g_pPicCConf->Init(&m_Ini);
	m_Feed.SetDBPath(_T(".\\PicCollector.db"));
	m_list_Feed.Init();

	if (PathIsDirectory(g_pPicCConf->m_sDlDir))
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CoUninitialize();

	DEL(g_pPicCConf);
}

void CPicCollectorDlg::Localize()
{
}

void CPicCollectorDlg::RefreshAllFeed()
{
	int i, j;
	CString strLink;
	CStringArray strTitleArray, strLinkArray;

	m_Feed.GetFeedSourceList(strTitleArray, strLinkArray);
	for (i=0 ; i<strLinkArray.GetCount() ; i++) {
		m_Feed.BuildFromFile(strLinkArray[i]);
		m_Feed.Save();

		for (j=0 ; j<m_Feed.m_item.GetCount() ; j++) {
			strLink = m_Feed.m_item[j].m_strLink;
			if (!m_Feed.IsItemRead(strLink)) {
				// do something
				m_Feed.MarkItemRead(strLink);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CPicCollectorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PICC_BTN_CHANGEDLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnChangedldir)
END_MESSAGE_MAP()

void CPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_LIST_FEED, m_list_Feed);
}

void CPicCollectorDlg::OnOK()
{
//	CDialog::OnOK();
}

void CPicCollectorDlg::OnCancel()
{
	((CDialog *)GetParent())->EndDialog(IDCANCEL);

//	CDialog::OnCancel();
}

void CPicCollectorDlg::OnBnClickedPiccBtnChangedldir()
{
	CString sDlDir;
	if (ChooseFolder(sDlDir, GetSafeHwnd())) {
		if (PathIsDirectory(sDlDir)) {
			GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(sDlDir);
			g_pPicCConf->m_sDlDir = sDlDir;
			g_pPicCConf->m_sDlDir.SetDirty();
		}
	}
}
