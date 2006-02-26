#include "stdafx.h"
#include "MagicKDDlg.h"

#include "MagicKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMagicKDApp theApp;
CMagicKDConf theConf;
CKDTray *pTheTray = NULL;
CMagicKDEndDlg *pTheAppEndDlg = NULL;

CMagicKDApp::CMagicKDApp()
{
}

BEGIN_MESSAGE_MAP(CMagicKDApp, CWinApp)
END_MESSAGE_MAP()

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsWndRunning())
		return FALSE;

	m_cIni.SetPathName(CString(GetAppConfDir()) + _T("MagicKD.ini"));

	CMagicKDDlg *pDlg = NULL;

	if (!pTheAppEndDlg)
		pTheAppEndDlg = new CMagicKDEndDlg;
	pTheAppEndDlg->Create(IDD_MAGICKD_END);

	if (!pDlg)
		pDlg = new CMagicKDDlg;
	m_pMainWnd = pDlg;
	pDlg->DoModal();
	delete pDlg;
	pDlg = NULL;

	delete pTheAppEndDlg;
	pTheAppEndDlg = NULL;

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}
