#include "stdafx.h"
#include "MagicKDDlg.h"

#include "MagicKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMagicKDApp theApp;
CMagicKDConf *g_pTheConf = NULL;
CKDTray *g_pTheTray = NULL;
CMagicKDEndDlg *g_pTheAppEndDlg = NULL;

CMagicKDApp::CMagicKDApp()
	:	m_bOnQuit(false)
{
}

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsWndRunning())
		return FALSE;

	m_cIni.SetPathName(CString(GetAppConfDir()) + _T("MagicKD.ini"));

	CMagicKDDlg *pDlg = NULL;

	if (!g_pTheAppEndDlg)
		g_pTheAppEndDlg = new CMagicKDEndDlg;
	g_pTheAppEndDlg->Create(IDD_MAGICKD_END);

	if (!pDlg)
		pDlg = new CMagicKDDlg;
	m_pMainWnd = pDlg;
	pDlg->DoModal();
	DEL(pDlg);

	delete g_pTheAppEndDlg;
	g_pTheAppEndDlg = NULL;

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
