#include "stdafx.h"
#include "Others.h"
#include "Resource.h"
#include "Language.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "KDWinMsgBox.h"
#include "PicCollector/HTMLReader/LiteHTMLReader.h"
#include "PicCHTMLEventHandler.h"
#include "InputBox.h"
#include "PicCDBViewDlg.h"
#include "FindDupFileDlg.h"
#include "PicCConf.h"

#include "PicCollectorDlg.h"

#define DEFAULT_REFRESH_TIME (10*60*1000) // (ms)

// GLobal variable
CPicCollectorDlg *g_pPicCollectorDlg = NULL;

enum {
	KDT_NULL,
	KDT_SHOWDOWNLOAD,
	KDT_REFRESH
};

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)
CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CPicCollectorDlg::IDD, pParent), m_uTimerShowDownload(0), m_uTimerRefresh(0), m_uLastDLDay(0),
		m_pDownLoader(NULL), m_bInit(false), m_iDLIconStatus(0)
{
	g_pPicCollectorDlg = this;
}

CPicCollectorDlg::~CPicCollectorDlg()
{
	g_pPicCollectorDlg = NULL;
}

BOOL CPicCollectorDlg::OnInitDialog()
{
	g_pTheLog->Log(_T("Init PicCollectorDlg"), CKDLog::LOGLV_INFO);
	CDialog::OnInitDialog();

	g_pTheAppEndDlg->SignWnd(GetSafeHwnd(), 2);

	CString sPath;
	sPath.Format(_T("%sPicCollector.ini"), theApp.GetAppConfDir());
	m_Ini.SetPathName(sPath);
	g_pPicCConf = new CPicCConf;
	g_pPicCConf->Init(&m_Ini);
	sPath.Format(_T("%sPicCollector.db"), theApp.GetAppConfDir());
	m_Feed.OpenDB(sPath);
	m_list_Feed.Init();
	m_ttc.Create(this, TTS_ALWAYSTIP);
	m_ttc.SetMaxTipWidth(600);

	// Setup ToolTipCtrl for CKDStaticPath
	m_static_DLDir.SignToolTipCtrl(&m_ttc);
	m_static_DLLocalPath.SignToolTipCtrl(&m_ttc);
	m_static_Download.SignToolTipCtrl(&m_ttc);

	if (PathIsDirectory(g_pPicCConf->m_sDlDir)) {
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);
	} else {
		g_pPicCConf->m_sDlDir.Format(_T("%sPicCollector\\"), theApp.GetAppDir());
		if (!PathIsDirectory(g_pPicCConf->m_sDlDir))
			SHCreateDirectoryEx(GetSafeHwnd(), g_pPicCConf->m_sDlDir, NULL);
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);
	}

	m_HTMLReader.setEventHandler(&m_HTMLEventHandler);
	DLIconSwitch(2);
	GetDlgItem(IDC_PICC_BTN_DELNOWDL)->EnableWindow(FALSE);
	GetDlgItem(IDC_PICC_BTN_DELAYDL)->EnableWindow(FALSE);
	GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(_T(""));
	GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH)->SetWindowText(_T(""));

	if (!m_pDownLoader) {
		m_pDownLoader = new CPicCDLManager;
		m_pDownLoader->Init(&m_Feed);
	}
	if (!m_uTimerShowDownload)
		m_uTimerShowDownload = SetTimer(KDT_SHOWDOWNLOAD, 1000, NULL);

	OnBnClickedPiccBtnRefreshfeed();

	CStatic *pStatic;
	pStatic = (CStatic *)GetDlgItem(IDC_PICC_STATIC_PLAY);
	pStatic->ModifyStyle(0, SS_ICON);
	pStatic->SetIcon(g_pTheIcons->GetIcon(IDI_PICC_PLAY, 14, 14));
	m_ttc.AddTool(pStatic);

	pStatic = (CStatic *)GetDlgItem(IDC_PICC_STATIC_PAUSE);
	pStatic->ModifyStyle(0, SS_ICON);
	pStatic->SetIcon(g_pTheIcons->GetIcon(IDI_PICC_PAUSE, 14, 14));
	m_ttc.AddTool(pStatic);

	pStatic = (CStatic *)GetDlgItem(IDC_PICC_STATIC_STOP);
	pStatic->ModifyStyle(0, SS_ICON);
	pStatic->SetIcon(g_pTheIcons->GetIcon(IDI_PICC_STOP, 14, 14));
	m_ttc.AddTool(pStatic);

	CButton *pBtn;
	pBtn = (CButton *)GetDlgItem(IDC_PICC_BTN_DELNOWDL);
	pBtn->ModifyStyle(0, BS_ICON);
	pBtn->SetIcon(g_pTheIcons->GetIcon(IDI_PICC_DEL, 18, 18));
	m_ttc.AddTool(pBtn);

	pBtn = (CButton *)GetDlgItem(IDC_PICC_BTN_DELAYDL);
	pBtn->ModifyStyle(0, BS_ICON);
	pBtn->SetIcon(g_pTheIcons->GetIcon(IDI_PICC_FORWARD, 14, 14));
	m_ttc.AddTool(pBtn);

	m_bInit = true;
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	g_pTheLog->Log(_T("Exit PicCollectorDlg"), CKDLog::LOGLV_INFO);
	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tPicCollector\tThread"));
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(15000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCollectorDlg Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}

	if (m_uTimerShowDownload) {
		KillTimer(m_uTimerShowDownload);
		m_uTimerShowDownload = 0;
	}
	if (m_uTimerRefresh) {
		KillTimer(m_uTimerRefresh);
		m_uTimerRefresh = 0;
	}

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tPicCollector\tDownload Thread"));
	if (m_pDownLoader) {
		m_pDownLoader->Destroy();
		DEL(m_pDownLoader);
	}

	CDialog::OnDestroy();

	DEL(g_pPicCConf);
}

