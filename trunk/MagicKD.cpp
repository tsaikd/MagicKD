#include "stdafx.h"
#include "MagicKDDlg.h"

#include "MagicKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMagicKDApp theApp;
CMagicKDConf *pTheConf = NULL;
CKDTray *pTheTray = NULL;
CMagicKDEndDlg *pTheAppEndDlg = NULL;

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

	if (!pTheAppEndDlg)
		pTheAppEndDlg = new CMagicKDEndDlg;
	pTheAppEndDlg->Create(IDD_MAGICKD_END);

	if (!pDlg)
		pDlg = new CMagicKDDlg;
	m_pMainWnd = pDlg;
	pDlg->DoModal();
	DEL(pDlg);

	delete pTheAppEndDlg;
	pTheAppEndDlg = NULL;

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
	return FALSE;
}

void CMagicKDApp::Quit()
{
	if (m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd()))
		::PostMessage(m_pMainWnd->GetSafeHwnd(), WM_QUIT, 0, 0);
}
