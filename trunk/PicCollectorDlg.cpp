// PicCollectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MagicKD.h"
#include "FeedSource.h"

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

	CString sUrl = _T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD");
	CFeed feed(sUrl);
	feed.Save();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	CoUninitialize();
	// TODO: Add your message handler code here
}

void CPicCollectorDlg::Localize()
{
}

BEGIN_MESSAGE_MAP(CPicCollectorDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_LIST_FEED, m_list_Feed);
}