void CPicCollectorDlg::OnAPMSuspend()
{
	if (m_uTimerShowDownload) {
		KillTimer(m_uTimerShowDownload);
		m_uTimerShowDownload = 0;
	}
	if (m_uTimerRefresh) {
		KillTimer(m_uTimerRefresh);
		m_uTimerRefresh = 0;
	}
}

void CPicCollectorDlg::OnAPMResumeSuspend()
{
	if (!m_uTimerShowDownload)
		m_uTimerShowDownload = SetTimer(KDT_SHOWDOWNLOAD, 1000, NULL);
	if (!m_uTimerRefresh)
		m_uTimerRefresh = SetTimer(KDT_REFRESH, DEFAULT_REFRESH_TIME, NULL);
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
	if (!m_bInit)
		return;

	m_list_Feed.Localize();

	GetDlgItem(IDC_PICC_STATIC_DLDIRHELP)->SetWindowText(CResString(IDS_PICC_STATIC_DLDIR));
	GetDlgItem(IDC_PICC_BTN_CHANGEDLDIR)->SetWindowText(CResString(IDS_PICC_BTN_CHANGE_DLDIR));
	GetDlgItem(IDC_PICC_BTN_VIEW_DLDIR)->SetWindowText(CResString(IDS_PICC_BTN_VIEW_DLDIR));
	GetDlgItem(IDC_PICC_BTN_ADDNEWFEED)->SetWindowText(CResString(IDS_PICC_BTN_ADD_NEW_FEED));
	GetDlgItem(IDC_PICC_BTN_REFRESHFEED)->SetWindowText(CResString(IDS_PICC_BTN_REFRESH_FEED));
	GetDlgItem(IDC_PICC_BTN_REMOVEFEED)->SetWindowText(CResString(IDS_PICC_BTN_DEL_FEED));
	GetDlgItem(IDC_PICC_BTN_DBVIEW)->SetWindowText(CResString(IDS_PICC_BTN_VIEWDB));

	m_ttc.UpdateTipText(CResString(IDS_PICC_TT_STATIC_PLAY), GetDlgItem(IDC_PICC_STATIC_PLAY));
	m_ttc.UpdateTipText(CResString(IDS_PICC_TT_STATIC_PAUSE), GetDlgItem(IDC_PICC_STATIC_PAUSE));
	m_ttc.UpdateTipText(CResString(IDS_PICC_TT_STATIC_STOP), GetDlgItem(IDC_PICC_STATIC_STOP));
	m_ttc.UpdateTipText(CResString(IDS_PICC_TT_BTN_DELNOWDL), GetDlgItem(IDC_PICC_BTN_DELNOWDL));
	m_ttc.UpdateTipText(CResString(IDS_PICC_TT_BTN_DELAY_DL), GetDlgItem(IDC_PICC_BTN_DELAYDL));
}

