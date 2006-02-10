// MagicKD.cpp : 定義應用程式的類別行為。
//

#include "stdafx.h"
#include "MagicKDDlg.h"

#include "MagicKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMagicKDApp theApp;
CMagicKDEndDlg *pTheAppEndDlg = NULL;

CMagicKDApp::CMagicKDApp()
{
}

BEGIN_MESSAGE_MAP(CMagicKDApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsDialogRunning())
		return FALSE;

	{
		TCHAR sBuffer[MAX_PATH], *ptr;
		GetModuleFileName(NULL, sBuffer, MAX_PATH);
		m_sAppPath = sBuffer;
		if (ptr = _tcsrchr(sBuffer, _T('\\'))) {
			ptr++;
			*ptr = _T('\0');
			SetCurrentDirectory(sBuffer);
			m_sAppDir = sBuffer;
			m_cIni.SetPathName(m_sAppDir + _T("MagicKD.ini"));
		} else {
			MessageBox(NULL, _T("Can not locate the execution file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

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
