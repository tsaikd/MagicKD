#include "StdAfx.h"
#include "KDGetHttpFile.h"

CKDGetHttpFile::CKDGetHttpFile()
	:	m_ulNowDLMaxSize(0), m_ulNowDLSize(0), m_ulTotalDLMaxSize(0), m_ulTotalDLSize(0)
{
}

CKDGetHttpFile::~CKDGetHttpFile()
{
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CKDGetHttpFile Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}
}

DWORD CKDGetHttpFile::ThreadProc()
{
	CInternetSession session(_T("Download File Thread Session"));
//	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, _T("1000"), _tcslen(_T("1000")));
//	session.SetOption(INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, _T("1000"), _tcslen(_T("1000")));
	CStdioFile *pFile = NULL;
	HANDLE hLocalFile = INVALID_HANDLE_VALUE;
	CString sMaxSize;
	static const int iQuerySize = 16384;
	BYTE *pBuf = new BYTE[iQuerySize];
	UINT uReadLen;
	DWORD dwWriteLen;

	while (IsCanThread() && !m_slURL.IsEmpty()) {
		m_muxNowDLURL.Lock();
		m_sNowDLURL = m_slURL.RemoveHead();
		m_ulNowDLSize = m_ulNowDLMaxSize = 0;
		m_muxNowDLURL.Unlock();

		m_sNowDLLocalPath = m_slLocalPath.RemoveHead();

		pFile = session.OpenURL(m_sNowDLURL);
		hLocalFile = CreateFile(m_sNowDLLocalPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (pFile && (hLocalFile != INVALID_HANDLE_VALUE)) {
			((CHttpFile *)pFile)->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, sMaxSize);
			m_muxNowDLURL.Lock();
			m_ulNowDLMaxSize = _ttoi64(sMaxSize);
			m_muxNowDLURL.Unlock();

			while ((uReadLen = pFile->Read(pBuf, iQuerySize)) && (IsCanThread())) {
				WriteFile(hLocalFile, pBuf, uReadLen, &dwWriteLen, NULL);
				m_ulNowDLSize += dwWriteLen;
				m_ulTotalDLSize += dwWriteLen;
			}
		}
		if (hLocalFile != INVALID_HANDLE_VALUE) {
			CloseHandle(hLocalFile);
			hLocalFile = INVALID_HANDLE_VALUE;
		}
		if (pFile) {
			pFile->Close();
			delete pFile;
			pFile = NULL;
		}
	}

	session.Close();
	delete [] pBuf;
	return 0;
}

bool CKDGetHttpFile::AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath)
{
	bool bRes = false;
	CInternetSession session(CString(_T("Download File Session")) + lpLocalPath);
	CStdioFile *pFile = session.OpenURL(lpURL);
	CString sMaxSize;

	if (pFile) {
		((CHttpFile *)pFile)->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, sMaxSize);
		m_muxNowDLURL.Lock();
		m_ulTotalDLMaxSize += _ttoi64(sMaxSize);
		m_muxNowDLURL.Unlock();

		m_slURL.AddTail(lpURL);
		m_slLocalPath.AddTail(lpLocalPath);

		if (!IsThreadRunning())
			CreateThread(THREAD_PRIORITY_LOWEST);

		bRes = true;
	} else {
		bRes = false;
	}

	if (pFile) {
		pFile->Close();
		delete pFile;
	}
	session.Close();

	return bRes;
}

bool CKDGetHttpFile::AddFileListQuick(LPCTSTR lpURL, LPCTSTR lpLocalPath)
{
	m_slURL.AddTail(lpURL);
	m_slLocalPath.AddTail(lpLocalPath);

	if (!IsThreadRunning())
		CreateThread(THREAD_PRIORITY_LOWEST);

	return true;
}

double CKDGetHttpFile::GetPercentOfNowDL()
{
	if (m_ulNowDLMaxSize < 1)
		return 0;

	double fRes = 0.0;
	m_muxNowDLURL.Lock();

	fRes = (double)m_ulNowDLSize / m_ulNowDLMaxSize;

	m_muxNowDLURL.Unlock();
	return fRes;
}

double CKDGetHttpFile::GetPercentOfTotalDL()
{
	if (m_ulTotalDLMaxSize < 1)
		return 0;

	double fRes = 0.0;
	m_muxNowDLURL.Lock();

	fRes = (double)m_ulTotalDLSize / m_ulTotalDLMaxSize;

	m_muxNowDLURL.Unlock();
	return fRes;
}

bool CKDGetHttpFile::DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize/* = 8192*/)
{
	bool bRes = false;
	CInternetSession session(_T("Download File Session"));
	CStdioFile *pFile = session.OpenURL(lpURL);
	HANDLE hLocalFile = CreateFile(lpLocalPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pFile && (hLocalFile != INVALID_HANDLE_VALUE)) {
		BYTE *pBuf = new BYTE[iQuerySize];
		UINT uReadLen; 
		DWORD dwWriteLen; 

		while (uReadLen = pFile->Read(pBuf, iQuerySize))
			WriteFile(hLocalFile, pBuf, uReadLen, &dwWriteLen, NULL);

		delete pBuf;
		CloseHandle(hLocalFile);
		bRes = true;
	} else {
		bRes = false;
	}

	if (pFile) {
		pFile->Close();
		delete pFile;
	}

	session.Close();
	return bRes;
}