void CPicCollectorDlg::DoSize()
{
	if (!m_bInit)
		return;

	int iMarginRight = 15;
	int iMarginBottom = 15;
	int iWidth;
	CRect rcItem;

	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DLDIR), GetDlgItem(IDC_PICC_STATIC_DLDIRHELP),
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_BTN_VIEW_DLDIR), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_BTN_CHANGEDLDIR), GetDlgItem(IDC_PICC_BTN_VIEW_DLDIR),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DLDIR), GetDlgItem(IDC_PICC_BTN_CHANGEDLDIR),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5, true);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_BTN_DBVIEW), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_LIST_FEED), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);
	m_list_Feed.GetClientRect(rcItem);
	iWidth = rcItem.Width();
	iWidth -= m_list_Feed.GetColumnWidth(0);
	iWidth -= m_list_Feed.GetColumnWidth(1);
	if (iWidth < 100)
		iWidth = 100;
	m_list_Feed.SetColumnWidth(2, iWidth);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DOWNLOAD), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_DOWNLOAD), GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_PLAY), GetDlgItem(IDC_PICC_STATIC_DOWNLOAD),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_PAUSE), GetDlgItem(IDC_PICC_STATIC_DOWNLOAD),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_STATIC_STOP), GetDlgItem(IDC_PICC_STATIC_DOWNLOAD),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_BTN_DELNOWDL), GetDlgItem(IDC_PICC_STATIC_DOWNLOAD),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_PICC_BTN_DELAYDL), GetDlgItem(IDC_PICC_STATIC_DOWNLOAD),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);

	KDMoveDlgItem(GetDlgItem(IDC_PICC_LIST_FEED), GetDlgItem(IDC_PICC_STATIC_PLAY),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5, true);

	Invalidate();
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

	if (!m_Feed.BuildFromFile(lpURL)) {
		if (0 == GetOnInternet()) {
			g_pKDWinMsgBox->Add(GetSafeHwnd(), _T("Failed to download ") + CString(lpURL),
				NULL, MB_OK|MB_ICONERROR);
		}
		return;
	}
	m_Feed.Save();
	if (!IsCanThread())
		return;

	CTime curTime(time(0));
	CString strLink, strName, strLocalDir, strLocalPath;
	UINT uPicNum = 1;
	if (!m_Ini.IsKeyExist(_T("General"), _T("uLastDLDay")))
		m_Ini.WriteUInt(_T("General"), _T("uLastDLDay"), _ttoi(curTime.Format(_T("%d"))));

	int i;
	for (i=0 ; i<m_Feed.m_item.GetCount() ; i++) {
		if (!IsCanThread())
			return;

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
				CStringArray saNames;
				int i;

				uPicNum = 1;
				m_Ini.GetKeyNames(_T("PicNum"), &saNames);
				for (i=0 ; i<saNames.GetCount() ; i++)
					m_Ini.WriteUInt(_T("PicNum"), saNames[i], 1);

				m_uLastDLDay = _ttoi(curTime.Format(_T("%d")));
				m_Ini.WriteUInt(_T("General"), _T("uLastDLDay"), m_uLastDLDay);
			}

			// Check necessary dir (parent dir)
			strLocalDir.Format(_T("%s\\%s\\%s"), CString(g_pPicCConf->m_sDlDir), strName, curTime.Format(_T("%Y_%m\\%d")));
			//if (!PathIsDirectory(strLocalDir))
			//	SHCreateDirectoryEx(GetSafeHwnd(), strLocalDir, NULL);

			// Download to local own dir and clear old list
			m_pDownLoader->SetDBSync(false);
			while (!m_HTMLEventHandler.m_slParsedPic.IsEmpty()) {
				strLocalPath.Format(_T("%s\\%05d.jpg"), strLocalDir, uPicNum++);
				m_pDownLoader->AddFileList(m_HTMLEventHandler.m_slParsedPic.RemoveHead(), strLocalPath);

				if (!IsCanThread())
					break;
			}
			m_pDownLoader->SetDBSync(true);

			if (!IsCanThread())
				return;
			m_pDownLoader->CreateThread();

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

void CPicCollectorDlg::DLIconSwitch(int iPos)
{
	if (!IsCanThread())
		return;

	switch (m_iDLIconStatus) {
	case 1:
		GetDlgItem(IDC_PICC_STATIC_PAUSE)->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		break;
	case 2:
		GetDlgItem(IDC_PICC_STATIC_STOP)->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		break;
	default:
		GetDlgItem(IDC_PICC_STATIC_PLAY)->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		break;
	}

	switch (iPos) {
	case 1:
		m_iDLIconStatus = iPos;
		GetDlgItem(IDC_PICC_STATIC_PAUSE)->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		break;
	case 2:
		m_iDLIconStatus = iPos;
		GetDlgItem(IDC_PICC_STATIC_STOP)->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		break;
	default:
		m_iDLIconStatus = 0;
		GetDlgItem(IDC_PICC_STATIC_PLAY)->ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
		break;
	}

	if (m_bInit)
		Invalidate();
}

