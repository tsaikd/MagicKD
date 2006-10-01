#include "stdafx.h"
#include "Others.h"
#include "MagicKDDlg.h"

#include "MagicKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMagicKDApp theApp;
CKDTray *g_pTheTray = NULL;

CMagicKDApp::CMagicKDApp()
	:	m_bOnQuit(false)
{
}

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsWndRunning())
		return FALSE;

	CString sBuf;
	sBuf.Format(_T("%sMagicKD_%s.log"), GetAppLogDir(), CTime(time(NULL)).Format(_T("%Y_%m_%d")));
	g_pTheLog = new CMagicKDLog;
#ifdef DEBUG
	g_pTheLog->Init(sBuf, CKDLog::LOGLV_DEBUG);
#else
	g_pTheLog->Init(sBuf, CKDLog::LOGLV_INFO);
#endif
	g_pTheLog->SetMaxLogSize(1*1024*1024, false);
	g_pTheLog->Log(_T("Init MagicKDApp"), CKDLog::LOGLV_INFO);

	sBuf.Format(_T("%sMagicKD.ini"), GetAppConfDir());
	m_cIni.SetPathName(sBuf);

	g_pTheAppEndDlg = new CMagicKDEndDlg;
	g_pTheAppEndDlg->Create(IDD_MAGICKD_END);

	CMagicKDDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	g_pTheLog->Log(_T("Exit MagicKDApp"), CKDLog::LOGLV_INFO);
	DEL(g_pTheAppEndDlg);
	DEL(g_pTheLog);

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}

void CMagicKDApp::Quit()
{
	if (m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd())) {
		::PostMessage(m_pMainWnd->GetSafeHwnd(), WM_QUIT, 0, 0);
		m_bOnQuit = true;
	}
}

bool CMagicKDApp::IsOnQuit()
{
	return m_bOnQuit;
}
