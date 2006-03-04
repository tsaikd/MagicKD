#include "StdAfx.h"
#include "afxinet.h"
#include "KDApp.h"

#define UPDATE_QUERY_SIZE 1024

CKDApp::CKDApp()
	:	m_bRestart(false),
		m_lpAppConfDir(NULL), m_lpAppLangDir(NULL),
#ifdef KDAPP_ENABLE_GETAPPVERSION
		m_lpAppFileVer(NULL), m_lpAppProductVer(NULL),
#endif //KDAPP_ENABLE_GETAPPVERSION
#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
		m_bUpdateApp(false), m_lpTmpBatchPath(NULL),
#endif //KDAPP_ENABLE_UPDATEAPPONLINE
		m_lpAppName(NULL), m_lpAppPath(NULL), m_lpAppDir(NULL)
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
		ptr++;
		PathRemoveExtension(ptr);
		m_lpAppName = new TCHAR[_tcslen(ptr) + 1];
		_tcscpy((LPTSTR)m_lpAppName, ptr);
		*(ptr) = _T('\0');
		SetCurrentDirectory(sBuffer);

		u64Len = _tcslen(sBuffer) + 1;
		m_lpAppDir = new TCHAR[u64Len];
		_tcscpy((LPTSTR)m_lpAppDir, sBuffer);

		m_lpAppConfDir = new TCHAR[u64Len + _tcslen(_T("conf\\"))];
		_stprintf((LPTSTR)m_lpAppConfDir, _T("%sconf\\"), m_lpAppDir);
		if (!PathFileExists(m_lpAppConfDir))
			::CreateDirectory(m_lpAppConfDir, NULL);

		m_lpAppLangDir = new TCHAR[u64Len + _tcslen(_T("lang\\"))];
		_stprintf((LPTSTR)m_lpAppLangDir, _T("%slang\\"), m_lpAppDir);
		if (!PathFileExists(m_lpAppLangDir))
			::CreateDirectory(m_lpAppLangDir, NULL);

#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
		m_lpTmpBatchPath = new TCHAR[u64Len + _tcslen(_T("tmp.cmd"))];
		_stprintf((LPTSTR)m_lpTmpBatchPath, _T("%stmp.vbs"), m_lpAppDir);
