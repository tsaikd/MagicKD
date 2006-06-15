#include "stdafx.h"
#include "MagicKD.h"
#include "PicCollector/HTMLReader/LiteHTMLReader.h"
#include "PicCHTMLEventHandler.h"

#include "PicCollectorDlg.h"

// GLobal variable
CPicCollectorDlg *g_pPicCollectorDlg = NULL;
#include "PicCConf.h"
CPicCConf *g_pPicCConf = NULL;

enum {
	KDT_SHOWDOWNLOAD		= 1
};

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)
CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCollectorDlg::IDD, pParent), m_uTimerShowDownload(0)
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

	m_HTMLReader.setEventHandler(&m_HTMLEventHandler);

//	AddNewFeed(_T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD"), _T("test1"));

	CStringArray saTable;
	int i;
	m_Feed.GetTableSQL(_T("SELECT Url, Localpath FROM PicUnDownload"), saTable);
	for (i=2 ; i<saTable.GetCount() ; i+=2)
		m_DownLoader.AddFileListQuick(saTable[i], saTable[i+1]);
	m_Feed.ExecSQL(_T("DELETE FROM PicUnDownload"));
	CreateThread(THREAD_PRIORITY_BELOW_NORMAL);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCollectorDlg Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}

	m_DownLoader.SetCanThread(false);
	CString strSQL;
	strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
		m_Feed.EscapeQuote(m_DownLoader.GetNowDLURL()),
		m_Feed.EscapeQuote(m_DownLoader.GetNowDLLocalPath()));
	m_Feed.ExecSQL(strSQL);
	while (!m_DownLoader.m_slURL.IsEmpty()) {
		strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
			m_Feed.EscapeQuote(m_DownLoader.m_slURL.RemoveHead()),
			m_Feed.EscapeQuote(m_DownLoader.m_slLocalPath.RemoveHead()));
		m_Feed.ExecSQL(strSQL);
	}
	if (WAIT_TIMEOUT == m_DownLoader.WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCollectorDlg.m_DownLoader Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		m_DownLoader.TerminateThread(0);
	}

	if (m_uTimerShowDownload) {
		KillTimer(m_uTimerShowDownload);
		m_uTimerShowDownload = 0;
	}

	CDialog::OnDestroy();
	CoUninitialize();

	DEL(g_pPicCConf);
}

DWORD CPicCollectorDlg::ThreadProc()
{
	if (!m_uTimerShowDownload)
		m_uTimerShowDownload = SetTimer(KDT_SHOWDOWNLOAD, 1000, NULL);

	RefreshAllFeed();

	return 0;
}

void CPicCollectorDlg::Localize()
{
}

void CPicCollectorDlg::AddNewFeed(LPCTSTR lpURL, LPCTSTR lpLocalName)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO PicFeed (FeedLink, name) values ('%s', '%s');"),
		m_Feed.EscapeQuote(lpURL),
		m_Feed.EscapeQuote(lpLocalName));
	m_Feed.ExecSQL(strSQL);
	CreateThread();
}

void CPicCollectorDlg::RefreshFeed(LPCTSTR lpURL)
{
	if (_tcslen(lpURL) < 5)
		return;
	if (!IsCanThread())
		return;
	m_Feed.BuildFromFile(lpURL);
	m_Feed.Save();

	CTime curTime(time(0));
	CString strLink, strName, strLocalDir, strLocalPath;
	UINT uPicNum = 1;
	int i;
	for (i=0 ; i<m_Feed.m_item.GetCount() ; i++) {
		strLink = m_Feed.m_item[i].m_strLink;
		if (!m_Feed.IsItemRead(strLink)) {
			strName = m_Feed.GetFeedName(lpURL);
			if (strName.IsEmpty())
				return;

			// Parse HTML statement
			m_HTMLReader.Read(m_Feed.m_item[i].m_strDescription);

			// check download list for black list
			// Create serial number for this link
			uPicNum = m_Ini.GetUInt(_T("PicNum"), strName, 1);

			// Check necessary dir (parent dir)
			strLocalDir.Format(_T("%s\\%s\\%s"), CString(g_pPicCConf->m_sDlDir), strName, curTime.Format(_T("%Y_%m\\%d")));
			if (!PathIsDirectory(strLocalDir))
				SHCreateDirectoryEx(GetSafeHwnd(), strLocalDir, NULL);

			// Download to local own dir and clear old list
			while (!m_HTMLEventHandler.m_slParsedPic.IsEmpty()) {
				strLocalPath.Format(_T("%s\\%05d.jpg"), strLocalDir, uPicNum++);
				m_DownLoader.AddFileListQuick(m_HTMLEventHandler.m_slParsedPic.RemoveHead(), strLocalPath);

				if (!IsCanThread())
					return;
			}

			m_Ini.WriteUInt(_T("PicNum"), strName, uPicNum);
			m_Feed.MarkItemRead(strLink);
		}
	}
}

void CPicCollectorDlg::RefreshAllFeed()
{
	int i;
	CStringArray strTitleArray, strLinkArray;

	m_Feed.GetFeedSourceList(strTitleArray, strLinkArray);
	for (i=0 ; i<strLinkArray.GetCount() ; i++)
		RefreshFeed(strLinkArray[i]);
}

BEGIN_MESSAGE_MAP(CPicCollectorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PICC_BTN_CHANGEDLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnChangedldir)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_LIST_FEED, m_list_Feed);
}

void CPicCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (!IsCanThread()) {
		KillTimer(nIDEvent);
		return;
	}

	switch (nIDEvent) {
	case KDT_SHOWDOWNLOAD:
		{
		CString sBuf;
		sBuf.Format(_T("%d: %s"), m_DownLoader.m_slURL.GetCount()+1, m_DownLoader.GetNowDLURL());
		GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(sBuf);
		}
		break;
	}

	__super::OnTimer(nIDEvent);
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
