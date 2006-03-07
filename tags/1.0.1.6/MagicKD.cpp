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

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
	return FALSE;
}

void CMagicKDApp::Quit()
{
	if (m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd()))
		::PostMessage(m_pMainWnd->GetSafeHwnd(), WM_QUIT, 0, 0);
}