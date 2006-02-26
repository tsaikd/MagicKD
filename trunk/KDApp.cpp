#include "StdAfx.h"
#include "KDApp.h"

CKDApp::CKDApp()
	:	m_bRestart(false), m_lpAppPath(NULL), m_lpAppDir(NULL), m_lpAppConfDir(NULL),
		m_lpAppLangDir(NULL), m_lpAppFileVer(NULL), m_lpAppProductVer(NULL)
{
	size_t u64Len;
	TCHAR sBuffer[MAX_PATH], *ptr;

	u64Len = GetModuleFileName(NULL, sBuffer, MAX_PATH);
	if (u64Len) {
		m_lpAppPath = new TCHAR[u64Len + 1];
		_tcscpy((LPTSTR)m_lpAppPath, sBuffer);
	}

	ptr = _tcsrchr(sBuffer, _T('\\'));
	if (ptr) {
		*(ptr+1) = _T('\0');
		SetCurrentDirectory(sBuffer);

		u64Len = _tcslen(sBuffer) + 1;
		m_lpAppDir = new TCHAR[u64Len];
		_tcscpy((LPTSTR)m_lpAppDir, sBuffer);

		m_lpAppConfDir = new TCHAR[u64Len];
		_stprintf((LPTSTR)m_lpAppConfDir, _T("%s"), m_lpAppDir);
//		m_lpAppConfDir = new TCHAR[u64Len + _tcslen(_T("conf\\"))];
//		_stprintf((LPTSTR)m_lpAppConfDir, _T("%sconf\\"), m_lpAppDir);

		m_lpAppLangDir = new TCHAR[u64Len + _tcslen(_T("lang\\"))];
		_stprintf((LPTSTR)m_lpAppLangDir, _T("%slang\\"), m_lpAppDir);
	} else {
		MessageBox(NULL, _T("Can not locate the execution file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
	}

	if (m_lpAppPath) {
		u64Len = GetFileVersionInfoSize(m_lpAppPath, NULL);
		if (u64Len) {
			CString sVer;
			TCHAR *btVersion;
			UINT uVersionLen;
			BYTE *pData = new BYTE[u64Len];

			GetFileVersionInfo(m_lpAppPath, NULL, u64Len, pData);
			if (VerQueryValue(pData, _T("\\StringFileInfo\\040403b6\\FileVersion"), (LPVOID *)&btVersion, &uVersionLen)) {
				sVer = btVersion;
				sVer.Replace(_T(" "), _T(""));
				m_lpAppFileVer = new TCHAR[uVersionLen + 1];
				_tcscpy((LPTSTR)m_lpAppFileVer, sVer);
			}
			if (VerQueryValue(pData, _T("\\StringFileInfo\\040403b6\\ProductVersion"), (LPVOID *)&btVersion, &uVersionLen)) {
				sVer = btVersion;
				sVer.Replace(_T(" "), _T(""));
				m_lpAppProductVer = new TCHAR[uVersionLen + 1];
				_tcscpy((LPTSTR)m_lpAppProductVer, sVer);
			}

			delete [] pData;
		}
	}
}

CKDApp::~CKDApp()
{
	if (m_bRestart)
		ShellExecute(NULL, _T("open"), GetAppPath(), NULL, NULL, SW_SHOW);

	if (m_lpAppPath)
		delete [] m_lpAppPath;
	if (m_lpAppDir)
		delete [] m_lpAppDir;
	if (m_lpAppConfDir)
		delete [] m_lpAppConfDir;
	if (m_lpAppLangDir)
		delete [] m_lpAppLangDir;
	if (m_lpAppFileVer)
		delete [] m_lpAppFileVer;
	if (m_lpAppProductVer)
		delete [] m_lpAppProductVer;
}
