#include "StdAfx.h"
#include "KDGetHttpFile.h"

CKDGetHttpFile::CKDGetHttpFile()
	:	m_ulNowDLMaxSize(0), m_ulNowDLSize(0), m_ulTotalDLMaxSize(0), m_ulTotalDLSize(0), m_uQueryRetryTimes(0),
		m_bDiscardNowDL(false), m_fNowDLMaxPercent(0)
{
}

CKDGetHttpFile::CKDGetHttpFile(LPCTSTR lpURL, LPCTSTR lpLocalPath)
	:	m_ulNowDLMaxSize(0), m_ulNowDLSize(0), m_ulTotalDLMaxSize(0), m_ulTotalDLSize(0), m_uQueryRetryTimes(0),
		m_bDiscardNowDL(false), m_fNowDLMaxPercent(0)
{
	DownloadFileFromHttp(lpURL, lpLocalPath);
}

CKDGetHttpFile::~CKDGetHttpFile()
{
	Destroy();
}

void CKDGetHttpFile::Destroy()
{
	SetCanThread(false);
	Pause(false);
	if (WAIT_TIMEOUT == WaitForThread(20000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CKDGetHttpFile Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}
}

DWORD CKDGetHttpFile::ThreadProc()
{
	CInternetSession session(_T("Download File Thread Session"));
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 6000);
	session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 6000);
	CHttpFile *pFile = NULL;
	HANDLE hLocalFile = INVALID_HANDLE_VALUE;
	CString sMaxSize;
	CString sLocalDir;
	static const int iQuerySize = 8192;
	const int iQueryMaxTimes = 50;
	BYTE *pBuf = new BYTE[iQuerySize];
	UINT uReadLen = 0;
	DWORD dwWriteLen = 0;
	bool bDLOver = true;
	m_uQueryRetryTimes = 0;
	OnStartDownloadThread();

	while (IsCanThread() && !m_slURL.IsEmpty()) {
		WaitForSingleObject(m_semPause, INFINITE);
		m_semPause.Unlock();
		if (!IsCanThread())
			break;

		m_muxNowDLURL.Lock();
		m_sNowDLURL = m_slURL.RemoveHead();
		sLocalDir = m_sNowDLLocalPath = m_slLocalPath.RemoveHead();
		m_ulNowDLSize = m_ulNowDLMaxSize = 0;
		hLocalFile = CreateFile(m_sNowDLLocalPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		m_muxNowDLURL.Unlock();

		PathRemoveFileSpec(sLocalDir.GetBuffer());
		sLocalDir.ReleaseBuffer();
		if (!PathIsDirectory(sLocalDir))
			SHCreateDirectoryEx(NULL, sLocalDir, NULL);

		m_bDiscardNowDL = false;
		m_fNowDLMaxPercent = 0;
		bDLOver = true;
		TRY {
			pFile = (CHttpFile *)session.OpenURL(m_sNowDLURL);
		} CATCH_ALL (e) {
#ifdef DEBUG
//			e->ReportError();
#endif //DEBUG
			bDLOver = false;
		} END_CATCH_ALL;

		if (pFile && (hLocalFile != INVALID_HANDLE_VALUE)) {
			pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, sMaxSize);
			m_muxNowDLURL.Lock();
			m_ulNowDLMaxSize = _ttoi64(sMaxSize);
			m_muxNowDLURL.Unlock();

			uReadLen = 1;
			while (uReadLen && IsCanThread() && !m_bDiscardNowDL) {
				WaitForSingleObject(m_semPause, INFINITE);
				m_semPause.Unlock();

				TRY {
					uReadLen = pFile->Read(pBuf, iQuerySize);
				} CATCH_ALL (e) {
#ifdef DEBUG
//					e->ReportError();
#endif //DEBUG
					bDLOver = false;
					break;
				} END_CATCH_ALL;
				WriteFile(hLocalFile, pBuf, uReadLen, &dwWriteLen, NULL);
				m_ulNowDLSize += dwWriteLen;
				m_ulTotalDLSize += dwWriteLen;

				double fDLPercent = GetPercentOfNowDL();
				m_fNowDLMaxPercent = (fDLPercent > m_fNowDLMaxPercent) ? fDLPercent : m_fNowDLMaxPercent;
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
		if (!bDLOver || !IsCanThread() || m_bDiscardNowDL) {
			DeleteFile(m_sNowDLLocalPath);
			if (m_bDiscardNowDL) {
				OnDownloadFileDiscard();
			} else if (m_uQueryRetryTimes++ >= iQueryMaxTimes) {
				OnDownloadFileRetryFailed();
			} else {
				while ((0 != GetOnInternet()) && IsCanThread())
					Sleep(5000);
				SaveNowDLToList();
			}
		} else {
			m_uQueryRetryTimes = 0;
			OnDownloadFileOver();
		}
	}

	m_muxNowDLURL.Lock();
	m_sNowDLURL.Empty();
	m_sNowDLLocalPath.Empty();
	m_muxNowDLURL.Unlock();
	session.Close();
	delete [] pBuf;
	OnExitDownloadThread();
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

int CKDGetHttpFile::GetDownloadCount()
{
	int iRes;
	m_muxNowDLURL.Lock();
	iRes = max(m_slURL.GetCount(), m_slLocalPath.GetCount());
	m_muxNowDLURL.Unlock();
	return iRes;
}

UINT CKDGetHttpFile::GetNowDLRetryTimes()
{
	return m_uQueryRetryTimes;
}

CString CKDGetHttpFile::RemoveHeadDLURL()
{
	CString sRes;
	m_muxNowDLURL.Lock();
	sRes = m_slURL.RemoveHead();
	m_muxNowDLURL.Unlock();
	return sRes;
}

CString CKDGetHttpFile::RemoveHeadLocalPath()
{
	CString sRes;
	m_muxNowDLURL.Lock();
	sRes = m_slLocalPath.RemoveHead();
	m_muxNowDLURL.Unlock();
	return sRes;
}

bool CKDGetHttpFile::SaveNowDLToList(bool bHead/* = true*/)
{
	if (m_sNowDLURL.IsEmpty())
		return false;
	if (m_sNowDLLocalPath.IsEmpty())
		return false;

	m_muxNowDLURL.Lock();
	if (bHead) {
		m_slURL.AddHead(m_sNowDLURL);
		m_slLocalPath.AddHead(m_sNowDLLocalPath);
	} else {
		m_slURL.AddTail(m_sNowDLURL);
		m_slLocalPath.AddTail(m_sNowDLLocalPath);
	}
	m_muxNowDLURL.Unlock();
	return true;
}

void CKDGetHttpFile::SetDiscardNowDL(bool bDiscard/* = true*/)
{
	m_bDiscardNowDL = bDiscard;
}

void CKDGetHttpFile::Pause(bool bPause/* = true*/)
{
	if (!IsCanThread()) {
		m_semPause.Unlock();
		return;
	}
	if (bPause)
		m_semPause.Lock(0);
	else
		m_semPause.Unlock();
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
