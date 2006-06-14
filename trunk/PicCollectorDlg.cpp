// PicCollectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MagicKD.h"
#include "FeedSource.h"

// GLobal variable
#include "PicCConf.h"
CPicCConf *g_pPicCConf = NULL;

#include "PicCollectorDlg.h"


// CPicCollectorDlg dialog

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)

CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCollectorDlg::IDD, pParent)
{
}

CPicCollectorDlg::~CPicCollectorDlg()
{
}

BOOL CPicCollectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);

	CString sIniPath = theApp.GetAppConfDir();
	sIniPath.Append(_T("PicCollector.ini"));
	m_Ini.SetPathName(sIniPath);
	g_pPicCConf = new CPicCConf;
	g_pPicCConf->Init(&m_Ini);
	m_list_Feed.Init();

	if (PathIsDirectory(g_pPicCConf->m_sDlDir))
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);

	CString sUrl = _T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD");
	CFeed feed(_T(".\\aaa.db"), sUrl);
	feed.Save();
	feed.LoadLocal(sUrl);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CoUninitialize();

	if (g_pPicCConf) {
		delete g_pPicCConf;
		g_pPicCConf = NULL;
	}
}

void CPicCollectorDlg::Localize()
{
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