#endif
	} else {
		MessageBox(NULL, _T("Can not locate the execution file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
	}

#ifdef KDAPP_ENABLE_GETAPPVERSION
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
#endif //KDAPP_ENABLE_GETAPPVERSION
}

CKDApp::~CKDApp()
{
#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
	while (m_bUpdateApp && m_lpTmpBatchPath) {
		HANDLE hBatchFile = CreateFile(m_lpTmpBatchPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hBatchFile == INVALID_HANDLE_VALUE)
			break;

		int i, iCount = m_saOldAppPath.GetCount();
		if (!iCount || (iCount != m_saNewAppPath.GetCount()))
			break;

		DWORD dwWriteLen;
		CString sBatchContext;
		sBatchContext.Format(
			_T("sAppName = \"%s\"\n")
			_T("Const iArraySize = %d\n")
			_T("Dim sOldFile(%d)\n")
			_T("Dim sNewFile(%d)\n")
			_T("\n"),
			m_lpAppName,
			iCount,
			iCount - 1,
			iCount - 1
			);
		for (i=0 ; i<iCount ; i++) {
			sBatchContext.AppendFormat(
				_T("sOldFile(%d) = \"%s\"\n")
				_T("sNewFile(%d) = \"%s\"\n"),
				i, m_saOldAppPath[i],
				i, m_saNewAppPath[i]
				);
		}
		sBatchContext.AppendFormat(
			_T("\n")
			_T("Set objShell = CreateObject(\"WScript.Shell\")\n")
			_T("Set objFS = CreateObject(\"Scripting.FileSystemObject\")\n")
			_T("\n")
			_T("For i=0 To (iArraySize-1)\n")
			_T("	iLoopMaxTimes = 30\n")
			_T("	While objFS.FileExists(sOldFile(i)) And (iLoopMaxTimes > 0)\n")
			_T("		objFS.DeleteFile sOldFile(i), True\n")
			_T("		iLoopMaxTimes = iLoopMaxTimes - 1\n")
			_T("		If (iLoopMaxTimes < 25) Then\n")
			_T("			WScript.Sleep(1000)\n")
			_T("		End If\n")
			_T("	Wend\n")
			_T("	If objFS.FileExists(sNewFile(i)) Then\n")
			_T("		objFS.MoveFile sNewFile(i), sOldFile(i)\n")
			_T("	End If\n")
			_T("Next\n")
			_T("\n")
			_T("MsgBox \"Application Updated\", vbOKOnly + vbInformation , sAppName\n")
			_T("If objFS.FileExists(WScript.ScriptFullName) Then\n")
			_T("	objFS.DeleteFile WScript.ScriptFullName, True\n")
			_T("End If\n")
			_T("objShell.Run sAppName + \".exe\"\n")
			);

		CStringA sBatchContextA;
		sBatchContextA = sBatchContext;
		WriteFile(hBatchFile, sBatchContextA, sBatchContextA.GetLength(), &dwWriteLen, NULL);
		CloseHandle(hBatchFile);

		ShellExecute(NULL, _T("open"), m_lpTmpBatchPath, NULL, NULL, SW_SHOW);

		m_bRestart = false;
		m_bUpdateApp = false;
	}
	if (m_lpTmpBatchPath)
		delete [] m_lpTmpBatchPath;
#endif

	if (m_bRestart)
		ShellExecute(NULL, _T("open"), m_lpAppPath, NULL, NULL, SW_SHOW);

	if (m_lpAppName)
		delete [] m_lpAppName;
	if (m_lpAppPath)
		delete [] m_lpAppPath;
	if (m_lpAppDir)
		delete [] m_lpAppDir;
	if (m_lpAppConfDir)
		delete [] m_lpAppConfDir;
	if (m_lpAppLangDir)
		delete [] m_lpAppLangDir;
#ifdef KDAPP_ENABLE_GETAPPVERSION
	if (m_lpAppFileVer)
		delete [] m_lpAppFileVer;
	if (m_lpAppProductVer)
		delete [] m_lpAppProductVer;
#endif //KDAPP_ENABLE_GETAPPVERSION
}

#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
CString CKDApp::GetUpdateAppOnLineVer(LPCTSTR lpQueryUrl, LPCTSTR lpQueryKeyword,
	LONGLONG i64QueryOffset, short unsigned int iQueryVerSize)
{
	CString sRes;

	CInternetSession session(_T("Update App Session"));
	CStdioFile *pFile = NULL;
	TRY {
		pFile = session.OpenURL(lpQueryUrl);
	} CATCH_ALL(e) {
	} END_CATCH_ALL
	if (pFile) {
		CString sPageContext;
		CString sBuf;
		char *pBuf = new char[UPDATE_QUERY_SIZE];

		while (pFile->Read(pBuf, UPDATE_QUERY_SIZE)) {
			sBuf = pBuf;
			sPageContext.Append(sBuf);
		}
		int iPos = sPageContext.Find(lpQueryKeyword);
		if (-1 != iPos) {
			LPCTSTR lpPageContext = sPageContext;
			lpPageContext += iPos + i64QueryOffset;

			LPTSTR lpVersion = new TCHAR[iQueryVerSize + 1];
			_tcsncpy(lpVersion, lpPageContext, iQueryVerSize);
			*(lpVersion + iQueryVerSize) = _T('\0');

			sRes = lpVersion;
			delete [] lpVersion;
		}

		delete pBuf;
		pFile->Close();
		delete pFile;
	}

	session.Close();
	return sRes;
}

bool CKDApp::SetUpdateApp(LPCTSTR lpOldAppPath, LPCTSTR lpNewAppPath)
{
	if (lpOldAppPath && !PathFileExists(lpOldAppPath))
		return false;
	if (lpNewAppPath && !PathFileExists(lpNewAppPath))
		return false;

	m_saOldAppPath.RemoveAll();
	m_saNewAppPath.RemoveAll();

	m_saOldAppPath.SetSize(1);
	m_saNewAppPath.SetSize(1);

	m_saOldAppPath[0] = lpOldAppPath;
	m_saNewAppPath[0] = lpNewAppPath;

	m_bUpdateApp = true;
	return true;
}

bool CKDApp::SetUpdateApp(CStringArray &saOldAppPath, CStringArray &saNewAppPath)
{
	if (saOldAppPath.GetCount() != saNewAppPath.GetCount())
		return false;

	int i, iCount = saOldAppPath.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (!saOldAppPath.GetAt(i).IsEmpty() && !PathFileExists(saOldAppPath.GetAt(i)))
			return false;
		if (!saNewAppPath.GetAt(i).IsEmpty() && !PathFileExists(saNewAppPath.GetAt(i)))
			return false;
	}

	m_saOldAppPath.RemoveAll();
	m_saNewAppPath.RemoveAll();

	m_saOldAppPath.Append(saOldAppPath);
	m_saNewAppPath.Append(saNewAppPath);

	m_bUpdateApp = true;
	return true;
}
#endif
