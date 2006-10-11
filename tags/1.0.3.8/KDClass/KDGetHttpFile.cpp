#include "StdAfx.h"
#include "Others.h"
#include "MagicKDLog.h"
#include "KDGetHttpFile.h"

#define MAX_RETRY_TIMES 50
#define QUERY_BLOCK_SIZE 8192
#define INTERNET_ACCESS_TIMEOUT 10000 // (ms)

CKDGetHttpFile::CKDGetHttpFile()
	:	m_ulNowDLMaxSize(0), m_ulNowDLSize(0), m_uQueryRetryTimes(0),
		m_bDiscardNowDL(false), m_fNowDLMaxPercent(0), m_pSession(NULL)
{
	Pause(false);
}

CKDGetHttpFile::CKDGetHttpFile(LPCTSTR lpURL, LPCTSTR lpLocalPath)
	:	m_ulNowDLMaxSize(0), m_ulNowDLSize(0), m_uQueryRetryTimes(0),
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

	if (m_pSession) {
		m_pSession->Close();
		delete m_pSession;
		m_pSession = NULL;
	}
}

#define GO1(x) { ilStatus.AddTail((x)); continue; }
#define GO2(x,y) { ilStatus.AddTail((x)); ilStatus.AddTail((y)); continue; }
#define GO3(x,y,z) { ilStatus.AddTail((x)); ilStatus.AddTail((y)); ilStatus.AddTail((z)); continue; }
#define GO4(x,y,z,w) { ilStatus.AddTail((x)); ilStatus.AddTail((y)); ilStatus.AddTail((z)); ilStatus.AddTail((w)); continue; }
DWORD CKDGetHttpFile::ThreadProc()
{
	CString sBuf;
	CHttpFile *pHttpFile = NULL;
	HANDLE hLocalFile = INVALID_HANDLE_VALUE;
	BYTE *pBuf = new BYTE[QUERY_BLOCK_SIZE];
	UINT uReadLen = 0;
	DWORD dwWriteLen = 0;

	if (!m_pSession) {
		m_pSession = new CInternetSession(_T("Download File Thread Session"));
		m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, INTERNET_ACCESS_TIMEOUT);
		m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, INTERNET_ACCESS_TIMEOUT);
	}

	OnStartDownloadThread();

	bool bLoop = true;
	CList<int, int> ilStatus;
	ilStatus.AddTail(DLSTATUS_INIT);
	int iStatus = DLSTATUS_INIT;
	while (bLoop) {
		if (ilStatus.IsEmpty() || !IsCanThread())
			iStatus = DLSTATUS_QUIT;
		else
			iStatus = ilStatus.RemoveHead();

		switch (iStatus) {
		case DLSTATUS_INIT:
			m_uQueryRetryTimes = 0;
			m_fNowDLMaxPercent = 0;
			m_bDiscardNowDL = false;

			m_muxNowDLURL.Lock();
			m_ulNowDLMaxSize = 0;
			m_ulNowDLSize = 0;
			if (m_slURL.IsEmpty()) {
				m_sNowDLURL.Empty();
				m_sNowDLLocalPath.Empty();
				m_muxNowDLURL.Unlock();
				GO1(DLSTATUS_QUIT);
			} else {
				m_sNowDLURL = m_slURL.GetHead();
				m_sNowDLLocalPath = m_slLocalPath.GetHead();
			}
			m_muxNowDLURL.Unlock();

			GO1(DLSTATUS_CHKDLDIR);
		case DLSTATUS_CHKDLDIR:
			m_muxNowDLURL.Lock();
			sBuf = m_sNowDLLocalPath;
			m_muxNowDLURL.Unlock();
			PathRemoveFileSpec(sBuf.GetBuffer());
			sBuf.ReleaseBuffer();
			if (!sBuf.IsEmpty() && !PathIsDirectory(sBuf)) {
				if (ERROR_SUCCESS != SHCreateDirectoryEx(NULL, sBuf, NULL)) {
					GO2(DLSTATUS_CHKUSER, DLSTATUS_LOCALFILEERROR);
				}
			}
			GO2(DLSTATUS_CHKUSER, DLSTATUS_SETLOCAL);
		case DLSTATUS_SETLOCAL:
			m_muxNowDLURL.Lock();
			hLocalFile = CreateFile(m_sNowDLLocalPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			m_muxNowDLURL.Unlock();
			if (hLocalFile == INVALID_HANDLE_VALUE)
				GO2(DLSTATUS_CHKUSER, DLSTATUS_LOCALFILEERROR);
			GO2(DLSTATUS_CHKUSER, DLSTATUS_CONNECTHTTP);
		case DLSTATUS_CONNECTHTTP:
			TRY {
				m_muxNowDLURL.Lock();
				sBuf = m_sNowDLURL;
				m_muxNowDLURL.Unlock();
				pHttpFile = (CHttpFile *)m_pSession->OpenURL(sBuf);
			} CATCH_ALL (e) {
#ifdef DEBUG
//				e->ReportError();
#endif //DEBUG
				if (++m_uQueryRetryTimes > MAX_RETRY_TIMES) {
					GO2(DLSTATUS_CHKUSER, DLSTATUS_OVERRETRY);
				} else {
					GO3(DLSTATUS_CHKUSER, DLSTATUS_CHKNET, DLSTATUS_CONNECTHTTP);
				}
			} END_CATCH_ALL;

			pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, sBuf);
			m_ulNowDLMaxSize = _ttoi64(sBuf);
			m_ulNowDLSize = 0;

			GO2(DLSTATUS_CHKUSER, DLSTATUS_DOWNLOAD);
		case DLSTATUS_DOWNLOAD:
			TRY {
				uReadLen = pHttpFile->Read(pBuf, QUERY_BLOCK_SIZE);
			} CATCH_ALL (e) {
#ifdef DEBUG
//			e->ReportError();
#endif //DEBUG
				if (++m_uQueryRetryTimes > MAX_RETRY_TIMES) {
					GO2(DLSTATUS_CHKUSER, DLSTATUS_OVERRETRY);
				} else {
					GO3(DLSTATUS_CHKUSER, DLSTATUS_CLOSEVAR, DLSTATUS_SETLOCAL);
				}
			} END_CATCH_ALL;

			if (WriteFile(hLocalFile, pBuf, uReadLen, &dwWriteLen, NULL) == FALSE) {
				GO2(DLSTATUS_CHKUSER, DLSTATUS_LOCALFILEERROR);
			}

			if (uReadLen) {
				m_ulNowDLSize += dwWriteLen;

				m_fNowDLMaxPercent = max(m_fNowDLMaxPercent, GetPercentOfNowDL());
				GO2(DLSTATUS_CHKUSER, DLSTATUS_DOWNLOAD);
			} else {
				GO2(DLSTATUS_CHKUSER, DLSTATUS_DLOVER);
			}
		case DLSTATUS_CHKUSER:
			m_eventPause.Lock();
			if (m_bDiscardNowDL) {
				ilStatus.RemoveAll();
				GO1(DLSTATUS_DISCARD);
			}
			continue;
		case DLSTATUS_CHKNET:
			while (0 != GetOnInternet()) {
				if (!IsCanThread())
					break;
				Sleep(5000);
			}
			continue;
		case DLSTATUS_RMLIST:
			m_muxNowDLURL.Lock();
			m_slLocalPath.RemoveHead();
			m_slURL.RemoveHead();
			m_muxNowDLURL.Unlock();
			continue;
		case DLSTATUS_CLOSEVAR:
			if (hLocalFile != INVALID_HANDLE_VALUE) {
				CloseHandle(hLocalFile);
				hLocalFile = INVALID_HANDLE_VALUE;
			}
			if (pHttpFile) {
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			continue;
		case DLSTATUS_QUIT:
			if (hLocalFile != INVALID_HANDLE_VALUE) {
				CloseHandle(hLocalFile);
				hLocalFile = INVALID_HANDLE_VALUE;
			}
			if (pHttpFile) {
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			bLoop = false;
			break;
		case DLSTATUS_DLOVER:
			OnDownloadFileOver();
			GO4(DLSTATUS_CHKUSER, DLSTATUS_RMLIST, DLSTATUS_CLOSEVAR, DLSTATUS_INIT);
		case DLSTATUS_LOCALFILEERROR:
			OnWriteFileError();
			GO3(DLSTATUS_CHKUSER, DLSTATUS_CLOSEVAR, DLSTATUS_INIT);
		case DLSTATUS_OVERRETRY:
			OnDownloadFileRetryFailed();
			GO4(DLSTATUS_CHKUSER, DLSTATUS_RMLIST, DLSTATUS_CLOSEVAR, DLSTATUS_INIT);
		case DLSTATUS_DISCARD:
			OnDownloadFileDiscard();
			GO3(DLSTATUS_RMLIST, DLSTATUS_CLOSEVAR, DLSTATUS_INIT);
		default:
			continue;
		}
	}

	m_muxNowDLURL.Lock();
	m_sNowDLURL.Empty();
	m_sNowDLLocalPath.Empty();
	m_muxNowDLURL.Unlock();
	delete [] pBuf;
	OnExitDownloadThread();
	return 0;
}
#undef GO1
#undef GO2
#undef GO3
#undef GO4

bool CKDGetHttpFile::AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath)
{
	if (_tcslen(lpURL) < 1)
		return false;
	if (_tcslen(lpLocalPath) < 1)
		return false;

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
	if (fRes < 0)
		fRes = 0;
	return fRes;
}

INT_PTR CKDGetHttpFile::GetDownloadCount()
{
	INT_PTR iRes;
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
	if (bPause)
		m_eventPause.ResetEvent();
	else
		m_eventPause.SetEvent();
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
