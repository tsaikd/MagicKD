#include "stdafx.h"
#include "MagicKD.h"
#include "PicCollector/HTMLReader/LiteHTMLReader.h"
#include "PicCHTMLEventHandler.h"
#include "InputBox.h"

#include "PicCollectorDlg.h"

// GLobal variable
CPicCollectorDlg *g_pPicCollectorDlg = NULL;
#include "PicCConf.h"
CPicCConf *g_pPicCConf = NULL;

enum {
	KDT_SHOWDOWNLOAD	= 1,
	KDT_REFRESH
};

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)
CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCollectorDlg::IDD, pParent), m_uTimerShowDownload(0), m_uTimerRefresh(0), m_uLastDLDay(0)
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

	g_pTheAppEndDlg->SignWnd(GetSafeHwnd(), 3);

	CString sPath;
	sPath.Format(_T("%sPicCollector.ini"), theApp.GetAppConfDir());
	m_Ini.SetPathName(sPath);
	g_pPicCConf = new CPicCConf;
	g_pPicCConf->Init(&m_Ini);
	sPath.Format(_T("%sPicCollector.db"), theApp.GetAppConfDir());
	m_Feed.SetDBPath(sPath);
	m_list_Feed.Init();

	if (PathIsDirectory(g_pPicCConf->m_sDlDir)) {
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);
	} else {
		g_pPicCConf->m_sDlDir.Format(_T("%sPicCollector\\"), theApp.GetAppDir());
		if (!PathIsDirectory(g_pPicCConf->m_sDlDir))
			SHCreateDirectoryEx(GetSafeHwnd(), g_pPicCConf->m_sDlDir, NULL);
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);
	}

	m_HTMLReader.setEventHandler(&m_HTMLEventHandler);

	CStringArray saTable;
	int i;
	m_Feed.GetTableSQL(_T("SELECT Url, Localpath FROM PicUnDownload"), saTable);
	for (i=2 ; i<saTable.GetCount() ; i+=2)
		m_DownLoader.AddFileListQuick(saTable[i], saTable[i+1]);
	m_DownLoader.CreateThread(THREAD_PRIORITY_LOWEST);

	OnBnClickedPiccBtnRefreshfeed();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tPicCollector\tThread"));
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCollectorDlg Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tPicCollector\tDownload Thread"));
	m_DownLoader.SetCanThread(false);
	if (WAIT_TIMEOUT == m_DownLoader.WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCollectorDlg.m_DownLoader Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		m_DownLoader.SaveNowDLToList();
		m_DownLoader.TerminateThread(0);
	}

	if (m_uTimerShowDownload) {
		KillTimer(m_uTimerShowDownload);
		m_uTimerShowDownload = 0;
	}
	if (m_uTimerRefresh) {
		KillTimer(m_uTimerRefresh);
		m_uTimerRefresh = 0;
	}

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Saving\tPicCollector\tUndownload List"));
	m_Feed.ExecSQL(_T("DELETE FROM PicUnDownload"));
	m_Feed.ExecSQL(_T("PRAGMA synchronous = OFF"));
	CString strSQL;
	while (!m_DownLoader.IsDownloadAllOver()) {
		strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
			m_Feed.EscapeQuote(m_DownLoader.RemoveHeadDLURL()),
			m_Feed.EscapeQuote(m_DownLoader.RemoveHeadLocalPath()));
		m_Feed.ExecSQL(strSQL);
	}

	CDialog::OnDestroy();

	DEL(g_pPicCConf);
}

DWORD CPicCollectorDlg::ThreadProc()
{
	GetDlgItem(IDC_PICC_BTN_REFRESHFEED)->EnableWindow(FALSE);
	RefreshAllFeed();
	GetDlgItem(IDC_PICC_BTN_REFRESHFEED)->EnableWindow(TRUE);

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
	m_Feed.BuildFromFile(lpURL);
	m_Feed.Save();
	m_list_Feed.ReloadItems();
	CreateThread();
}

