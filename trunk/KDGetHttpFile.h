#pragma once
#include "afxinet.h"
#include "KDThread.h"

class CKDGetHttpFile : public CKDThread
{
public:
	CKDGetHttpFile();
	virtual ~CKDGetHttpFile();
	virtual DWORD ThreadProc();

	bool AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath);
	bool AddFileListQuick(LPCTSTR lpURL, LPCTSTR lpLocalPath);

	__inline bool IsDownloadAllOver() { return (!IsThreadRunning() && m_slURL.IsEmpty()); }
	CString GetNowDLURL() { m_muxNowDLURL.Lock(); CString sRes = m_sNowDLURL; m_muxNowDLURL.Unlock(); return sRes; }
	CString GetNowDLLocalPath() { CString sRes = m_sNowDLLocalPath; return sRes; }
	double GetPercentOfNowDL();
	double GetPercentOfTotalDL();

	CStringList m_slURL;
	CStringList m_slLocalPath;

private:
	bool DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize = 8192);

private:
	CMutex m_muxNowDLURL;
	CString m_sNowDLURL;
	CString m_sNowDLLocalPath;
	ULONGLONG m_ulNowDLMaxSize;
	ULONGLONG m_ulNowDLSize;
	ULONGLONG m_ulTotalDLMaxSize;
	ULONGLONG m_ulTotalDLSize;
};
