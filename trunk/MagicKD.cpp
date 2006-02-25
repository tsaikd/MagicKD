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
	:	m_bRestart(false)
{
}

void CMagicKDApp::SetRestart(bool bRestart/* = true*/)
{
	m_bRestart = bRestart;
}

CString CMagicKDApp::GetAppFileVer()
{
	CString sRes;
	DWORD dwVerInfoSize = GetFileVersionInfoSize(theApp.m_sAppPath, NULL);
	if (dwVerInfoSize) {
		BYTE *pData = new BYTE[dwVerInfoSize];
		GetFileVersionInfo(theApp.m_sAppPath, NULL, dwVerInfoSize, pData);
		TCHAR *btVersion;
		UINT uVersionLen;
		if (VerQueryValue(pData, _T("\\StringFileInfo\\040403b6\\FileVersion"), (LPVOID *)&btVersion, &uVersionLen)) {
			sRes = btVersion;
			sRes.Replace(_T(" "), _T(""));
		}
		delete [] pData;
	}

	return sRes;
}

CString CMagicKDApp::GetAppProductVer()
{
	CString sRes;
	DWORD dwVerInfoSize = GetFileVersionInfoSize(theApp.m_sAppPath, NULL);
	if (dwVerInfoSize) {
		BYTE *pData = new BYTE[dwVerInfoSize];
		GetFileVersionInfo(theApp.m_sAppPath, NULL, dwVerInfoSize, pData);
		TCHAR *btVersion;
		UINT uVersionLen;
		if (VerQueryValue(pData, _T("\\StringFileInfo\\040403b6\\ProductVersion"), (LPVOID *)&btVersion, &uVersionLen)) {
			sRes = btVersion;
			sRes.Replace(_T(" "), _T(""));
		}
		delete [] pData;
	}

	return sRes;
}

BEGIN_MESSAGE_MAP(CMagicKDApp, CWinApp)
END_MESSAGE_MAP()

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsWndRunning())
		return FALSE;

	{
		TCHAR sBuffer[MAX_PATH], *ptr;
		GetModuleFileName(NULL, sBuffer, MAX_PATH);
		m_sAppPath = sBuffer;
		ptr = _tcsrchr(sBuffer, _T('\\'));
		if (ptr) {
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

	if (m_bRestart)
		ShellExecute(NULL, _T("open"), m_sAppDir + _T("MagicKD.exe"), NULL, NULL, SW_SHOW);

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
	return FALSE;
}