void CPicCollectorDlg::RefreshFeed(LPCTSTR lpURL)
{
	if (_tcslen(lpURL) < 5)
		return;
	if (!IsCanThread())
		return;
	if (!m_uTimerShowDownload)
		m_uTimerShowDownload = SetTimer(KDT_SHOWDOWNLOAD, 1000, NULL);

	m_Feed.BuildFromFile(lpURL);
	m_Feed.Save();

	CTime curTime(time(0));
	CString strLink, strName, strLocalDir, strLocalPath;
	UINT uPicNum = 1;
	if (!m_Ini.IsKeyExist(_T("General"), _T("uLastDLDay")))
		m_Ini.WriteUInt(_T("General"), _T("uLastDLDay"), _ttoi(curTime.Format(_T("%d"))));

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
			m_uLastDLDay = m_Ini.GetUInt(_T("General"), _T("uLastDLDay"), _ttoi(curTime.Format(_T("%d"))));
			if (m_uLastDLDay == (UINT)_ttoi(curTime.Format(_T("%d")))) {
				uPicNum = m_Ini.GetUInt(_T("PicNum"), strName, 1);
			} else {
				uPicNum = 1;
				m_uLastDLDay = _ttoi(curTime.Format(_T("%d")));
				m_Ini.WriteUInt(_T("General"), _T("uLastDLDay"), m_uLastDLDay);
			}

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
			m_DownLoader.CreateThread(THREAD_PRIORITY_LOWEST);

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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PICC_BTN_CHANGEDLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnChangedldir)
	ON_BN_CLICKED(IDC_PICC_BTN_ADDNEWFEED, &CPicCollectorDlg::OnBnClickedPiccBtnAddnewfeed)
	ON_BN_CLICKED(IDC_PICC_BTN_REFRESHFEED, &CPicCollectorDlg::OnBnClickedPiccBtnRefreshfeed)
	ON_BN_CLICKED(IDC_PICC_BTN_REMOVEFEED, &CPicCollectorDlg::OnBnClickedPiccBtnRemovefeed)
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
		sBuf.Format(_T("%d:(%d%%) %s"), m_DownLoader.GetDownloadCount()+1, (int)(m_DownLoader.GetPercentOfNowDL()*100), m_DownLoader.GetNowDLURL());
		GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(sBuf);
		GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH)->SetWindowText(m_DownLoader.GetNowDLLocalPath());
		if (!m_DownLoader.IsThreadRunning()) {
			Sleep(2000);
			if (m_DownLoader.IsThreadRunning())
				break;
			Sleep(2000);
			if (m_DownLoader.IsThreadRunning())
				break;
			Sleep(2000);
			if (m_DownLoader.IsThreadRunning())
				break;
			Sleep(2000);
			if (m_DownLoader.IsThreadRunning())
				break;
			GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(_T(""));
			GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH)->SetWindowText(_T(""));
			KillTimer(nIDEvent);
			m_uTimerShowDownload = 0;
		}
		}
		break;
	case KDT_REFRESH:
		CreateThread();
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

void CPicCollectorDlg::OnBnClickedPiccBtnAddnewfeed()
{
	CString strURL;
	CString strName;
	CInputBox input(this, false);
	if (IDOK == input.Show(_T("請輸入 rss 網址"), strURL)) {
		if (strURL.IsEmpty())
			return;
		if (IDOK == input.Show(_T("請輸入 rss 本機名稱"), strName)) {
			if (strName.IsEmpty())
				return;
			AddNewFeed(strURL, strName);
		}
	}
}

void CPicCollectorDlg::OnBnClickedPiccBtnRefreshfeed()
{
	if (m_uTimerRefresh)
		KillTimer(m_uTimerRefresh);
	m_uTimerRefresh = SetTimer(KDT_REFRESH, 10*60*1000, NULL);
	CreateThread();
}

void CPicCollectorDlg::OnBnClickedPiccBtnRemovefeed()
{
	CPicCFeedListItem *pItem = (CPicCFeedListItem *)m_list_Feed.GetFirstSelectedItemLParam();
	if (pItem) {
		CString strURL(pItem->GetFeedURL());
		if (!strURL.IsEmpty()) {
			CString strSQL;

			strSQL.Format(_T("DELETE FROM FeedItem WHERE FeedLink = '%s'"), m_Feed.EscapeQuote(strURL));
			m_Feed.ExecSQL(strSQL);
			strSQL.Format(_T("DELETE FROM FeedSource WHERE FeedLink = '%s'"), m_Feed.EscapeQuote(strURL));
			m_Feed.ExecSQL(strSQL);
			strSQL.Format(_T("DELETE FROM PicFeed WHERE FeedLink = '%s'"), m_Feed.EscapeQuote(strURL));
			m_Feed.ExecSQL(strSQL);

			m_list_Feed.ReloadItems();
		}
	}
}