BEGIN_MESSAGE_MAP(CPicCollectorDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PICC_BTN_CHANGEDLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnChangedldir)
	ON_BN_CLICKED(IDC_PICC_BTN_ADDNEWFEED, &CPicCollectorDlg::OnBnClickedPiccBtnAddnewfeed)
	ON_BN_CLICKED(IDC_PICC_BTN_REFRESHFEED, &CPicCollectorDlg::OnBnClickedPiccBtnRefreshfeed)
	ON_BN_CLICKED(IDC_PICC_BTN_REMOVEFEED, &CPicCollectorDlg::OnBnClickedPiccBtnRemovefeed)
	ON_BN_CLICKED(IDC_PICC_BTN_FINDDF, &CPicCollectorDlg::OnBnClickedPiccBtnFindDF)
	ON_BN_CLICKED(IDC_PICC_BTN_DELAYDL, &CPicCollectorDlg::OnBnClickedPiccBtnDelaydl)
	ON_BN_CLICKED(IDC_PICC_BTN_VIEW_DLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnViewDldir)
	ON_BN_CLICKED(IDC_PICC_BTN_DBVIEW, &CPicCollectorDlg::OnBnClickedPiccBtnDbview)
	ON_STN_CLICKED(IDC_PICC_STATIC_DLDIR, &CPicCollectorDlg::OnStnClickedPiccStaticDldir)
	ON_STN_CLICKED(IDC_PICC_STATIC_PLAY, &CPicCollectorDlg::OnStnClickedPiccStaticPlay)
	ON_STN_CLICKED(IDC_PICC_STATIC_PAUSE, &CPicCollectorDlg::OnStnClickedPiccStaticPause)
	ON_STN_CLICKED(IDC_PICC_STATIC_STOP, &CPicCollectorDlg::OnStnClickedPiccStaticStop)
	ON_STN_CLICKED(IDC_PICC_STATIC_DLLOCALPATH, &CPicCollectorDlg::OnStnClickedPiccStaticDllocalpath)
	ON_STN_CLICKED(IDC_PICC_STATIC_DOWNLOAD, &CPicCollectorDlg::OnStnClickedPiccStaticDownload)
	ON_BN_CLICKED(IDC_PICC_BTN_DELNOWDL, &CPicCollectorDlg::OnBnClickedPiccBtnDelnowdl)
END_MESSAGE_MAP()

void CPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_LIST_FEED, m_list_Feed);
	DDX_Control(pDX, IDC_PICC_STATIC_DLDIR, m_static_DLDir);
	DDX_Control(pDX, IDC_PICC_STATIC_DLLOCALPATH, m_static_DLLocalPath);
	DDX_Control(pDX, IDC_PICC_STATIC_DOWNLOAD, m_static_Download);
}

BOOL CPicCollectorDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ttc.RelayEvent(pMsg);

	return __super::PreTranslateMessage(pMsg);
}

void CPicCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (!IsCanThread()) {
		KillTimer(nIDEvent);
		return;
	}

	switch (nIDEvent) {
	case KDT_SHOWDOWNLOAD:
		if (m_pDownLoader->IsThreadRunning()) {
			CString sBuf;
			if (m_pDownLoader->GetNowDLRetryTimes()) {
				sBuf.Format(_T("%d:(%d%% retry: %d) %s"), m_pDownLoader->GetDownloadCount()+1, (int)(m_pDownLoader->GetPercentOfNowDL()*100), m_pDownLoader->GetNowDLRetryTimes(), m_pDownLoader->GetNowDLURL());
			} else {
				sBuf.Format(_T("%d:(%d%%) %s"), m_pDownLoader->GetDownloadCount()+1, (int)(m_pDownLoader->GetPercentOfNowDL()*100), m_pDownLoader->GetNowDLURL());
			}
			GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(sBuf);
			GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH)->SetWindowText(m_pDownLoader->GetNowDLLocalPath());
		} else if (m_pDownLoader->GetDownloadCount()) {
			Sleep(2000);
			if (m_pDownLoader->IsThreadRunning() || !IsCanThread())
				break;
			KillTimer(nIDEvent);
			m_uTimerShowDownload = 0;
#ifdef DEBUG
			AfxMessageBox(_T("WARNNING: Downloader exit by unknown reason!"));
#endif //DEBUG
			m_pDownLoader->SaveNowDLToList();
			m_pDownLoader->Destroy();
			delete m_pDownLoader;
			m_Feed.ReloadDB();
			m_pDownLoader = new CPicCDLManager;
			m_pDownLoader->Init(&m_Feed);
			m_uTimerShowDownload = SetTimer(KDT_SHOWDOWNLOAD, 1000, NULL);
			break;
		} else {
			GetDlgItem(IDC_PICC_STATIC_DOWNLOAD)->SetWindowText(_T(""));
			GetDlgItem(IDC_PICC_STATIC_DLLOCALPATH)->SetWindowText(_T(""));
			KillTimer(nIDEvent);
			m_uTimerShowDownload = 0;
		}
		break;
	case KDT_REFRESH:
		CreateThread();
		break;
	}

	__super::OnTimer(nIDEvent);
}

void CPicCollectorDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	DoSize();
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

void CPicCollectorDlg::OnBnClickedPiccBtnViewDldir()
{
	ExplorerDir(g_pPicCConf->m_sDlDir);
}

void CPicCollectorDlg::OnStnClickedPiccStaticDldir()
{
	ExplorerDir(g_pPicCConf->m_sDlDir);
}

void CPicCollectorDlg::OnBnClickedPiccBtnAddnewfeed()
{
	CString strURL;
	CString strName;
	CInputBox input(this, false);
	if (IDOK == input.Show(CResString(IDS_PICC_MSG_ENTER_FEED_URL), strURL)) {
		if (strURL.IsEmpty())
			return;
		if (IDOK == input.Show(CResString(IDS_PICC_MSG_ENTER_FEED_NAME), strName)) {
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
	m_uTimerRefresh = SetTimer(KDT_REFRESH, DEFAULT_REFRESH_TIME, NULL);

	if (m_bInit)
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

void CPicCollectorDlg::OnBnClickedPiccBtnFindDF()
{
	g_pMagicKDDlg->SetFuncEnable(CMagicKDDlg::eFunc_FindDupFile, true);
	g_pMagicKDDlg->SetCurTabSel(CMagicKDDlg::eFunc_FindDupFile);
	g_pFindDupFileDlg->AddFindPath(g_pPicCConf->m_sDlDir);
	g_pFindDupFileDlg->OnBnClickedFindBtnStartfind();
}

void CPicCollectorDlg::OnBnClickedPiccBtnDbview()
{
	CPicCDBViewDlg dlg;
	dlg.DoModal();
}

void CPicCollectorDlg::OnBnClickedPiccBtnDelnowdl()
{
	GetDlgItem(IDC_PICC_BTN_DELNOWDL)->EnableWindow(FALSE);
	m_pDownLoader->SetDiscardNowDL();
	GetParent()->SetFocus();
}

void CPicCollectorDlg::OnBnClickedPiccBtnDelaydl()
{
	GetDlgItem(IDC_PICC_BTN_DELAYDL)->EnableWindow(FALSE);
	m_pDownLoader->DelayDownload();
	GetParent()->SetFocus();
}

void CPicCollectorDlg::OnStnClickedPiccStaticPlay()
{
	if (m_iDLIconStatus == 1) {		// Only Affect At Pause Status
		m_pDownLoader->Pause(false);
		DLIconSwitch(0);
	}
}

void CPicCollectorDlg::OnStnClickedPiccStaticPause()
{
	if (m_iDLIconStatus == 0) {		// Affect At Play or Pause Status
		m_pDownLoader->Pause(true);
		DLIconSwitch(1);
	} else if (m_iDLIconStatus == 1) {
		OnStnClickedPiccStaticPlay();
	}
}

void CPicCollectorDlg::OnStnClickedPiccStaticStop()
{
	// User can't do this operation
}

void CPicCollectorDlg::OnStnClickedPiccStaticDllocalpath()
{
	CString sPath = m_pDownLoader->GetNowDLLocalPath();
	if (sPath.IsEmpty())
		return;

	ExplorerFile(sPath);
}

void CPicCollectorDlg::OnStnClickedPiccStaticDownload()
{
	CString sUrl = m_pDownLoader->GetNowDLURL();
	if (sUrl.IsEmpty())
		return;

	if (0 == _tcsncmp(sUrl, _T("http://"), 7))
		ShellExecute(NULL, _T("open"), sUrl, NULL, NULL, SW_SHOW);
}
